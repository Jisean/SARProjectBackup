/*!
 * \file Monster.h
 * \date 2016/01/26 14:24
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

#ifndef CentryGun_h__
#define CentryGun_h__

#include "Obstacle.h"

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
class CObjectCol;
class CPlayer;
class CBullet;
class CMonsterTex;
class CCentryGun : public CObstacle
{
private:
	float				m_fSpeed;
	float				m_fDistance;
	float				m_fCameraAngle;
	float				m_fAngle;
	DWORD				m_dwTime;
	list<CBullet*>		m_BulletList;
	int					m_iHp;
public:
	list<CBullet*>* GetBulletList(void)
	{
		return &m_BulletList;
	}
	int* GetCentryGunHp(void)
	{
		return &m_iHp;
	}

private:
	ENGINE::CResourcesMgr*	m_pResourceMgr;
	ENGINE::CManagement*	m_pManagement;
	ENGINE::CTimeMgr*		m_pTimeMgr;

	ENGINE::CVIBuffer*		m_pBuffer;
	D3DXMATRIX				matRot;
	D3DXMATRIX				matTran;
	D3DXVECTOR3				vTrans;

public:
	void setvTrans(D3DXVECTOR3 vTran)
	{
		vTrans = vTran;
	}


private:
	/*ENGINE::VTXTEX*			m_pVertex;
	ENGINE::VTXTEX*			m_pConvertVertex;*/
	ENGINE::VTXCUBE*		m_pColVtx;
	CObjectCol*				m_pObjectCol;

	ENGINE::CInfoSubject*	m_pInfoSubject;

private:
	CCollisionMgr*			m_pCollisionMgr;
	CTerrainCol*			m_pTerrainCol;
	const ENGINE::VTXTEX*	m_pTerrainVertex;
	D3DXVECTOR3				m_vDestPos;
	bool					m_bMove;
	CGameObject*			m_pTarget;
	CMonsterTex*			m_pMonsterTex;	


private:
	void		SetTransForm(void);

public:
	static  CCentryGun* Create(LPDIRECT3DDEVICE9 pDevice);
	void				Update(void);
	void				Render(void);
	void				SetPos(const D3DXVECTOR3& vPos);
	CMonsterTex* GetMonsterTex(void)
	{
		return m_pMonsterTex;
	}
public:
	virtual const D3DXVECTOR3*		GetPos(void);
	const	ENGINE::VTXCUBE*	GetCubeVertex();
	void						TexToPos(void);

private:
	HRESULT		Initialize(void);
	HRESULT		AddComponent(void);
	void		Release(void);

private:
	explicit CCentryGun(LPDIRECT3DDEVICE9	pDevice);
public:
	~CCentryGun();

};




#endif // CentryGun_h__