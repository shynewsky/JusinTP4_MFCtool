#include "stdafx.h"
#include "Player.h"
#include "Device.h"
#include "TextureMgr.h"
#include "KeyMgr.h"
#include "AstarMgr.h"
#include "TimeMgr.h"
#include "ObjMgr.h"
#include "MyTerrain.h"

#include "TextMgr.h"
#include "Obj.h"
#include "Include.h"

CPlayer::CPlayer()
	: m_iTileIndex(0), m_ePlayerDir(MD_DOWN), m_vTargetTile({ 0, 0, 0 })
	, m_bMoveDirNow(false), m_bMoveAstarNow(false)
	, m_bReservMove(false), m_eMoveReserv(MD_DOWN), m_iTargetTileIndex(31)
	, m_bScrollCtr(true), m_bScenario(false), m_bDrawScenario(false)
{
	ZeroMemory(&m_tSpeech, sizeof(m_tSpeech));
	m_tSpeech.strChar = L" ";
	m_tSpeech.strFace = L" ";
	m_tSpeech.strSpeech = L" ";
}

CPlayer::~CPlayer()
{
	Release();
}

HRESULT CPlayer::Initialize(void)
{
	m_tInfo.vPos = { 72.f, 72.f,0.f };
	m_vOriginSpot = m_tInfo.vPos;
	m_wstrObjKey = L"Player";
	m_wstrStateKey = L"Move";
	m_iTileIndex = 31;
	m_tFrame = { 0.f, 2.f };

	// 시나리오 불러오기
	CTextMgr::GetInstance()->Load_Scenario();

	return S_OK;
}

int CPlayer::Update(void)
{
	D3DXMATRIX		matScale, matRotZ, matTrans;

	D3DXMatrixScaling(&matScale, 2.f, 2.f, 2.f);
	D3DXMatrixTranslation(&matTrans,
		m_tInfo.vPos.x + m_vScroll.x,	// 0인 경우 x 스크롤 값
		m_tInfo.vPos.y - 20 + m_vScroll.y,	// 1인 경우 y 스크롤 값
		m_tInfo.vPos.z);

	m_tInfo.matWorld = matScale * matTrans;

	Key_Input();

	if (m_bReservMove && !m_bMoveAstarNow)
	{
		Set_Move_Dir(m_eMoveReserv);
		m_bMoveDirNow = true;
		m_bMoveAstarNow = false;
		m_bReservMove = false;
	}
	else if (m_bMoveDirNow && !m_bMoveAstarNow)
	{
		Move_Dir();
	}
	else if (m_bMoveAstarNow && !m_bMoveDirNow)
	{
		Move_AStar();
	}

	return 0;
}

void CPlayer::Late_Update(void)
{
	if (m_bScrollCtr)
	{
		m_vScroll.x = -(m_tInfo.vPos.x - m_vOriginSpot.x - 7 * 48);
		m_vScroll.y = -(m_tInfo.vPos.y - m_vOriginSpot.y - 6 * 48);
	}



	__super::Move_Frame();
}

void CPlayer::Render(void)
{
	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&m_tInfo.matWorld);

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(m_wstrObjKey.c_str(), m_wstrStateKey.c_str(), (int)m_tFrame.fFrame);

	if (nullptr == pTexInfo)
		return;

	float	fCenterX = pTexInfo->tImgInfo.Width / 2.f;
	float	fCenterY = pTexInfo->tImgInfo.Height / 2.f;

	CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f),
		nullptr,
		D3DCOLOR_ARGB(255, 255, 255, 255));


	if (m_bDrawScenario)
	{
#pragma region 배경

		D3DXMATRIX		matScale, matRotZ, matTrans;

		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matTrans,
			20,	// 0인 경우 x 스크롤 값
			430,	// 1인 경우 y 스크롤 값
			0);

		m_tInfo.matWorld = matScale * matTrans;

		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&m_tInfo.matWorld);

		CTextureMgr::Get_Instance()->ReadImgPath(L"../Texture/Stage/UI/Scenario/UiWindow0.png");

		const TEXINFO* pTexInfoUI = CTextureMgr::Get_Instance()->Get_Texture(L"UI", L"Scenario", 0);

		CDevice::Get_Instance()->Get_Sprite()->Draw(
			pTexInfoUI->pTexture,
			nullptr,
			nullptr,
			nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255));

#pragma endregion

#pragma region 글자

		D3DXMatrixTranslation(&matTrans,
			400,	// 0인 경우 x 스크롤 값
			500,	// 1인 경우 y 스크롤 값
			0);

		m_tInfo.matWorld = matScale * matTrans;

		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&m_tInfo.matWorld);

		CDevice::Get_Instance()->Get_Font()->DrawTextW(
			CDevice::Get_Instance()->Get_Sprite(),
			m_tSpeech.strSpeech.c_str(),
			m_tSpeech.strSpeech.length() - 1,
			nullptr,			// 출력할 위치를 가진 렉트의 주소
			DT_CENTER,			// 정렬 옵션
			D3DCOLOR_ARGB(255, 255, 255, 255));

#pragma endregion

#pragma region 이미지

		D3DXMatrixScaling(&matScale, 3.f, 3.f, 1.f);
		D3DXMatrixTranslation(&matTrans,
			50,	// 0인 경우 x 스크롤 값
			450,	// 1인 경우 y 스크롤 값
			0);

		m_tInfo.matWorld = matScale * matTrans;

		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&m_tInfo.matWorld);

		int iFace(0);

		if (m_tSpeech.strFace == L"normal")
		{
			iFace = 0;
		}
		else if (m_tSpeech.strFace == L"smile")
		{
			iFace = 1;
		}
		else if (m_tSpeech.strFace == L"angry")
		{
			iFace = 2;
		}
		else if (m_tSpeech.strFace == L"happy")
		{
			iFace = 3;
		}

		const TEXINFO* pTexInfoIMG = CTextureMgr::Get_Instance()->Get_Texture(L"Player", L"Face", iFace);

		CDevice::Get_Instance()->Get_Sprite()->Draw(
			pTexInfoIMG->pTexture,
			nullptr,
			nullptr,
			nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255));

