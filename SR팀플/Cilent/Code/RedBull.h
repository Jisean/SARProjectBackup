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

#ifndef RedBull_h__
#define RedBull_h__

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
class CObjectCol;
class CRedBullTex;
class CRedBull : public ENGINE::CGameObject
{
private:
	float				m_fSpeed;
	float				m_fDistance;
	float				m_fCameraAngle;
public:
	bool				m_bFirstCol;

private:
	ENGINE::CResourcesMgr*	m_pResourceMgr;
	ENGINE::CManagement*	m_pManagement;
	ENGINE::CTimeMgr*		m_pTimeMgr;
	ENGINE::CTransform*		m_pInfo;

	ENGINE::CVIBuffer*		m_pBuffer;
	ENGINE::CTexture*		m_pTexture;


private:
	/*ENGINE::VTXTEX*			m_pVertex;
	ENGINE::VTXTEX*			m_pConvertVertex;*/
	ENGINE::VTXCUBE*		m_pVertex;
	ENGINE::VTXCUBE*		m_pConvertVertex;
	ENGINE::VTXCUBE*		m_pColVtx;
	CObjectCol*				m_pObjectCol;
	DWORD					m_dwVtxCnt;

	ENGINE::CInfoSubject*	m_pInfoSubject;
	CCameraObserver*		m_pCameraObserver;

private:
	CCollisionMgr*			m_pCollisionMgr;
	CTerrainCol*			m_pTerrainCol;
	const ENGINE::VTXTEX*	m_pTerrainVertex;
	D3DXVECTOR3				m_vDestPos;
	bool					m_bMove;
	CRedBullTex*			m_pMonsterTex;	


private:
	void		SetTransForm(void);
public:
	ENGINE::CTransform* GetInfo(void)
	{
		return m_pInfo;
	}
	static  CRedBull* Create(LPDIRECT3DDEVICE9 pDevice);
	void		Update(void);
	void		Render(void);
	void		SetPos(const D3DXVECTOR3& vPos);
	CRedBullTex* GetMonsterTex(void)
	{
		return m_pMonsterTex;
	}
	CCameraObserver*	GetCamObserver(void)
	{
		return m_pCameraObserver;
	}
	D3DXVECTOR3* GetPos(void);
	const	ENGINE::VTXCUBE*	GetCubeVertex();
private:
	HRESULT		Initialize(void);
	HRESULT		AddComponent(void);
	void		Release(void);
	D3DXVECTOR3	GetMaxPos(const ENGINE::VTXCUBE* pCube);
	D3DXVECTOR3	GetMinPos(const ENGINE::VTXCUBE* pCube);

private:
	explicit CRedBull(LPDIRECT3DDEVICE9	pDevice);
public:
	~CRedBull();

};




#endif // RedBull_h__