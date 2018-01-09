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

#ifndef Monster_h__
#define Monster_h__

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
class CMonsterTex;
class CMonster : public ENGINE::CGameObject
{
private:
	float				m_fSpeed;
	float				m_fDistance;
	float				m_fCameraAngle;

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
	CMonsterTex*			m_pMonsterTex;	


private:
	void		SetTransForm(void);
	void		KeyInput(void);
	void		Move(void);
public:
	static  CMonster* Create(LPDIRECT3DDEVICE9 pDevice);
	void		Update(void);
	void		Render(void);
	void		SetPos(const D3DXVECTOR3& vPos);
	CMonsterTex* GetMonsterTex(void)
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
	explicit CMonster(LPDIRECT3DDEVICE9	pDevice);
public:
	~CMonster();

};




#endif // Monster_h__