#pragma endregion

	}

}

void CPlayer::Release(void)
{
}

void CPlayer::Key_Input()
{
	if (CKeyMgr::GetInstance()->KeyDown(VK_LBUTTON))
	{
		if (!m_bMoveAstarNow && !m_bMoveDirNow)
		{
			m_bMoveAstarNow = true;
			CAstarMgr::Get_Instance()->Start_Astar(m_tInfo.vPos, ::Get_Mouse() - CObj::m_vScroll);
		}
	}

	if (CKeyMgr::GetInstance()->KeyPressing('W'))
	{
		if (m_bMoveAstarNow)
		{
			m_bReservMove = true;
			m_eMoveReserv = MD_UP;
		}
		else if (!m_bMoveDirNow)
		{
			Set_Move_Dir(MD_UP);
		}
	}
	if (CKeyMgr::GetInstance()->KeyPressing('S'))
	{
		if (m_bMoveAstarNow)
		{
			m_bReservMove = true;
			m_eMoveReserv = MD_DOWN;
		}
		else if (!m_bMoveDirNow)
		{
			Set_Move_Dir(MD_DOWN);
		}
	}
	if (CKeyMgr::GetInstance()->KeyPressing('A'))
	{
		if (m_bMoveAstarNow)
		{
			m_bReservMove = true;
			m_eMoveReserv = MD_LEFT;
		}
		else if (!m_bMoveDirNow)
		{
			Set_Move_Dir(MD_LEFT);
		}
	}
	if (CKeyMgr::GetInstance()->KeyPressing('D'))
	{
		if (m_bMoveAstarNow)
		{
			m_bReservMove = true;
			m_eMoveReserv = MD_RIGHT;
		}
		else if (!m_bMoveDirNow)
		{
			Set_Move_Dir(MD_RIGHT);
		}
	}

	if (CKeyMgr::GetInstance()->KeyPressing(VK_RBUTTON))
	{
		m_bScrollCtr = false;
	}

	if (CKeyMgr::GetInstance()->KeyUp(VK_RBUTTON))
	{
		m_bScrollCtr = true;
	}

	if (CKeyMgr::GetInstance()->KeyDown(VK_SPACE))
	{
		m_bScenario = true;
		Read_Scenario();
	}
}

void CPlayer::Move_AStar()
{
	list<TILE*>& BestList = CAstarMgr::Get_Instance()->Get_BestList();

	if (!BestList.empty())
	{
		D3DXVECTOR3	vDir = BestList.front()->vPos * 2 - m_tInfo.vPos;

		float	fDistance = D3DXVec3Length(&vDir);
		D3DXVec3Normalize(&vDir, &vDir);
		m_tInfo.vPos += vDir * 300.f * CTimeMgr::Get_Instance()->Get_TimeDelta();

		if (3.f >= fDistance)
		{
			if (m_bReservMove)
			{
				m_bMoveAstarNow = false;
			}
			m_iTileIndex = BestList.front()->iIndex;
			BestList.pop_front();
		}
	}
	else
	{
		m_bMoveAstarNow = false;
	}

}

void CPlayer::Set_Move_Dir(MOVEDIR _eDir)
{
	CObj* pMyTerrain = CObjMgr::Get_Instance()->Get_Terrain();
	vector<TILE*>& vecTile = dynamic_cast<CMyTerrain*>(pMyTerrain)->Get_VecTile();
	vector<list<TILE*>>& vecAdj = dynamic_cast<CMyTerrain*>(pMyTerrain)->Get_VecAdj();
	int iTemp(0);
	switch (_eDir)
	{
	case MD_UP:
		iTemp = -TILEX;
		break;
	case MD_DOWN:
		iTemp = TILEX;
		break;
	case MD_LEFT:
		iTemp = -1;
		break;
	case MD_RIGHT:
		iTemp = 1;
		break;
	}


	for (auto& Iter : vecAdj[m_iTileIndex])
	{
		if (Iter->iIndex == m_iTileIndex + iTemp)
		{
			m_vTargetTile = Iter->vPos;
			m_iTargetTileIndex = Iter->iIndex;
			m_bMoveDirNow = true;
			return;
		}
	}

	m_ePlayerDir = _eDir;
	//방향전환 이동은 X 
}

void CPlayer::Move_Dir()
{
	D3DXVECTOR3	vDir = m_vTargetTile * 2 - m_tInfo.vPos;

	float	fDistance = D3DXVec3Length(&vDir);
	D3DXVec3Normalize(&vDir, &vDir);
	m_tInfo.vPos += vDir * 300.f * CTimeMgr::Get_Instance()->Get_TimeDelta();

	if (3.f >= fDistance)
	{
		m_iTileIndex = m_iTargetTileIndex;
		m_bMoveDirNow = false;
	}
}

void CPlayer::Read_Scenario()
{
	if (m_bScenario)
	{
		// 다음 대사 있는지 확인하기
		bool bScenario = CTextMgr::GetInstance()->Next_Speech(m_tSpeech);

		if (bScenario)
		{
			// 대사창 띄우고, 아이콘, 대사 띄우기
			m_bDrawScenario = true;
			m_bScenario = false;
		}
		else
		{
			m_bDrawScenario = false;
			m_bScenario = false;
		}
	}
}
