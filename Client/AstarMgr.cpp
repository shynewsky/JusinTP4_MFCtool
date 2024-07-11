#include "stdafx.h"
#include "AstarMgr.h"
#include "ObjMgr.h"
#include "MyTerrain.h"

IMPLEMENT_SINGLETON(CAstarMgr)

CAstarMgr::CAstarMgr() : m_iStartIdx(0)
{
}

CAstarMgr::~CAstarMgr()
{
	Release();
}

bool CAstarMgr::Make_Route(int iStartIdx, int iGoalIdx)
{
	CObj*					pMyTerrain = CObjMgr::Get_Instance()->Get_Terrain();
	vector<TILE*>&			vecTile = dynamic_cast<CMyTerrain*>(pMyTerrain)->Get_VecTile();
	vector<list<TILE*>>&	vecAdj = dynamic_cast<CMyTerrain*>(pMyTerrain)->Get_VecAdj();

	if (!m_OpenList.empty())
		m_OpenList.pop_front();

	m_CloseList.push_back(iStartIdx);

	for (auto& pTile : vecAdj[iStartIdx])
	{
		if (iGoalIdx == pTile->iIndex)
		{
			pTile->iParentIdx = iStartIdx;
			return true;
		}

		if (false == Check_Close(pTile->iIndex) &&
			false == Check_Open(pTile->iIndex))
		{
			pTile->iParentIdx = iStartIdx;
			m_OpenList.push_back(pTile->iIndex);
		}		
	}

	if (m_OpenList.empty())
		return false;

	int		iOriginStart = m_iStartIdx;

	m_OpenList.sort([&vecTile, &iGoalIdx, &iOriginStart](int Dst, int Src) -> bool
	{
		D3DXVECTOR3	vPCost1 = vecTile[iOriginStart]->vPos - vecTile[Dst]->vPos;
		D3DXVECTOR3	vPCost2 = vecTile[iOriginStart]->vPos - vecTile[Src]->vPos;

		D3DXVECTOR3	vGCost1 = vecTile[iGoalIdx]->vPos - vecTile[Dst]->vPos;
		D3DXVECTOR3	vGCost2 = vecTile[iGoalIdx]->vPos - vecTile[Src]->vPos;

		float	fCost1 = D3DXVec3Length(&vPCost1) + D3DXVec3Length(&vGCost1);
		float	fCost2 = D3DXVec3Length(&vPCost2) + D3DXVec3Length(&vGCost2);

		return fCost1 < fCost2;
	});

	return Make_Route(m_OpenList.front(), iGoalIdx);
}

void CAstarMgr::Make_BestList(int iStartIdx, int iGoalIdx)
{
	CObj*					pMyTerrain = CObjMgr::Get_Instance()->Get_Terrain();
	vector<TILE*>&			vecTile = dynamic_cast<CMyTerrain*>(pMyTerrain)->Get_VecTile();

	m_BestList.push_front(vecTile[iGoalIdx]);

	int	iRouteIdx = vecTile[iGoalIdx]->iParentIdx;

	while (true)
	{
		if (iRouteIdx == iStartIdx)
			break;

		m_BestList.push_front(vecTile[iRouteIdx]);
		iRouteIdx = vecTile[iRouteIdx]->iParentIdx;
	}

}

void CAstarMgr::Start_Astar(const D3DXVECTOR3 & vStart, const D3DXVECTOR3 & vGoal)
{
	Release();

	CObj*	pMyTerrain = CObjMgr::Get_Instance()->Get_Terrain();
	vector<TILE*>&	vecTile = dynamic_cast<CMyTerrain*>(pMyTerrain)->Get_VecTile();

	if (vecTile.empty())
		return;
	
	m_iStartIdx = Get_TileIdx(vStart);

	int iGoalIdx = Get_TileIdx(vGoal);

	if (0 > m_iStartIdx || 
		0 > iGoalIdx || 
		vecTile.size() <= (size_t)m_iStartIdx || 
		vecTile.size() <= (size_t)iGoalIdx)
		return;

	if (m_iStartIdx == iGoalIdx)
		return;

	if (0 != vecTile[iGoalIdx]->byOption)
		return;

	if (true == Make_Route(m_iStartIdx, iGoalIdx))
		Make_BestList(m_iStartIdx, iGoalIdx);
}



void CAstarMgr::Release()
{
	m_OpenList.clear();
	m_CloseList.clear();
	m_BestList.clear();
}


int CAstarMgr::Get_TileIdx(const D3DXVECTOR3 & vPos)
{
	CObj*	pMyTerrain = CObjMgr::Get_Instance()->Get_Terrain();
	vector<TILE*>&	vecTile = dynamic_cast<CMyTerrain*>(pMyTerrain)->Get_VecTile();

	if (vecTile.empty())
		return -1;

	for (size_t index = 0; index < vecTile.size(); ++index)
	{
		if (Picking_Dot(vPos, index))
		{
			return index;
		}
	}

	return -1;
}

bool CAstarMgr::Picking_Dot(const D3DXVECTOR3 & vPos, const int & iIndex)
{
	CObj*	pMyTerrain = CObjMgr::Get_Instance()->Get_Terrain();
	vector<TILE*>&	vecTile = dynamic_cast<CMyTerrain*>(pMyTerrain)->Get_VecTile();
	if (vecTile.empty())
		return false;

	if (vPos.x > int(vecTile[iIndex]->vPos.x  * 2 - (TILECX * 0.5f  * 2))
		&& vPos.x <= int(vecTile[iIndex]->vPos.x  * 2 + (TILECX * 0.5f  * 2))
		&& vPos.y > int(vecTile[iIndex]->vPos.y  * 2 - (TILECY * 0.5f  * 2))
		&& vPos.y <= int(vecTile[iIndex]->vPos.y  * 2 + (TILECY * 0.5f  * 2)))
	{
		return true;
	}
	return false;
}


bool CAstarMgr::Check_Open(int iIndex)
{
	for (int& iOpenIdx : m_OpenList)
	{
		if (iOpenIdx == iIndex)
			return true;
	}

	return false;
}

bool CAstarMgr::Check_Close(int iIndex)
{
	for (int& iCloseIdx : m_CloseList)
	{
		if (iCloseIdx == iIndex)
			return true;
	}
	return false;
}
