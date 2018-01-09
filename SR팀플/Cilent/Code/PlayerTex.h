/*!
 * \file PlayerTex.h
 * \date 2016/02/02 17:48
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


#ifndef PlayerTex_h__
#define PlayerTex_h__


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
class CMouseCol;
class CPlayerTex : public ENGINE::CGameObject
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


private:
	void		KeyInput(void);
	void		SetTransForm(void);

public:
	static  CPlayerTex* Create(LPDIRECT3DDEVICE9 pDevice);
	void		Update(void);
	void		Render(void);

private:
	HRESULT		Initialize(void);
	HRESULT		AddComponent(void);
	void		Release(void);
	void		Move(void);

private:
	D3DXVECTOR3*		m_pPos;

public:
	void	SetPlayerTex(D3DXVECTOR3*	vPos);
	ENGINE::VTXTEX*	GetpVertex(void);
	ENGINE::CTransform* GetpInfo(void);
	CCameraObserver* GetCameraObserver(void)
	{
		return m_pCameraObserver;
	}

private:
	explicit CPlayerTex(LPDIRECT3DDEVICE9	pDevice);
public:
	~CPlayerTex();

};

#endif // PlayerTex_h__