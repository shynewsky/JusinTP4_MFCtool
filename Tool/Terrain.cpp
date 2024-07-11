#include "stdafx.h"
#include "Terrain.h"
#include "TextureMgr.h"
#include "Device.h"
#include "ToolView.h"
#include "Include.h"

CTerrain::CTerrain() : m_pMainView(NULL), m_fScale(1.f)
{
	m_vecTile.reserve(TILEX * TILEY);
}

CTerrain::~CTerrain()
{
	Release();
}

void CTerrain::Initialize()
{
	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Stage/Terrain/Tile/%d.png", TEX_MULTI, L"Terrain", L"Tile", 47)))
	{
		AfxMessageBox(L"Tile Image Insert Failed");
		return;
	}

	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			TILE* pTile = new TILE;

			float fX = TILECX * j + (TILECX / 2);
			float fY = TILECY * i + (TILECY / 2);

			pTile->vPos = { fX, fY, 0.f };
			pTile->vSize = { TILECX, TILECY, 0.f };
			pTile->byOption = 0;
			pTile->byDrawID = 0;

			pTile->iIndex = i * TILEX + j;
			pTile->iParentIdx = 0;

			m_vecTile.push_back(pTile);
		}
	}


}

void CTerrain::Update()
{

}

void CTerrain::Render()
{
	D3DXMATRIX		matWorld, matScale, matTrans;

	TCHAR		szBuf[MIN_STR] = L"";
	int			iIndex(0);

	RECT		rc{};


	for (auto& pTile : m_vecTile)
	{
		D3DXMatrixIdentity(&matWorld);

		D3DXMatrixScaling(&matScale, m_fScale * 2, m_fScale * 2, m_fScale);
		D3DXMatrixTranslation(&matTrans,
			pTile->vPos.x * m_fScale * 2 - m_pMainView->GetScrollPos(0),
			pTile->vPos.y * m_fScale * 2 - m_pMainView->GetScrollPos(1),
			0.f);

		matWorld = matScale * matTrans;

		GetClientRect(m_pMainView->m_hWnd, &rc);

		float	fRatioX = WINCX / float(rc.right - rc.left);
		float	fRatioY = WINCY / float(rc.bottom - rc.top);

		Set_Ratio(&matWorld, fRatioX, fRatioY);

		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

		const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Terrain", L"Tile", pTile->byDrawID);

		float	fCenterX = pTexInfo->tImgInfo.Width / 2.f;
		float	fCenterY = pTexInfo->tImgInfo.Height / 2.f;

		// Draw : 스크린 공간에 2d 스프라이트 이미지를 출력
		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
			nullptr, // 출력할 이미지 영역에 대한 rect 구조체 주소(nullptr인 경우 이미지의 0,0 기준으로 출력)
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), // 출력할 이미지의 중심 위치에 대한 vec3 구조체 주소(nullptr인 경우 0,0으로 중점을 조정)
			nullptr, // 출력할 위치 좌표에 대한 vec3 구조체 주소(nullptr인 경우 스크린 상의 0,0에 출력)
			D3DCOLOR_ARGB(255, 255, 255, 255));



		D3DXMatrixIdentity(&matWorld);

		D3DXMatrixScaling(&matScale, m_fScale, m_fScale, m_fScale);
		D3DXMatrixTranslation(&matTrans,
			pTile->vPos.x * m_fScale * 2 - m_pMainView->GetScrollPos(0),
			pTile->vPos.y * m_fScale * 2 - m_pMainView->GetScrollPos(1),
			0.f);

		matWorld = matScale * matTrans;

		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);


		RECT	Rect = { fCenterX * m_fScale - (pTexInfo->tImgInfo.Width / 2 * m_fScale * 2)
			, fCenterY * m_fScale - (pTexInfo->tImgInfo.Height / 2 * m_fScale * 2)
			, fCenterX * m_fScale + (pTexInfo->tImgInfo.Width / 2 * m_fScale * 2)
			, fCenterY * m_fScale + (pTexInfo->tImgInfo.Height / 2 * m_fScale * 2) };

		// 타일 인덱스 출력
		swprintf_s(szBuf, L"%d", iIndex);
		CDevice::Get_Instance()->Get_Font()->DrawTextW(CDevice::Get_Instance()->Get_Sprite(),
			szBuf,
			lstrlen(szBuf),
			&Rect,	// 출력할 위치를 가진 렉트의 주소
			DT_LEFT,			// 정렬 옵션
			D3DCOLOR_ARGB(255, 255, 255, 255));



		++iIndex;
	}

	//피킹 타일 표기
	int iPickIndex = Get_TileIdx(D3DXVECTOR3(Get_Mouse().x + GetScrollPos(g_hWnd, 0),
		(float)Get_Mouse().y + GetScrollPos(g_hWnd, 1),
		0.f));


	if (iPickIndex != -1)
	{
		D3DXMatrixIdentity(&matWorld);

		D3DXMatrixScaling(&matScale, m_fScale * 2, m_fScale * 2, m_fScale);
		D3DXMatrixTranslation(&matTrans,
			m_vecTile[iPickIndex]->vPos.x * m_fScale * 2 - m_pMainView->GetScrollPos(0),
			m_vecTile[iPickIndex]->vPos.y * m_fScale * 2 - m_pMainView->GetScrollPos(1),
			0.f);

		matWorld = matScale * matTrans;

		GetClientRect(m_pMainView->m_hWnd, &rc);

		float	fRatioX = WINCX / float(rc.right - rc.left);
		float	fRatioY = WINCY / float(rc.bottom - rc.top);

		Set_Ratio(&matWorld, fRatioX, fRatioY);

		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

		const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Terrain", L"Tile", 46);

		float	fCenterX = pTexInfo->tImgInfo.Width / 2.f;
		float	fCenterY = pTexInfo->tImgInfo.Height / 2.f;

		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
			nullptr, // 출력할 이미지 영역에 대한 rect 구조체 주소(nullptr인 경우 이미지의 0,0 기준으로 출력)
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), // 출력할 이미지의 중심 위치에 대한 vec3 구조체 주소(nullptr인 경우 0,0으로 중점을 조정)
			nullptr, // 출력할 위치 좌표에 대한 vec3 구조체 주소(nullptr인 경우 스크린 상의 0,0에 출력)
			D3DCOLOR_ARGB(255, 255, 255, 255));

	}

}

