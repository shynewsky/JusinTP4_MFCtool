#pragma once

#include "Include.h"

class CDevice
{
	DECLARE_SINGLETON(CDevice)

private:
	CDevice();
	~CDevice();

public:
	LPDIRECT3DDEVICE9	Get_Device() { return m_pDevice; }
	LPD3DXSPRITE		Get_Sprite() { return m_pSprite; }
	LPD3DXFONT			Get_Font() { return m_pFont; }

public:
	// 장치 초기화 과정

	// 1. 장치를 조사할 객체를 먼저 생성
	// 2. 장치를 조사하도록 지시(지원 수준 파악)
	// 3. 장치를 제어할 객체를 생성

	HRESULT		Init_Device();
	void		Render_Begin();
	void		Render_End(HWND hWnd = nullptr);
	void		Release();

private:
	void		Set_Parameters(D3DPRESENT_PARAMETERS& d3dpp);

private:
	LPDIRECT3D9			m_pSDK;		// 1번에 해당하는 객체
	LPDIRECT3DDEVICE9	m_pDevice;	// 3번에 해당하는 객체
	LPD3DXSPRITE		m_pSprite;	// dx 상에서 2d 이미지 출력을 담당하는 객체
	LPD3DXFONT			m_pFont;
};

