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

#ifndef Spinblade_h__
#define Spinblade_h__

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
class CSpinblade : public CObstacle
{
private:
	float				m_fSpeed;
	float				m_fDistance;
	float				m_fCameraAngle;
	float				m_fAngle;

private:
	ENGINE::CResourcesMgr*	m_pResourceMgr;
	ENGINE::CManagement*	m_pManagement;
	ENGINE::CTimeMgr*		m_pTimeMgr;

	ENGINE::CVIBuffer*		m_pBuffer;
	ENGINE::CTexture*		m_pTexture;
	D3DXMATRIX				matRot;
	D3DXMATRIX				matTran;
	DWORD					m_dwTime;
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


private:
	void		SetTransForm(void);
	void		Move(void);
public:
	static  CSpinblade* Create(LPDIRECT3DDEVICE9 pDevice);
	void		Update(void);
	void		Render(void);
	void		SetPos(const D3DXVECTOR3& vPos);
	
	D3DXVECTOR3* GetPos(void);
	const	ENGINE::VTXCUBE*	GetCubeVertex();
private:
	HRESULT		Initialize(void);
	HRESULT		AddComponent(void);
	void		Release(void);
	D3DXVECTOR3	GetMaxPos(const ENGINE::VTXCUBE* pCube);
	D3DXVECTOR3	GetMinPos(const ENGINE::VTXCUBE* pCube);


private:
	explicit CSpinblade(LPDIRECT3DDEVICE9	pDevice);
public:
	~CSpinblade();

};




#endif // Spinblade_h__