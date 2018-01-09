/*!
 * \file ScreenEffect.h
 * \date 2016/02/16 21:13
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

#ifndef ScreenEffect_h__
#define ScreenEffect_h__

#include "GameObject.h"

namespace ENGINE
{
	class CTexture;
	class CVIBuffer;

	class CResourcesMgr;
	class CTimeMgr;
}

class CScreenEffect
	: public ENGINE::CGameObject
{
private:
	ENGINE::CVIBuffer*			m_pBuffer;
	ENGINE::CTexture*			m_pTexture[3];

	ENGINE::CResourcesMgr*		m_pResourceMgr;
	ENGINE::CTimeMgr*			m_pTimeMgr;
private:
	float			m_fFrameCnt;
	float			m_fFrameMax;
	float			m_fFrameSpeed;

	bool			test1;
	bool			test2;

	wstring			m_wImage;
public:
	virtual void Update(void);
	virtual void Render(void);

private:
	virtual HRESULT	Initialize(void);
	HRESULT	AddComponent(void);
	void	Release(void);

public:
	static CScreenEffect*	Create(LPDIRECT3DDEVICE9 pDevice);

private:
	explicit CScreenEffect(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CScreenEffect(void);

};



#endif // ScreenEffect_h__