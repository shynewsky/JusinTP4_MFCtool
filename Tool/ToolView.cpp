
// ToolView.cpp : CToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "ToolDoc.h"
#include "ToolView.h"
#include "Device.h"
#include "MainFrm.h"
#include "KeyMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND	g_hWnd;


// CToolView

IMPLEMENT_DYNCREATE(CToolView, CScrollView)

BEGIN_MESSAGE_MAP(CToolView, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CToolView 생성/소멸

CToolView::CToolView() : m_StartScroll({ 0, 0 }), m_vDragPoint(0, 0, 0) //: m_pSingle(nullptr)
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CToolView::~CToolView()
{
}

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}

void CToolView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	// SetScrollSizes : 스크롤 바의 사이즈를 지정하는 cscrollview의 멤버 함수
	// MM_TEXT : 픽셀 다위로 조정하겠다는 옵션
	// CSize : MFC에서 제공하는 사이즈를 저장하기 위한 용도의 클래스 자료형

	SetScrollSizes(MM_TEXT, CSize(TILEX * TILECX * 2, TILEY * TILECY * 2));

	// AfxGetMainWnd : 현재의 메인 윈도우를 반환하는 전역 함수		
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();

	RECT	rcWnd{};

	// GetWindowRect : 현재 윈도우의 RECT 정보를 얻어오는 함수
	pMainFrm->GetWindowRect(&rcWnd);

	// SetRect : 지정한 매개 변수대로 RECT 정보를 기록해주는 함수
	SetRect(&rcWnd, 0, 0, rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top);

	RECT		rcMainView{};
	//GetClientRect : 현재 view 창의 RECT 정보를 얻어오는 함수
	GetClientRect(&rcMainView);

	float	fRowFrm = float(rcWnd.right - rcMainView.right);
	float	fColFrm = float(rcWnd.bottom - rcMainView.bottom);

	// SetWindowPos : 매개 변수대로 새롭게 윈도우 위치와 크기를 조정하는 함수
	// SetWindowPos(배치할 윈도우의 z순서에 대한 포인터, x좌표, y좌표, 가로 크기, 세로 크기, 배치할 윈도우의 크기 및 위치 지정 옵션)
	// nullptr : 순서 변경하지 않음
	// SWP_NOZORDER : 현재 순서를 유지하겠음
	pMainFrm->SetWindowPos(nullptr, 0, 0, int(WINCX + fRowFrm), int(WINCY + fColFrm), SWP_NOZORDER);


	g_hWnd = m_hWnd;

	if (FAILED(CDevice::Get_Instance()->Init_Device()))
	{
		AfxMessageBox(L"Init_Device Failed");
		return;
	}

	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Cube.png", TEX_SINGLE, L"Cube")))
	{
		AfxMessageBox(L"Cube Image Insert Failed");
		return;
	}

	m_pTerrain = new CTerrain;
	m_pTerrain->Initialize();
	m_pTerrain->Set_MainView(this);

}
void CToolView::OnDraw(CDC* /*pDC*/)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CDevice::Get_Instance()->Render_Begin();

	m_pTerrain->Render();

	CDevice::Get_Instance()->Render_End();
}


void CToolView::OnDestroy()
{
	CScrollView::OnDestroy();

	Safe_Delete(m_pTerrain);

	CTextureMgr::Destroy_Instance();
	CDevice::Destroy_Instance();
}

void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CScrollView::OnLButtonDown(nFlags, point);

	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMiniView* pMiniView = dynamic_cast<CMiniView*>(pMainFrm->m_SecondSplitter.GetPane(0, 0));
	CMyForm* pMyForm = dynamic_cast<CMyForm*>(pMainFrm->m_SecondSplitter.GetPane(1, 0));
	CMapTool* pMapTool = &(pMyForm->m_MapTool);

	m_pTerrain->Tile_Change(D3DXVECTOR3((float)point.x + GetScrollPos(0),
		(float)point.y + GetScrollPos(1),
		0.f),
		pMapTool->m_iDrawID,
		pMapTool->m_iOption);


	pMiniView->Invalidate(FALSE);

	// 피킹 처리 함수

	// Invalidate : 호출 시 윈도우에 WM_PAINT와 WM_ERASEBKGND 메세지를 발생시킴
	// FALSE : WM_PAINT만 발생
	// TRUE : WM_PAINT, WM_ERASEBKGND 동시에 발생

	Invalidate(FALSE);
}

void CToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CScrollView::OnMouseMove(nFlags, point);

	Key_Input();

	if (GetAsyncKeyState(VK_LBUTTON))
	{
		CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
		CMiniView* pMiniView = dynamic_cast<CMiniView*>(pMainFrm->m_SecondSplitter.GetPane(0, 0));
		CMyForm* pMyForm = dynamic_cast<CMyForm*>(pMainFrm->m_SecondSplitter.GetPane(1, 0));
		CMapTool* pMapTool = &(pMyForm->m_MapTool);

		m_pTerrain->Tile_Change(D3DXVECTOR3((float)point.x + GetScrollPos(0),
			(float)point.y + GetScrollPos(1),
			0.f),
			pMapTool->m_iDrawID,
			pMapTool->m_iOption);
		Invalidate(FALSE);


		pMiniView->Invalidate(FALSE);
	}
}

void CToolView::Key_Input()
{
	CDevice::Get_Instance()->Render_Begin();


	if (CKeyMgr::GetInstance()->KeyDown(VK_RBUTTON))
	{
		m_vDragPoint = Get_Mouse();
		m_StartScroll = { GetScrollPos(0), GetScrollPos(1) };
	}

	if (CKeyMgr::GetInstance()->KeyPressing(VK_RBUTTON))
	{
		//x,y 판단 / 위치 / 업데이트 여부
		SetScrollPos(0, m_StartScroll.x + m_vDragPoint.x - Get_Mouse().x, true);
		SetScrollPos(1, m_StartScroll.y + m_vDragPoint.y - Get_Mouse().y, true);

	}

	if (CKeyMgr::GetInstance()->KeyDown(VK_PRIOR))
	{
		m_pTerrain->Add_Scale(0.1f);
		float fScale = m_pTerrain->Get_Scale();
		SetScrollSizes(MM_TEXT, CSize(TILEX * TILECX * fScale, TILEY * TILECY * fScale / 2));
	}
	if (CKeyMgr::GetInstance()->KeyDown(VK_NEXT))
	{
		m_pTerrain->Add_Scale(-0.1f);
		float fScale = m_pTerrain->Get_Scale();
		SetScrollSizes(MM_TEXT, CSize(TILEX * TILECX * fScale, TILEY * TILECY * fScale / 2));
	}

	m_pTerrain->Render();

	CDevice::Get_Instance()->Render_End();

}


#pragma region 생략

// CToolView 인쇄

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CToolView 진단

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView 메시지 처리기

#pragma endregion 생략