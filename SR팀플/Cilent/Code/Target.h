/*!
 * \file Target.h
 * \date 2016/02/13 14:39
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

#ifndef Target_h__
#define Target_h__

#include "GameObject.h"
namespace	ENGINE
{
	class CResourcesMgr;
	class CManagement;
	class CTimeMgr;

	class CVIBuffer;
	class CTexture;
	class CTransform;
	class CInfoSubject;
}
class CCameraObserver;
class CCollisionMgr;
class CTerrainCol;
class CMouseCol;

class CTarget
	: public ENGINE::CGameObject
{
private:
	D3DXMATRIX				matRot;
	float					m_fAngle;
	bool					m_bup;
	bool					m_bStay;
	bool					m_bDown;
	bool					m_bisDead;
	bool					m_bStart;

	float					m_fStay;
	float					m_fDeadTime;

	bool					m_bLife;
	bool					m_bMiss;
	int						m_iIndex;
private:
	ENGINE::CResourcesMgr*	m_pResourceMgr;
	ENGINE::CManagement*	m_pManagement;
	ENGINE::CTimeMgr*		m_pTimeMgr;
	ENGINE::CTransform*		m_pInfo;

	ENGINE::CVIBuffer*		m_pBuffer;
	ENGINE::CTexture*		m_pTexture[5];


private:
	ENGINE::VTXTEX*			m_pVertex;
	ENGINE::VTXTEX*			m_pVertexTemp;
	ENGINE::VTXTEX*			m_pConvertVertex;
	DWORD					m_dwVtxCnt;

	ENGINE::CInfoSubject*	m_pInfoSubject;
	CCameraObserver*		m_pCameraObserver;
	

private:
	void		SetTransForm(void);
	void		RotateX(void);

public:
	static		CTarget*	Create(LPDIRECT3DDEVICE9 pDevice);
	void		Update(void);
	void		Render(void);
	void		SetPos(const D3DXVECTOR3& vPos);
	void		SetLife(bool bLife)
	{
		m_bLife = bLife;
	}
	bool		GetLife(void)
	{
		return m_bisDead;
	}

public:
	ENGINE::VTXTEX*	GetpVertex(void);
	ENGINE::CTransform* GetpInfo(void)
	{
		return m_pInfo;
	}
	CCameraObserver*	GetCamObserver(void)
	{
		return m_pCameraObserver;
	}
private:
	HRESULT		Initialize(void);
	HRESULT		AddComponet(void);
	void		Release(void);
private:
	explicit CTarget(LPDIRECT3DDEVICE9 pDevice);
public:
	~CTarget();

};


#endif // Target_h__