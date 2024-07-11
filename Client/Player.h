#pragma once
#include "Obj.h"
class CPlayer : public CObj
{
public:
	CPlayer();
	virtual ~CPlayer();

public:
	virtual HRESULT Initialize(void) override;
	virtual int Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;

private:
	void	Key_Input();
	void	Move_AStar();
	void	Set_Move_Dir(MOVEDIR _eDir);
	void	Move_Dir();
	void	Read_Scenario();

private:
	int			m_iTileIndex;
	D3DXVECTOR3	m_vTargetTile;
	int			m_iTargetTileIndex;

	bool		m_bMoveDirNow;
	bool		m_bMoveAstarNow;

	bool		m_bReservMove;
	MOVEDIR		m_eMoveReserv;

	bool		m_bScrollCtr;
	D3DXVECTOR3	m_vOriginSpot;

	MOVEDIR		m_ePlayerDir;

	bool		m_bScenario;
	SCENARIODATA m_tSpeech;
	bool		m_bDrawScenario;

};

