/*!
 * \file CubeObj.h
 * \date 2016/01/27 13:50
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

#ifndef SpinObstacle_h__
#define SpinObstacle_h__

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
class CSpinblade;
class CSpinObstacle : public CObstacle
{
private:
	float				m_fSpeed;
	float				m_fDistance;
	float				m_fCameraAngle;
	D3DXVECTOR3			m_vCopyPos;

private:
	ENGINE::CResourcesMgr*	m_pResourceMgr;
	ENGINE::CManagement*	m_pManagement;
	ENGINE::CTimeMgr*		m_pTimeMgr;
	

	ENGINE::CVIBuffer*		m_pBuffer;

private:
	ENGINE::CInfoSubject*	m_pInfoSubject;

public:
	CSpinblade*				m_pMonster1;
	CSpinblade*				m_pMonster2;

private:
	CCollisionMgr*			m_pCollisionMgr;
	CTerrainCol*			m_pTerrainCol;
	const ENGINE::VTXTEX*	m_pTerrainVertex;

private:
	ENGINE::VTXCUBE*		m_pColVtx;

private:
	void		SetTransForm(void);

public:
	static  CSpinObstacle* Create(LPDIRECT3DDEVICE9 pDevice);
	const	ENGINE::VTXCUBE*	GetCubeVertex();
	void		Update(void);
	void		Render(void);
	void		SetPos(const D3DXVECTOR3& vPos);
	D3DXVECTOR3*	GetPos(void);
	ENGINE::VTXCUBE* GetpVertex(void)
	{
		return m_pVertex;
	}
	CCameraObserver*	GetCamObserver(void)
	{
		return m_pCameraObserver;
	}

	void TexToPos(void);
private:
	HRESULT		Initialize(void);
	HRESULT		AddComponent(void);
	void		Release(void);

private:
	explicit CSpinObstacle(LPDIRECT3DDEVICE9	pDevice);
public:
	~CSpinObstacle();

};


#endif // SpinObstacle_h__