/*!
 * \file PlayerUI.h
 * \date 2016/02/10 14:48
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

#ifndef PlayerUI_h__
#define PlayerUI_h__

#include "GameObject.h"

namespace ENGINE
{
	class CTexture;
	class CVIBuffer;
	
	class CResourcesMgr;
	class CTimeMgr;
}

class CPlayerUI
	: public ENGINE::CGameObject
{
private:
	ENGINE::CVIBuffer*			m_pBuffer;
	ENGINE::CTexture*			m_pTexture[16];

	ENGINE::CResourcesMgr*		m_pResourceMgr;
	ENGINE::CTimeMgr*			m_pTimeMgr;
	
private:
	float			m_fFrameCnt;
	float			m_fFrameMax;
	
	float			m_fFrameSpeed;
	/*bool			m_bPisTol;
	bool			m_bRifle;
	bool			m_bKnife;
	bool			m_bSniper;*/
	
	enum STATE {STATE_READY, STATE_STAND, STATE_SHOT1,STATE_SHOT2, STATE_LOAD};

public:
	virtual void Update(void);
	virtual void Render(void);

private:
	virtual HRESULT	Initialize(void);
	HRESULT	AddComponent(void);
	void	Release(void);
	STATE   eType;
	
public:
	static CPlayerUI*	Create(LPDIRECT3DDEVICE9 pDevice);
	void	FrameWork(void);
	void	KeyInput(void);

private:
	explicit CPlayerUI(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CPlayerUI(void);
};

#endif // PlayerUI_h__