void CTerrain::Mini_Render()
{
	D3DXMATRIX		matWorld, matScale, matTrans;

	for (auto& pTile : m_vecTile)
	{
		D3DXMatrixIdentity(&matWorld);

		D3DXMatrixScaling(&matScale, 2.f, 2.f, 1.f);
		D3DXMatrixTranslation(&matTrans,
			pTile->vPos.x,
			pTile->vPos.y,
			0.f);

		matWorld = matScale * matTrans;

		Set_Ratio(&matWorld, 0.3f, 0.3f);

		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

		const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Terrain", L"Tile", pTile->byDrawID);

		float	fCenterX = pTexInfo->tImgInfo.Width / 2.f;
		float	fCenterY = pTexInfo->tImgInfo.Height / 2.f;

		// Draw : 스크린 공간에 2d 스프라이트 이미지를 출력
		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
			nullptr, // 출력할 이미지 영역에 대한 rect 구조체 주소(nullptr인 경우 이미지의 0,0 기준으로 출력)
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), // 출력할 이미지의 중심 위치에 대한 vec3 구조체 주소(nullptr인 경우 0,0으로 중점을 조정)
			nullptr, // 출력할 위치 좌표에 대한 vec3 구조체 주소(nullptr인 경우 스크린 상의 0,0에 출력)
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void CTerrain::Release()
{
	for_each(m_vecTile.begin(), m_vecTile.end(), [](auto& pTile)
		{
			if (pTile)
			{
				delete pTile;
				pTile = nullptr;
			}
		});

	m_vecTile.clear();
	m_vecTile.shrink_to_fit();

}

int CTerrain::Get_TileIdx(const D3DXVECTOR3& vPos)
{
	for (size_t index = 0; index < m_vecTile.size(); ++index)
	{
		if (Picking(vPos, index))
		{
			return index;
		}
	}

	return -1;
}

void CTerrain::Tile_Change(const D3DXVECTOR3& vPos, const int& iDrawID, const int& iOption)
{
	int	iIndex = Get_TileIdx(vPos);

	if (-1 == iIndex)
		return;

	m_vecTile[iIndex]->byDrawID = iDrawID;
	m_vecTile[iIndex]->byOption = iOption;
}

bool CTerrain::Picking(const D3DXVECTOR3& vPos, const int& iIndex)
{
	if (vPos.x > int(m_vecTile[iIndex]->vPos.x * m_fScale * 2 - (TILECX * 0.5f * m_fScale * 2))
		&& vPos.x <= int(m_vecTile[iIndex]->vPos.x * m_fScale * 2 + (TILECX * 0.5f * m_fScale * 2))
		&& vPos.y > int(m_vecTile[iIndex]->vPos.y * m_fScale * 2 - (TILECY * 0.5f * m_fScale * 2))
		&& vPos.y <= int(m_vecTile[iIndex]->vPos.y * m_fScale * 2 + (TILECY * 0.5f * m_fScale * 2)))
	{
		return TRUE;
	}
	return FALSE;
}

bool CTerrain::Picking_Dot(const D3DXVECTOR3& vPos, const int& iIndex)
{
	return NULL;
}

void CTerrain::Set_Ratio(D3DXMATRIX* pOut, float fRatioX, float fRatioY)
{
	pOut->_11 *= fRatioX;
	pOut->_21 *= fRatioX;
	pOut->_31 *= fRatioX;
	pOut->_41 *= fRatioX;

	pOut->_12 *= fRatioY;
	pOut->_22 *= fRatioY;
	pOut->_32 *= fRatioY;
	pOut->_42 *= fRatioY;

}

void CTerrain::Add_Scale(float _fScale)
{
	if (_fScale < 0)
	{
		if (m_fScale > 0.2)
		{
			m_fScale += _fScale;
		}
	}
	else
	{
		if (m_fScale < 2)
		{
			m_fScale += _fScale;
		}
	}
}
