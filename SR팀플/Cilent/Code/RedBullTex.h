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
#ifndef RedBullTex_h__
#define RedBullTex_h__

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
class CRedBullTex : public ENGINE::CGameObject
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
	ENGINE::VTXTEX*			m_pVertex;
	ENGINE::VTXTEX*			m_pConvertVertex;
	DWORD					m_dwVtxCnt;

	ENGINE::CInfoSubject*	m_pInfoSubject;
	CCameraObserver*		m_pCameraObserver;

private:
	CCollisionMgr*			m_pCollisionMgr;
	CTerrainCol*			m_pTerrainCol;
	CMouseCol*				m_pMouseCol;
	const ENGINE::VTXTEX*	m_pTerrainVertex;

	D3DXVECTOR3				m_vDestPos;
	bool					m_bMove;

public:
	static  CRedBullTex* Create(LPDIRECT3DDEVICE9 pDevice);
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
	CCameraObserver*	GetCamObserver(void)
	{
		return m_pCameraObserver;
	}
private:
	HRESULT		Initialize(void);
	HRESULT		AddComponent(void);
	void		Release(void);
	void		SetTransForm(void);

private:
	D3DXVECTOR3*		m_pPos;

public:
	void	SetMonsterTex(D3DXVECTOR3*	vPos);
private:
	explicit CRedBullTex(LPDIRECT3DDEVICE9 pDevice);
public:
	~CRedBullTex();

};
#endif // RedBullTex_h__