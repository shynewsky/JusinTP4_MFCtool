// Scenario.cpp: 구현 파일
//

#include "stdafx.h"
//#include "pch.h"
#include "Tool.h"
#include "afxdialogex.h"
#include "Scenario.h"


// CScenario 대화 상자

IMPLEMENT_DYNAMIC(CScenario, CDialog)

CScenario::CScenario(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_SCENARIO, pParent)
	, m_strChar(_T(""))
	, m_strFace(_T(""))
	, m_strSpeech(_T(""))
	, m_strText(_T(""))
{

}

CScenario::~CScenario()
{
}

void CScenario::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListControl);
	DDX_Text(pDX, IDC_EDIT1, m_strChar);
	DDX_Text(pDX, IDC_EDIT2, m_strFace);
	DDX_Text(pDX, IDC_EDIT3, m_strSpeech);
	DDX_Text(pDX, IDC_EDIT7, m_strText);
	DDX_Control(pDX, IDC_PICTURE, m_imgPic);
}


BEGIN_MESSAGE_MAP(CScenario, CDialog)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CScenario::OnListControl)
	ON_BN_CLICKED(IDC_BUTTON1, &CScenario::OnAdd)
	ON_BN_CLICKED(IDC_BUTTON10, &CScenario::OnDelete)
	ON_BN_CLICKED(IDC_BUTTON11, &CScenario::OnSave)
	ON_BN_CLICKED(IDC_BUTTON12, &CScenario::OnLoad)
	ON_BN_CLICKED(IDC_BUTTON13, &CScenario::OnFix)
END_MESSAGE_MAP()

// CScenario 메시지 처리기

