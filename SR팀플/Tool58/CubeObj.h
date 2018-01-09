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

#ifndef CubeObj_h__
#define CubeObj_h__

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
	class CResources;
	
	class CCubeTex;
	class CTriCol;
	class CRcTex;
}
class CCameraObserver;
class CCollisionMgr;
class CMouseCol;
class CCubeObj : public CObstacle
{
private:
	float				m_fSpeed;
	float				m_fDistance;
	float				m_fCameraAngle;
	D3DXVECTOR3			m_vDestPos;

private:
	ENGINE::CResourcesMgr*	m_pResourceMgr;
	ENGINE::CManagement*	m_pManagement;
	ENGINE::CTimeMgr*		m_pTimeMgr;

private:
	ENGINE::CVIBuffer*		m_pBuffer;
	ENGINE::CResources*		m_pResources;

	CCollisionMgr*			m_pCollisionMgr;
	CMouseCol*				m_pMouseCol;

private:
	ENGINE::CInfoSubject*	m_pInfoSubject;

private:
	const ENGINE::VTXTEX*	m_pTerrainVertex;

public:
	ENGINE::CCubeTex*	GetCubeTex(void);
	
	

private:
	void		SetTransForm(void);

public:
	static  CCubeObj* Create(LPDIRECT3DDEVICE9 pDevice);
	void		Update(void);
	void		Render(void);
	void		SetPos(const D3DXVECTOR3& vPos);
	void		TexToPos(void);



private:
	HRESULT		Initialize(void);
	HRESULT		AddComponent(void);
	void		Release(void);

private:
	explicit CCubeObj(LPDIRECT3DDEVICE9	pDevice);
public:
	~CCubeObj();

};


#endif // CubeObj_h__