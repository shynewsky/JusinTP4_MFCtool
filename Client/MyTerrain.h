#pragma once

#include "Obj.h"

class CMyTerrain : public CObj
{
public:
	CMyTerrain();
	virtual ~CMyTerrain();

public:
	vector<TILE*>& Get_VecTile() { return m_vecTile; }
	vector<list<TILE*>>& Get_VecAdj() { return m_vecAdj; }

public:
	virtual HRESULT Initialize(void) override;
	virtual int Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;

private:
	HRESULT		Load_Tile(const TCHAR* pFilePath);
	HRESULT		Ready_Adj();

private:
	vector<TILE*>		m_vecTile;
	vector<list<TILE*>>	m_vecAdj;		//타일 하나가 가지고 있을 자신을 기준으로 이동할수 있는 타일들의 정보
};