BOOL CScenario::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 리스트 컨트롤의 크기 가져오기
	CRect rc;
	m_ListControl.GetClientRect(rc);

	// 리스트 컨트롤에 그리드 표시, 아이템 선택시 행 전체 선택
	m_ListControl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	// 헤더에 들어갈 이름들
	m_ListControl.InsertColumn(0, TEXT("번호"), LVCFMT_LEFT, rc.Width() * 0.1);
	m_ListControl.InsertColumn(1, TEXT("이름"), LVCFMT_LEFT, rc.Width() * 0.2);
	m_ListControl.InsertColumn(2, TEXT("표정"), LVCFMT_LEFT, rc.Width() * 0.2);
	m_ListControl.InsertColumn(3, TEXT("대사"), LVCFMT_LEFT, rc.Width() * 0.5);

	// 번호 초기화
	m_iCount = m_ListControl.GetItemCount();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CScenario::OnListControl(NMHDR* pNMHDR, LRESULT* pResult)
{
	//LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	UpdateData(TRUE);

	POSITION pos = m_ListControl.GetFirstSelectedItemPosition();
	int iIndex = m_ListControl.GetNextSelectedItem(pos);
	CString strKey = m_ListControl.GetItemText(iIndex, 0);

	auto& iter = m_mapScenarioData.find(strKey);

	if (iter == m_mapScenarioData.end())
		return;

	m_strChar = iter->second->strChar;
	m_strFace = iter->second->strFace;
	m_strSpeech = iter->second->strSpeech;

	m_strText = m_strChar + L"\r\n" + L"\r\n" + m_strSpeech;

	int iFace(0);

	if (m_strFace == L"normal")
	{
		iFace = 0;
	}
	else if(m_strFace == L"smile")
	{
		iFace = 1;
	}
	else if(m_strFace == L"angry")
	{
		iFace = 2;
	}
	else if(m_strFace == L"happy")
	{
		iFace = 3;
	}

	CString strPic;
	strPic.Format(L"../Texture/Stage/Player/Face/pikachu%d.bmp", iFace);

	HBITMAP	hBitmap = (HBITMAP)LoadImage(nullptr, strPic,
		IMAGE_BITMAP, 80, 80, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	m_imgPic.SetBitmap(hBitmap);

	UpdateData(FALSE);

	//*pResult = 0;
}

void CScenario::OnAdd()
{
	UpdateData(TRUE);

	SCENARIODATA* pScenario = new SCENARIODATA;
	pScenario->strChar = m_strChar;
	pScenario->strFace = m_strFace;
	pScenario->strSpeech = m_strSpeech;

	// 대사 데이터베이스에 추가한다
	CString strKey;
	strKey.Format(_T("%d"), m_iCount);
	m_mapScenarioData.insert({ strKey, pScenario });

	// 리스트 박스에 출력한다
	int iIndex = m_ListControl.GetItemCount();
	m_ListControl.InsertItem(iIndex, strKey);
	m_ListControl.SetItem(iIndex, 1, LVIF_TEXT, m_strChar, NULL,NULL,NULL,NULL);
	m_ListControl.SetItem(iIndex, 2, LVIF_TEXT, m_strFace, NULL,NULL,NULL,NULL);
	m_ListControl.SetItem(iIndex, 3, LVIF_TEXT, m_strSpeech, NULL,NULL,NULL,NULL);

	m_iCount++;

	UpdateData(FALSE);
}

void CScenario::OnDelete()
{
	// 현재 원소 가져오기
	POSITION pos = m_ListControl.GetFirstSelectedItemPosition();
	int iIndex = m_ListControl.GetNextSelectedItem(pos);
	CString strKey = m_ListControl.GetItemText(iIndex, 0);

	// 현재 원소 삭제하기
	auto& iter = m_mapScenarioData.find(strKey);

	if (iter == m_mapScenarioData.end()) // 반복자가 리스트를 가리키지 않을떄
		return;

	Safe_Delete(iter->second);
	m_mapScenarioData.erase(strKey);

	m_ListControl.DeleteItem(iIndex);
}

void CScenario::OnFix()
{
	UpdateData(TRUE);

	// 현재 원소 가져오기
	POSITION pos =  m_ListControl.GetFirstSelectedItemPosition();
	int iIndex = m_ListControl.GetNextSelectedItem(pos);
	CString strKey = m_ListControl.GetItemText(iIndex, 0);

	// 현재 원소 수정하기
	auto& iter = m_mapScenarioData.find(strKey);

	if (iter == m_mapScenarioData.end()) // 반복자가 리스트를 가리키지 않을떄
		return;

	iter->second->strChar = m_strChar;
	iter->second->strFace = m_strFace;
	iter->second->strSpeech = m_strSpeech;

	m_ListControl.SetItem(iIndex, 1, LVIF_TEXT, m_strChar, NULL, NULL, NULL, NULL);
	m_ListControl.SetItem(iIndex, 2, LVIF_TEXT, m_strFace, NULL, NULL, NULL, NULL);
	m_ListControl.SetItem(iIndex, 3, LVIF_TEXT, m_strSpeech, NULL, NULL, NULL, NULL);

	UpdateData(FALSE);
}


void CScenario::OnSave()
{
	// 다른이름으로 저장하기
	CFileDialog	Dlg( FALSE, L"txt", L"*.txt", 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"Data Files(*.txt)|*.txt||", this );	

	// 대화상자 디폴트 폴더 경로 만들기
	TCHAR	szPath[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, szPath); 	// 현재 프로젝트의 절대 경로
	PathRemoveFileSpec(szPath); // 현재 프로젝트가 있는 폴더 경로
	lstrcat(szPath, L"\\Data"); // Data 폴더로 들어가기
	Dlg.m_ofn.lpstrInitialDir = szPath; // 디폴트 폴더 경로로 설정

	// DoModal : 대화 상자 실행 함수
	if (IDOK == Dlg.DoModal())
	{
		// 파일을 선택하여 경로를 완성시킨다
		CString	str = Dlg.GetPathName().GetString();
		const TCHAR* pGetPath = str.GetString();

		// 스트림 개방 -> 쓰기 -> 닫기
		wofstream fout;
		fout.open(pGetPath, ios::out);

		if (!fout.fail())
		{
			for (auto& MyPair : m_mapScenarioData)
			{
				wstring	wstrChar(MyPair.second->strChar);
				wstring	wstrFace(MyPair.second->strFace);
				wstring	wstrSpeech(MyPair.second->strSpeech);

				fout << wstrChar << L"\t"
					<< wstrFace << L"\t"
					<< wstrSpeech << L"\t"
					<< endl;
			}

			fout.close();
		}
	}
}


void CScenario::OnLoad()
{
	UpdateData(TRUE);

	// 불러오기
	CFileDialog	Dlg(TRUE, L"txt", L"*.txt",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"Data Files(*.txt)|*.txt||", this);

	// 대화상자 디폴트 폴더 경로 만들기
	TCHAR	szPath[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, szPath); 	// 현재 프로젝트의 절대 경로
	PathRemoveFileSpec(szPath); // 현재 프로젝트가 있는 폴더 경로
	lstrcat(szPath, L"\\Data"); // Data 폴더로 들어가기
	Dlg.m_ofn.lpstrInitialDir = szPath; // 디폴트 폴더 경로로 설정

	// DoModal : 대화 상자 실행 함수
	if (IDOK == Dlg.DoModal())
	{
		// 파일을 선택하여 경로를 완성시킨다
		CString	str = Dlg.GetPathName().GetString();
		const TCHAR* pGetPath = str.GetString();

		// 스트림 개방 -> 읽기 -> 닫기
		wifstream fin;
		fin.open(pGetPath, ios::in);

		if (!fin.fail())
		{
			TCHAR	szChar[MAX_STR] = L"";
			TCHAR	szFace[MAX_STR] = L"";
			TCHAR	szSpeech[MAX_STR] = L"";
						
			m_iCount = 0;
			m_ListControl.DeleteAllItems();
			m_mapScenarioData.clear();

			while (true)
			{
				fin.getline(szChar, MAX_STR, L'\t'); // 탭 직전에 자르고
				fin.getline(szFace, MAX_STR, L'\t');
				fin.getline(szSpeech, MAX_STR, L'\n'); // 엔터 직전에 자르고

				if (fin.eof())
					break;

				SCENARIODATA* pScenario = new SCENARIODATA;
				pScenario->strChar = wstring(szChar).c_str();
				pScenario->strFace = wstring(szFace).c_str();
				pScenario->strSpeech = wstring(szSpeech).c_str();

				CString strKey;
				strKey.Format(_T("%d"), m_iCount);
				m_mapScenarioData.insert({ strKey, pScenario });

				// 리스트 박스에 출력한다
				int iIndex = m_ListControl.GetItemCount();
				m_ListControl.InsertItem(iIndex, strKey);
				m_ListControl.SetItem(iIndex, 1, LVIF_TEXT, pScenario->strChar, NULL, NULL, NULL, NULL);
				m_ListControl.SetItem(iIndex, 2, LVIF_TEXT, pScenario->strFace, NULL, NULL, NULL, NULL);
				m_ListControl.SetItem(iIndex, 3, LVIF_TEXT, pScenario->strSpeech, NULL, NULL, NULL, NULL);

				m_iCount++;
			}
			fin.close();
		}
	}

	UpdateData(FALSE);
}


