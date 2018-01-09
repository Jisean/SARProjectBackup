/*!
 * \file Player.h
 * \date 2016/01/13 14:28
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

#ifndef Player_h__
#define Player_h__


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
class CObjectCol;
class CPlayerTex;
class CPlayer : public ENGINE::CGameObject
{
public:
	enum	STAGESTATE
	{
		ST_STAGE,
		ST_STAGE1,
		ST_SHOT,
		ST_STAGE2
	};
	STAGESTATE	eStageState;


private:
	float				m_fSpeed;
	float				m_fDistance;
	float				m_fCameraAngle;
	float				m_fJumpAcc;
	float				m_fPeak;
	bool				m_bUp;
	bool				m_bDown;
	float				m_fBeforY;
	float				m_fTemp;
	float				m_fRedBullTime;
	bool				m_bUpDowncol;
	bool				m_bYcol;
	bool				m_bZcol;
	DWORD				m_Time;
	int					m_iOriginX;
	bool				m_bTemp;
	bool				m_bOnShrink;
	bool				m_bFired;
	D3DXVECTOR3			m_RespawnPos;

private:
	ENGINE::CResourcesMgr*	m_pResourceMgr;
	ENGINE::CManagement*	m_pManagement;
	ENGINE::CTimeMgr*		m_pTimeMgr;
	ENGINE::CTransform*		m_pInfo;

	ENGINE::CVIBuffer*		m_pBuffer;
	ENGINE::CTexture*		m_pTexture;
	bool					m_bBool;

private:
	ENGINE::VTXCUBE*		m_pVertex;
	ENGINE::VTXCUBE*		m_pConvertVertex;
	ENGINE::VTXCUBE*		m_pColVtx;
	ENGINE::VTXCUBE*		m_pYColVtx;
	/*ENGINE::VTXTEX*		m_pVertex;
	ENGINE::VTXTEX*			m_pConvertVertex;*/
	DWORD					m_dwVtxCnt;

	ENGINE::CInfoSubject*	m_pInfoSubject;
	CCameraObserver*		m_pCameraObserver;
	ENGINE::CGameObject*	m_Ycoliter;
	CPlayerTex*				m_pPlayerTex;

private:
	CCollisionMgr*			m_pCollisionMgr;
	CTerrainCol*			m_pTerrainCol;
	CMouseCol*				m_pMouseCol;
	CObjectCol*				m_pObjectCol;
	const ENGINE::VTXTEX*	m_pTerrainVertex;
	const ENGINE::VTXCUBE*	m_pCubeVertex;

	D3DXVECTOR3				m_vDestPos;
	D3DXVECTOR3				m_vColObjMaxPos;
	D3DXVECTOR3				m_vColObjMinPos;
	bool					m_bMove;
	float					m_fColDisX;
	float					m_fColDisZ;


private:
	void		KeyInput(void);
	void		SetTransForm(void);
	
public:
	static int CubeCnt;
	static  CPlayer* Create(LPDIRECT3DDEVICE9 pDevice);
	void		Update(void);
	void		Render(void);
	void		Jump(void);
	void		CubeCollision(void);
	void		FireBullet(void);
	void		BookCheck(void);
	void		CentryCheck(void);
private:
	HRESULT		Initialize(void);
	HRESULT		AddComponent(void);
	void		Release(void);
	void		Move(void);
	D3DXVECTOR3	GetMaxPos(const ENGINE::VTXCUBE* pCube);
	D3DXVECTOR3	GetMinPos(const ENGINE::VTXCUBE* pCube);

public:
	ENGINE::CTransform*	GetInfo(void);
	void SetStageState(STAGESTATE eState);

	
private:
	explicit CPlayer(LPDIRECT3DDEVICE9	pDevice);
public:
	~CPlayer();

};


#endif // Player_h__