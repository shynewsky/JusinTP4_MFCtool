#include "stdafx.h"
#include "MyTerrain.h"
#include "Device.h"
#include "TextureMgr.h"
#include "TimeMgr.h"


CMyTerrain::CMyTerrain()
{
}


CMyTerrain::~CMyTerrain()
{
	Release();
}

HRESULT CMyTerrain::Initialize(void)
{
	if (FAILED(Load_Tile(L"../Data/Test.dat")))
	{
		ERR_MSG(L"Tile Data Load Failed");
		return E_FAIL;
	}

	if (FAILED(Ready_Adj()))
	{
		ERR_MSG(L"Ready_Adj Failed");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CMyTerrain::Ready_Adj()
{
	m_vecAdj.resize(m_vecTile.size());

	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			int		iIndex = i * TILEX + j;

			// 네 방향의 인접한 이웃인가?
			// 갈 수 있는 옵션 값을 가진 타일인가?

			//우선은 필요한 상하좌우 우선 구현

			//좌
			if (0 != j)
			{
				if (m_vecTile[iIndex - 1]->byOption != 1)
				{
					m_vecAdj[iIndex].push_back(m_vecTile[iIndex - 1]);
				}
			}

			//우		왜 아웃오브 레인지 뜨는지 의문 29 != 일때 발생
			if (29 != j % (TILEX))
			{
				if (m_vecTile[iIndex + 1]->byOption != 1)
				{
					m_vecAdj[iIndex].push_back(m_vecTile[iIndex + 1]);
				}
			}

			//상
			if (0 != i)
			{
				if (m_vecTile[iIndex - TILEX]->byOption != 1)
				{
					m_vecAdj[iIndex].push_back(m_vecTile[iIndex - TILEX]);
				}
			}

			//하
			if (TILEY - 1 != i)
			{
				//if (iIndex + TILEX >= TILEX * TILEY)
				//{
				//	continue;
				//}
				if (m_vecTile[iIndex + TILEX]->byOption != 1)
				{
					m_vecAdj[iIndex].push_back(m_vecTile[iIndex + TILEX]);
				}
			}


			//// 좌 상단
			//
			//// 맨 윗줄 && 맨 왼쪽 줄
			//if ((0 != i) && (0 != iIndex % (TILEX * 2)))
			//{
			//	// 홀 -> 짝 20 감소, 짝 -> 홀 21 감소
			//	if ((0 != i % 2) && (!m_vecTile[iIndex - TILEX]->byOption))
			//	{
			//		m_vecAdj[iIndex].push_back(m_vecTile[iIndex - TILEX]);
			//	}
			//	else if ((0 == i % 2) && (!m_vecTile[iIndex - (TILEX + 1)]->byOption))
			//	{
			//		m_vecAdj[iIndex].push_back(m_vecTile[iIndex - (TILEX + 1)]);
			//	}
			//
			//}
			//
			//// 우 상단
			//
			//// 맨 윗줄 && 맨 오른쪽 줄
			//if ((0 != i) && ((TILEX * 2 - 1) != iIndex % (TILEX * 2)))
			//{
			//	// 홀 -> 짝 19 감소, 짝 -> 홀 20 감소
			//	if ((0 != i % 2) && (!m_vecTile[iIndex - (TILEX - 1)]->byOption))
			//	{
			//		m_vecAdj[iIndex].push_back(m_vecTile[iIndex - (TILEX - 1)]);
			//	}
			//	else if ((0 == i % 2) && (!m_vecTile[iIndex - TILEX]->byOption))
			//	{
			//		m_vecAdj[iIndex].push_back(m_vecTile[iIndex - TILEX]);
			//	}
			//
			//}
			//
			//// 좌 하단
			//
			//// 맨 아랫줄 && 맨 왼쪽 줄
			//if ((TILEY - 1 != i) && (0 != iIndex % (TILEX * 2)))
			//{
			//	// 홀 -> 짝 20 증가, 짝 -> 홀 19 증가
			//	if ((0 != i % 2) && (!m_vecTile[iIndex + TILEX]->byOption))
			//	{
			//		m_vecAdj[iIndex].push_back(m_vecTile[iIndex + TILEX]);
			//	}
			//	else if ((0 == i % 2) && (!m_vecTile[iIndex + (TILEX - 1)]->byOption))
			//	{
			//		m_vecAdj[iIndex].push_back(m_vecTile[iIndex + (TILEX - 1)]);
			//	}
			//
			//}
			//
			//// 우 하단
			//
			//// 맨 아랫줄 && 맨 오른쪽 줄
			//if ((TILEY - 1 != i) && ((TILEX * 2 - 1) != iIndex % (TILEX * 2)))
			//{
			//	// 홀 -> 짝 21 증가, 짝 -> 홀 20 증가
			//	if ((0 != i % 2) && (!m_vecTile[iIndex + (TILEX + 1)]->byOption))
			//	{
			//		m_vecAdj[iIndex].push_back(m_vecTile[iIndex + (TILEX + 1)]);
			//	}
			//	else if ((0 == i % 2) && (!m_vecTile[iIndex + TILEX]->byOption))
			//	{
			//		m_vecAdj[iIndex].push_back(m_vecTile[iIndex + TILEX]);
			//	}
			//}
		}
	}

	return S_OK;
}

