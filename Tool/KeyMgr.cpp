#include "stdafx.h"
#include "KeyMgr.h"


CKeyMgr* CKeyMgr::m_pInstance = nullptr;

CKeyMgr::CKeyMgr()
{
	ZeroMemory(m_bKeyState, sizeof(m_bKeyState));		//키 스테이트 초기화
}

CKeyMgr::~CKeyMgr()
{
}

bool CKeyMgr::KeyPressing(int _iKey)
{
	if (GetAsyncKeyState(_iKey) & 0x8000)
		return true;

	return false;
}

bool CKeyMgr::KeyDown(int _iKey)
{
	if ((!m_bKeyState[_iKey]) && (GetAsyncKeyState(_iKey) & 0x8000))
	{
		m_bKeyState[_iKey] = !m_bKeyState[_iKey];

		return true;
	}
	else        //해당 키만 상태 초기화
	{
		m_bKeyState[_iKey] = GetAsyncKeyState(_iKey);
	}
	return false;
}

bool CKeyMgr::KeyUp(int _iKey)
{
	// 이전에 눌린 적이 있고, 현재는 눌리지 않은 상태
	if ((m_bKeyState[_iKey]) && !(GetAsyncKeyState(_iKey) & 0x8000))
	{
		m_bKeyState[_iKey] = false;
		return true;
	}
	else        //해당 키만 상태 초기화
	{
		m_bKeyState[_iKey] = GetAsyncKeyState(_iKey);
	}
	return false;
}
