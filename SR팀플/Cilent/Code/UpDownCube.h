/*!
 * \file UpDownCube.h
 * \date 2016/02/10 14:59
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

#ifndef UpDownCube_h__
#define UpDownCube_h__


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
class CUpDownCube : public CObstacle
{
private:
	float				m_fSpeed;
	float				m_fDistance;
	float				m_fCameraAngle;
	D3DXVECTOR3			m_vCopyPos;
public:
	float				m_Height;
	bool				m_bMaxHeight;
	bool				m_bMinHeight;
private:
	ENGINE::CResourcesMgr*	m_pResourceMgr;
	ENGINE::CManagement*	m_pManagement;
	ENGINE::CTimeMgr*		m_pTimeMgr;

	ENGINE::CVIBuffer*		m_pBuffer;

private:
	ENGINE::CInfoSubject*	m_pInfoSubject;

private:
	CCollisionMgr*			m_pCollisionMgr;
	CTerrainCol*			m_pTerrainCol;
	const ENGINE::VTXTEX*	m_pTerrainVertex;

private:
	ENGINE::VTXCUBE*		m_pColVtx;

private:
	void		SetTransForm(void);

public:
	static  CUpDownCube* Create(LPDIRECT3DDEVICE9 pDevice);
	const	ENGINE::VTXCUBE*	GetCubeVertex();
	void		Update(void);
	void		Render(void);
	void		SetPos(const D3DXVECTOR3& vPos);
	D3DXVECTOR3*	GetPos(void);
	void TexToPos(void);


private:
	HRESULT		Initialize(void);
	HRESULT		AddComponent(void);
	void		Release(void);

private:
	explicit CUpDownCube(LPDIRECT3DDEVICE9	pDevice);
public:
	~CUpDownCube();

};


#endif // UpDownCube_h__
