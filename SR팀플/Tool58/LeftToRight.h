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

#ifndef LeftToRight_h__
#define LeftToRight_h__

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
class CLeftToRight : public CObstacle
{
private:
	float				m_fSpeed;
	float				m_fDistance;
	float				m_fCameraAngle;
	D3DXVECTOR3			m_vCopyPos;
	bool				m_bUp;
	bool				m_bDown;
	bool				m_bToRight;
	bool				m_bToLeft;
public:
	float				m_fMaxHeight;
	float				m_fMaxMove;

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
	static  CLeftToRight* Create(LPDIRECT3DDEVICE9 pDevice);
	const	ENGINE::VTXCUBE*	GetCubeVertex();
	void		Update(void);
	void		Render(void);
	void		SetPos(const D3DXVECTOR3& vPos);
	D3DXVECTOR3*	GetPos(void);

	map<wstring, ENGINE::CComponent*>* CLeftToRight::GetmapComponent(void)
	{
		return &m_mapComponent;
	}
private:
	HRESULT		Initialize(void);
	HRESULT		AddComponent(void);
	void		Release(void);

private:
	explicit CLeftToRight(LPDIRECT3DDEVICE9	pDevice);
public:
	~CLeftToRight();

};


#endif // LeftToRight_h__