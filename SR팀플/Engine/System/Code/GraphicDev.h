/*!
 * \file GraphicDev.h
 * \date 2016/01/05 14:49
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef GraphicDev_h__
#define GraphicDev_h__

#include "Engine_Include.h"

BEGIN(ENGINE)

class ENGINE_DLL CGraphicDev
{
public:
	DECLARE_SINGLETON(CGraphicDev)

public:
	enum WINMODE {MODE_FULL, MODE_WIN};
private:
	explicit CGraphicDev(void);
	~CGraphicDev(void);

public: // getter
	LPDIRECT3DDEVICE9 GetDevice(void) {
		return m_pDevice;}
public:
	HRESULT InitGraphicDev(WINMODE Mode, HWND hWnd, const WORD& wSizeX, const WORD& wSizeY);
	LPD3DXSPRITE		GetSprite(void);
	void		Render_Begin(void);
	void		Render_End(void);
private:
	LPD3DXSPRITE				m_pSprite;
	LPDIRECT3D9					m_pSDK;
	LPDIRECT3DDEVICE9			m_pDevice; // 장치를 대표하는 객체다.
private:
	void SetParameters(D3DPRESENT_PARAMETERS& d3dpp, WINMODE Mode, HWND hWnd, const WORD& wSizeX, const WORD& wSizeY);
	void Release(void);
};
END

#endif // GraphicDev_h__