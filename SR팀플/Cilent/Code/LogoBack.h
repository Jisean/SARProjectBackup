/*!
 * \file LogoBack.h
 * \date 2016/01/18 14:37
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

#ifndef LogoBack_h__
#define LogoBack_h__

#include "GameObject.h"

namespace ENGINE
{
	class CTexture;
	class CVIBuffer;

	class CResourcesMgr;
	class CTimeMgr;
}

class CLogoBack 
	: public ENGINE::CGameObject
{
private:
	ENGINE::CVIBuffer*			m_pBuffer;
	ENGINE::CTexture*			m_pTexture;

	ENGINE::CResourcesMgr*		m_pResourceMgr;
	ENGINE::CTimeMgr*			m_pTimeMgr;

private:
	float			m_fFrameCnt;
	float			m_fFrameMax;

public:
	virtual void Update(void);
	virtual void Render(void);

private:
	virtual HRESULT	Initialize(void);
	HRESULT	AddComponent(void);
	void	Release(void);

public:
	static CLogoBack*	Create(LPDIRECT3DDEVICE9 pDevice);

private:
	explicit CLogoBack(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CLogoBack(void);
};

#endif // LogoBack_h__