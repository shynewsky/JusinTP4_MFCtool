#pragma once

#include "Include.h"

class CToolView;
class CTerrain
{
public:
	CTerrain();
	~CTerrain();

public:
	vector<TILE*>& Get_VecTile() { return  m_vecTile; }

public:
	void		Initialize();
	void		Update();
	void		Render();
	void		Mini_Render();
	void		Release();

public:
	int		Get_TileIdx(const D3DXVECTOR3& vPos);
	void	Tile_Change(const D3DXVECTOR3& vPos, const int& iDrawID, const int& iOption);
	bool	Picking(const D3DXVECTOR3& vPos, const int& iIndex);
	bool	Picking_Dot(const D3DXVECTOR3& vPos, const int& iIndex);

public:
	void		Set_MainView(CToolView* pMainView) { m_pMainView = pMainView; }
	void		Set_Ratio(D3DXMATRIX* pOut, float fRatioX, float fRatioY);
	void		Set_Scale(float _fScale) { m_fScale = _fScale; }
	void		Add_Scale(float _fScale);
	float		Get_Scale() { return m_fScale; }



private:
	vector<TILE*>		m_vecTile;
	CToolView*			m_pMainView;
	float				m_fScale;

};

