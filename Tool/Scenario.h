#pragma once
#include "afxdialogex.h"

#include "Include.h"

// CScenario 대화 상자

class CScenario : public CDialog
{
	DECLARE_DYNAMIC(CScenario)

public:
	CScenario(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CScenario();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SCENARIO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();

	afx_msg void OnAdd();
	afx_msg void OnDelete();
	afx_msg void OnFix();
	afx_msg void OnSave();
	afx_msg void OnLoad();
	afx_msg void OnListControl(NMHDR* pNMHDR, LRESULT* pResult);

public:
	// value
	CString m_strChar;
	CString m_strFace;
	CString m_strSpeech;
	CString m_strText;


public:
	// control
	CListCtrl m_ListControl;
	CStatic m_imgPic;

public:
	map<CString, SCENARIODATA*> m_mapScenarioData;

public:
	int		m_iCount = 0; // 추가할때만 커지는 숫자 --> 맵 컨테이너 키로 사용됨
};
