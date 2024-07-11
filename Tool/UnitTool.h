#pragma once
#include "afxwin.h"

#include "Include.h"

// CUnitTool 대화 상자입니다.

class CUnitTool : public CDialog
{
	DECLARE_DYNAMIC(CUnitTool)

public:
	CUnitTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CUnitTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UNITTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnAdd();
	afx_msg void OnListBox();
	afx_msg void OnDestroy();
	afx_msg void OnDelete();
	afx_msg void OnSave();
	afx_msg void OnLoad();
	afx_msg void OnFind();

	virtual BOOL OnInitDialog();

public:
	CString m_strTest;
	CString m_strCopy;

public:	// value
	CString m_strName;
	int		m_iAttack;
	int		m_iHp;
	CString m_strFindName;

public:	// control
	CListBox	m_ListBox;
	CButton		m_Radio[3];
	CButton		m_Check[3];
	CButton		m_Bitmap;

public:
	map<CString, UNITDATA*>		m_mapUnitData;

};

// 탐색 : 문자열을 입력했을 경우, 해당 문자열과 일치하는 리스트 박스 목록이 선택되며 유닛 데이터 반영
// 삭제 : 리스트 박스에 있는 문자열을 마우스를 클릭하여 선택하고 그 이후, 삭제버튼을 눌러 데이터를 삭제
// 저장, 불러오기 : 버튼을 누를 경우, 리스트 박스에 있는 데이터를 저장하거나 또는 불러오기 버튼을 누를 경우 데이터를 읽어들여 리스트 박스 목록에 데이터를 불러와줌

