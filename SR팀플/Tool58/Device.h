#pragma once

#include "Include.h"

class CDevice
{
	DECLARE_SINGLETON2(CDevice)

private:
	LPDIRECT3D9			m_p3D;
	LPDIRECT3DDEVICE9	m_pDevice;
	LPD3DXSPRITE		m_pSprite;
	LPD3DXFONT			m_pFont;

public:
	HRESULT		InitDevice(void);
	void		Release(void);
	void		SetParameters(D3DPRESENT_PARAMETERS& d3dpp);
	void		Render_Begin(void);
	void		Render_End(void);

public:
	LPDIRECT3DDEVICE9	GetDevice(void);
	LPD3DXSPRITE		GetSprite(void);
	LPD3DXFONT			GetFont(void);
private:
	CDevice(void);
	~CDevice(void);
};