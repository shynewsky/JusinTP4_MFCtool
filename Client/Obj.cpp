#include "stdafx.h"
#include "Obj.h"
#include "TimeMgr.h"
//#include "TimeMgr.h"

D3DXVECTOR3 CObj::m_vScroll{};

CObj::CObj()
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	ZeroMemory(&m_tFrame, sizeof(FRAME));
	D3DXMatrixIdentity(&m_tInfo.matWorld);

	m_tInfo.vLook = { 1.f, 0.f, 0.f };
}

CObj::~CObj()
{
}

void CObj::Move_Frame()
{
	m_tFrame.fFrame += m_tFrame.fMax * CTimeMgr::Get_Instance()->Get_TimeDelta();

	if (m_tFrame.fFrame >= m_tFrame.fMax + 1)
		m_tFrame.fFrame = 0.f;
}


