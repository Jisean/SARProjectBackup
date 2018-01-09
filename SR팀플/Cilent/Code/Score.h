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

#ifndef Score_h__
#define Score_h__

#include "GameObject.h"
namespace ENGINE
{
	class CGraphicDev;
	class CTimeMgr;
}

class CScore 
	: public ENGINE::CGameObject
{
private:
	ENGINE::CGraphicDev*		m_pGraphicDev;
	ENGINE::CTimeMgr*			m_pTimer;
private:
	float			m_fFrameCnt;
	float			m_fFrameMax;
	float			m_fTime;

public:
	virtual void Update(void);
	virtual void Render(void);

private:
	virtual HRESULT	Initialize(void);
	void	Release(void);

public:
	static CScore*	Create(LPDIRECT3DDEVICE9 pDevice);

private:
	explicit CScore(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CScore(void);
};

#endif // Score_h__