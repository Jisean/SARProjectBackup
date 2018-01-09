/*!
 * \file StaticCamera.h
 * \date 2016/01/21 14:29
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

#ifndef StaticCamera_h__
#define StaticCamera_h__

#include "Camera.h"

namespace ENGINE
{
	class CTransform;
	class CTimeMgr;
	class CInfoSubject;
}

class CStaticCamera : public ENGINE::CCamera
{
private:
	explicit CStaticCamera(LPDIRECT3DDEVICE9 pDevice);
public:
	~CStaticCamera();

public:
	static  CStaticCamera* Create(LPDIRECT3DDEVICE9 pDevice, 
									const ENGINE::CTransform* pTargetInfo);
	void	SetCameraTarget(const ENGINE::CTransform* pTargetInfo);
	
public:
	virtual HRESULT		Initialize(void);
	virtual void		Update(void);

	void		Release(void);

private:
	void		KeyCheck(void);
	void		TargetRenewal(void);

private:
	ENGINE::CTimeMgr*			m_pTimeMgr;
	const ENGINE::CTransform*	m_pTargetInfo;
	ENGINE::CInfoSubject*		m_pInfoSubject;

private:
	float		m_fTargetDistance;
	float		m_fCameraSpeed;
	float		m_fAngle;
	float		m_fAngle2;
	DWORD		m_dwTime;
	int			m_iOriginY;
	float		m_Height;
	bool		m_bMaxHeight;
	bool		m_bMinHeight;
};

#endif // StaticCamera_h__