int CMyTerrain::Update(void)
{

	if (0.f > ::Get_Mouse().x)
		m_vScroll.x += 300.f * CTimeMgr::Get_Instance()->Get_TimeDelta();

	if (WINCX < ::Get_Mouse().x)
		m_vScroll.x -= 300.f * CTimeMgr::Get_Instance()->Get_TimeDelta();

	if (0.f > ::Get_Mouse().y)
		m_vScroll.y += 300.f * CTimeMgr::Get_Instance()->Get_TimeDelta();

	if (WINCY < ::Get_Mouse().y)
		m_vScroll.y -= 300.f * CTimeMgr::Get_Instance()->Get_TimeDelta();


	return 0;
}

void CMyTerrain::Late_Update(void)
{
}

void CMyTerrain::Render(void)
{
	D3DXMATRIX		matScale, matRotZ, matTrans;

	TCHAR		szBuf[MIN_STR] = L"";
	int			iIndex(0);

	int		iCullX = int(-m_vScroll.x) / (TILECX * 2);
	int		iCullY = int(-m_vScroll.y) / (TILECY * 2);

	int		iMaxX = WINCX / (TILECX * 2);
	int		iMaxY = WINCY / (TILECY * 2);

	for (int i = iCullY - 1; i < iCullY + iMaxY + 2; ++i)
	{
		for (int j = iCullX - 1; j < iCullX + iMaxX + 2; ++j)
		{
			int		iIndex = i * TILEX + j;

			if (0 > iIndex || (size_t)iIndex >= m_vecTile.size())
				continue;

			D3DXMatrixScaling(&matScale, 2.f, 2.f, 2.f);
			D3DXMatrixTranslation(&matTrans,
				m_vecTile[iIndex]->vPos.x * 2 + m_vScroll.x,	// 0인 경우 x 스크롤 값
				m_vecTile[iIndex]->vPos.y * 2 + m_vScroll.y,	// 1인 경우 y 스크롤 값
				m_vecTile[iIndex]->vPos.z);

			m_tInfo.matWorld = matScale * matTrans;

			CDevice::Get_Instance()->Get_Sprite()->SetTransform(&m_tInfo.matWorld);

			const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Terrain", L"Tile", m_vecTile[iIndex]->byDrawID);

			if (nullptr == pTexInfo)
				return;

			float	fCenterX = pTexInfo->tImgInfo.Width / 2.f;
			float	fCenterY = pTexInfo->tImgInfo.Height / 2.f;

			CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
				nullptr,
				&D3DXVECTOR3(fCenterX, fCenterY, 0.f),
				nullptr,
				D3DCOLOR_ARGB(255, 255, 255, 255));



			D3DXMATRIX matWorld;

			D3DXMatrixIdentity(&matWorld);

			D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
			D3DXMatrixTranslation(&matTrans,
				m_vecTile[iIndex]->vPos.x * 2 + m_vScroll.x,
				m_vecTile[iIndex]->vPos.y * 2 + m_vScroll.y,
				0.f);

			matWorld = matScale * matTrans;

			CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);


			RECT	Rect = { fCenterX - (pTexInfo->tImgInfo.Width)
			, fCenterY - (pTexInfo->tImgInfo.Height)
			, fCenterX + (pTexInfo->tImgInfo.Width)
			, fCenterY + (pTexInfo->tImgInfo.Height) };

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
	}



}

void CMyTerrain::Release(void)
{
	for_each(m_vecTile.begin(), m_vecTile.end(), CDeleteObj());
	m_vecTile.clear();
}

HRESULT CMyTerrain::Load_Tile(const TCHAR* pFilePath)
{
	HANDLE hFile = CreateFile(pFilePath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	Release();

	DWORD		dwByte(0);
	TILE* pTile = nullptr;

	while (true)
	{
		pTile = new TILE;

		ReadFile(hFile, pTile, sizeof(TILE), &dwByte, nullptr);

		if (0 == dwByte)
		{
			Safe_Delete(pTile);
			break;
		}

		m_vecTile.push_back(pTile);
	}

	return S_OK;
}
