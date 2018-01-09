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

#ifndef ShotTimer_h__
#define ShotTimer_h__

#include "GameObject.h"
namespace ENGINE
{
	class CGraphicDev;
	class CTimeMgr;
}

class CShotTimer 
	: public ENGINE::CGameObject
{
private:
	ENGINE::CGraphicDev*		m_pGraphicDev;
	ENGINE::CTimeMgr*			m_pTimer;
private:
	float			m_fFrameCnt;
	float			m_fFrameMax;
	float			m_fTime;
	float			m_fTimeStart;
	bool			m_bShowStart;
	bool			m_bShowOver;
	bool			m_bTemp;
public:
	virtual void Update(void);
	virtual void Render(void);

private:
	virtual HRESULT	Initialize(void);
	void	Release(void);

public:
	static CShotTimer*	Create(LPDIRECT3DDEVICE9 pDevice);

private:
	explicit CShotTimer(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CShotTimer(void);
};

#endif // ShotTimer_h__