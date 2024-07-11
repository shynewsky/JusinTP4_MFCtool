#pragma once
#include "Define.h"

class CKeyMgr
{
private:
	CKeyMgr();
	~CKeyMgr();

public:
	
	static CKeyMgr* GetInstance()
	{
		if (m_pInstance == nullptr)
		{
			m_pInstance = new CKeyMgr;
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

	bool KeyPressing(int _iKey);
	bool KeyDown(int _iKey);
	bool KeyUp(int _iKey);

private:
	static CKeyMgr* m_pInstance;
	bool m_bKeyState[VK_MAX];
};

