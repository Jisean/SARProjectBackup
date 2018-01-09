/*!
 * \file Renderer.h
 * \date 2016/01/06 14:37
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

#ifndef Renderer_h__
#define Renderer_h__

#include "Engine_Include.h"

BEGIN(ENGINE)

class CScene;
class ENGINE_DLL	CRenderer
{
private:
	LPDIRECT3DDEVICE9			m_pDevice;
	CScene*						m_pScene;
	ID3DXFont*					m_pD3DXFont;
	float						m_fTime;
	WORD						m_wFrameCnt;
	TCHAR						m_szFPS[128];

public:
	static	CRenderer*		Create(LPDIRECT3DDEVICE9 pDevice);
	HRESULT	InitRenderer(void);
	void	Render(float fTime);

public:
	void	SetScene(CScene*	pScene);

private:
	CRenderer(LPDIRECT3DDEVICE9 pDevice);
public:
	~CRenderer();

};

END

#endif // Renderer_h__