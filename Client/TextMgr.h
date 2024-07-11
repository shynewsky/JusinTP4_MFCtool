#pragma once
#include "Define.h"

#include "Include.h"

class CTextMgr
{
private:
	CTextMgr();
	~CTextMgr();

public:

	static CTextMgr* GetInstance()
	{
		if (m_pInstance == nullptr)
		{
			m_pInstance = new CTextMgr;
		}
		return m_pInstance;
	}

	static void Destroy()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static CTextMgr* m_pInstance;

public:
	void	Load_Scenario();	// 대화 데이터베이스 불러오기
	bool	Next_Speech(SCENARIODATA& _struct); // 대화 한줄만 내보내기
	void	Set_Count(int _i) { m_iCount += _i; }

public:
	int		m_iKey;
	int		m_iCount;
	map<wstring, SCENARIODATA*> m_mapScenarioData;

};

