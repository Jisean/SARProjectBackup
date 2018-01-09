///*!
// * \file SpinCutter.h
// * \date 2016/02/10 13:33
// *
// * \author Administrator
// * Contact: user@company.com
// *
// * \brief 
// *
// * TODO: long description
// *
// * \note
//*/
//
#ifndef SpinCutter_h__
#define SpinCutter_h__

#include "Obstacle.h"

namespace ENGINE
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
class CPattern1;
class CSpinCutter : public CObstacle
{
private:
	float				m_fSpeed;
	float				m_fDistance;
	float				m_fCameraAngle;
	D3DXVECTOR3			m_vCopyPos;
	D3DXVECTOR3			m_vCutScale;
	D3DXVECTOR3			m_vCutTrans;

private:
	ENGINE::CResourcesMgr*	m_pResourceMgr;
	ENGINE::CManagement*	m_pManagement;
	ENGINE::CTimeMgr*		m_pTimeMgr;

	ENGINE::CVIBuffer*		m_pBuffer;

private:
	ENGINE::CInfoSubject*	m_pInfoSubject;

public:
	CPattern1*	m_pPattern1;
	CPattern1*	m_pPattern2;
	CPattern1*	m_pPattern3;
	CPattern1*	m_pPattern4;
	CPattern1*  m_pPattern5;

private:
	CCollisionMgr*			m_pCollisionMgr;
	CTerrainCol*			m_pTerrainCol;
	const ENGINE::VTXTEX*	m_pTerrainVertex;

private:
	ENGINE::VTXCUBE*		m_pColVtx;

private:
	void		SetTransForm(void);

public:
	static  CSpinCutter* Create(LPDIRECT3DDEVICE9 pDevice);
	const	ENGINE::VTXCUBE*	GetCubeVertex();
	void		Update(void);
	void		Render(void);
	void		SetPos(const D3DXVECTOR3& vPos);
	void		SetScale(const D3DXVECTOR3& fScale);
	void		SetTrans(const D3DXVECTOR3& fTrans);
	D3DXVECTOR3*	GetPos(void);
	
private:
	HRESULT		Initialize(void);
	HRESULT		AddComponent(void);
	void		Release(void);

private:
	explicit CSpinCutter(LPDIRECT3DDEVICE9	pDevice);
public:
	~CSpinCutter();


private:
};
#endif // SpinCutter_h__