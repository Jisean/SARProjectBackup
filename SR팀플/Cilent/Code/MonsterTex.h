/*!
 * \file MonsterTex.cpp
 * \date 2016/02/05 11:24
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
#ifndef MonsterTex_h__
#define MonsterTex_h__

#include "GameObject.h"

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
class CMouseCol;
class CMonsterTex : public ENGINE::CGameObject
{
private:
	float				m_fSpeed;
	float				m_fDistance;
	float				m_fCameraAngle;
	int					m_iFrame;
	float				m_fFrame2;
	float				m_fTime;
	bool				m_bDead;
	bool				m_bDelete;
private:
	ENGINE::CResourcesMgr*	m_pResourceMgr;
	ENGINE::CManagement*	m_pManagement;
	ENGINE::CTimeMgr*		m_pTimeMgr;
	ENGINE::CTransform*		m_pInfo;

	ENGINE::CVIBuffer*		m_pBuffer;
	ENGINE::CTexture*		m_pTexture[2];


private:
	ENGINE::VTXTEX*			m_pVertex;
	ENGINE::VTXTEX*			m_pConvertVertex;
	DWORD					m_dwVtxCnt;

	ENGINE::CInfoSubject*	m_pInfoSubject;
	CCameraObserver*		m_pCameraObserver;
	CGameObject*			m_pTarget;

private:
	CCollisionMgr*			m_pCollisionMgr;
	CTerrainCol*			m_pTerrainCol;
	CMouseCol*				m_pMouseCol;
	const ENGINE::VTXTEX*	m_pTerrainVertex;

	D3DXVECTOR3				m_vDestPos;
	bool					m_bMove;

public:
	static  CMonsterTex* Create(LPDIRECT3DDEVICE9 pDevice);
	void		Update(void);
	void		Render(void);
	ENGINE::VTXTEX*			GetpVertex(void)
	{
		return m_pVertex;
	}
	ENGINE::CTransform* GetpInfo(void)
	{
		return m_pInfo;
	}
	bool		GetDelete(void)
	{
		return m_bDelete;
	}
	void		SetDead(bool bDead)
	{
		m_bDead = bDead;
	}
	CCameraObserver*	GetCamObserver(void)
	{
		return m_pCameraObserver;
	}
private:
	HRESULT		Initialize(void);
	HRESULT		AddComponent(void);
	void		Release(void);
	void		SetTransForm(void);
	void		SetFrame(void);

private:
	D3DXVECTOR3*		m_pPos;

public:
	void	SetMonsterTex(D3DXVECTOR3*	vPos);
private:
	explicit CMonsterTex(LPDIRECT3DDEVICE9 pDevice);
public:
	~CMonsterTex();

};
#endif // MonsterTex_h__