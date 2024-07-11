#pragma once

#include "Include.h"

class CAstarMgr
{
	DECLARE_SINGLETON(CAstarMgr)

private:
	CAstarMgr();
	~CAstarMgr();

public:
	list<TILE*>&	Get_BestList() { return m_BestList; }

public:
	void		Start_Astar(const D3DXVECTOR3& vStart, const D3DXVECTOR3& vGoal);

	bool		Make_Route(int iStartIdx, int iGoalIdx);
	void		Make_BestList(int iStartIdx, int iGoalIdx);

public:
	int			Get_TileIdx(const D3DXVECTOR3& vPos);
	bool		Picking_Dot(const D3DXVECTOR3& vPos, const int& iIndex);
	void		Release();

private:
	bool		Check_Open(int iIndex);
	bool		Check_Close(int iIndex);


private:
	list<int>			m_OpenList;
	list<int>			m_CloseList;
	list<TILE*>			m_BestList;

	int					m_iStartIdx;
};

