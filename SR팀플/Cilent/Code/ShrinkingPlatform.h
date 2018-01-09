/*!
 * \file ShrinkingPlatform.h
 * \date 2016/02/10 13:33
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

#ifndef ShrinkingPlatform_h__
#define ShrinkingPlatform_h__

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
class CPattern2;
class CShrinkingPlatform : public CObstacle
{
private:
	float				m_fSpeed;
	float				m_fDistance;
	float				m_fCameraAngle;
	D3DXVECTOR3			m_vCopyPos;
	D3DXVECTOR3				m_vMazeScale;
	float				m_fMazeTrans;
	D3DXVECTOR3			memOriginPos;
	bool				m_bPosUpdate1;
	bool				m_bPosUpdate2;


private:
	ENGINE::CResourcesMgr*	m_pResourceMgr;
	ENGINE::CManagement*	m_pManagement;
	ENGINE::CTimeMgr*		m_pTimeMgr;

	ENGINE::CVIBuffer*		m_pBuffer;

private:
	ENGINE::CInfoSubject*	m_pInfoSubject;

public:
	CPattern2*	m_pPattern1;
	CPattern2*	m_pPattern2;
private:
	CCollisionMgr*			m_pCollisionMgr;
	CTerrainCol*			m_pTerrainCol;
	const ENGINE::VTXTEX*	m_pTerrainVertex;

private:
	ENGINE::VTXCUBE*		m_pColVtx;

private:
	void		SetTransForm(void);

public:
	static  CShrinkingPlatform* Create(LPDIRECT3DDEVICE9 pDevice);
	const	ENGINE::VTXCUBE*	GetCubeVertex();
	void		Update(void);
	void		Render(void);
	void		SetPos(const D3DXVECTOR3& vPos);
	void		SetScale(const D3DXVECTOR3& fScale);
	void		SetTrans(const float& fTrans);
	void		SetOriginPos(const D3DXVECTOR3& vOriginPos);
	void		SetSpeed(const float& fSpeed);
	D3DXVECTOR3*	GetPos(void);

private:
	HRESULT		Initialize(void);
	HRESULT		AddComponent(void);
	void		Release(void);

private:
	explicit CShrinkingPlatform(LPDIRECT3DDEVICE9	pDevice);
public:
	~CShrinkingPlatform();


private:
};
#endif // ShrinkingPlatform_h__