#include "stdafx.h"
#include "TextMgr.h"

#include "KeyMgr.h"

CTextMgr* CTextMgr::m_pInstance = nullptr;

CTextMgr::CTextMgr()
	: m_iKey(0), m_iCount(0)
{
}

CTextMgr::~CTextMgr()
{
}

void CTextMgr::Load_Scenario()
{
	wifstream fin;
	fin.open(L"../Data/ScenarioDB.txt", ios::in);

	if (!fin.fail())
	{
		TCHAR	szChar[MAX_STR] = L"";
		TCHAR	szFace[MAX_STR] = L"";
		TCHAR	szSpeech[MAX_STR] = L"";

		m_iKey = 0;

		while (true)
		{
			fin.getline(szChar, MAX_STR, L'\t'); // 탭 직전에 자르고
			fin.getline(szFace, MAX_STR, L'\t');
			fin.getline(szSpeech, MAX_STR, L'\n'); // 엔터 직전에 자르고

			if (fin.eof())
				break;

			SCENARIODATA* pScenario = new SCENARIODATA;
			pScenario->strChar = wstring(szChar);
			pScenario->strFace = wstring(szFace);
			pScenario->strSpeech = wstring(szSpeech);

			wstring wstrKey(L"%d", m_iKey);
			m_mapScenarioData.insert({ wstrKey, pScenario });

			m_iKey++;
		}
		fin.close();
	}
}

bool CTextMgr::Next_Speech(SCENARIODATA& _struct)
{
	wstring wstrKey(L"%d", m_iCount);

	auto& iter = m_mapScenarioData.find(wstrKey);

	// 다음 대사가 없는 경우

	if (iter == m_mapScenarioData.end())
		return false;

	// 다음 대사가 있는 경우

	

	_struct.strChar = iter->second->strChar;
	_struct.strFace = iter->second->strFace;
	_struct.strSpeech = iter->second->strSpeech;

	m_iCount++;

	return true;
}
