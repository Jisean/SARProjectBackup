/*!
 * \file Pattern2.h
 * \date 2016/02/09 12:36
 *
 * \author Nia
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef CrashPattern_h__
#define CrashPattern_h__

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
class CObjectCol;

class CPattern2 : public CObstacle
{
private:
	float				m_fSpeed;
	float				m_fDistance;
	float				m_fCameraAngle;
	float				m_fMemAngle;
public:
	enum		MOVEWAY
	{
		MW_X,
		MW_Y,
		MW_Z,
		MW_RevX,
		MW_RevY,
		MW_RevZ,
		MW_END
	};
	MOVEWAY		eMoveState;

private:
	ENGINE::CResourcesMgr*	m_pResourceMgr;
	ENGINE::CManagement*	m_pManagement;
	ENGINE::CTimeMgr*		m_pTimeMgr;

	ENGINE::CVIBuffer*		m_pBuffer;
	D3DXMATRIX				matRot;
	D3DXVECTOR3				memScale;
	D3DXVECTOR3				memTrans;
	D3DXVECTOR3				memOriginPos;
	DWORD					m_dwTime;

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
	bool					m_bPeak;


private:
	void		SetTransForm(void);
	void		KeyInput(void);
	void		Move(void);
public:
	static  CPattern2* Create(LPDIRECT3DDEVICE9 pDevice);
	void		Update(void);
	void		Render(void);
	void		SetPos(const D3DXVECTOR3& vPos);
	void		SetScale(const D3DXVECTOR3& vScale);
	void		SetTrans(const D3DXVECTOR3& vTrans);
	void		SetSpeed(const float& fSpeed);
	void		SetOriginPos(const D3DXVECTOR3& vOriginPos);
	void		SetMoveWay(const	MOVEWAY&	eState);

	D3DXVECTOR3* GetPos(void);
	const	ENGINE::VTXCUBE*	GetCubeVertex();

private:
	HRESULT		Initialize(void);
	HRESULT		AddComponent(void);
	void		Release(void);
	D3DXVECTOR3	GetMaxPos(const ENGINE::VTXCUBE* pCube);
	D3DXVECTOR3	GetMinPos(const ENGINE::VTXCUBE* pCube);

private:
	explicit CPattern2(LPDIRECT3DDEVICE9 pDevice);
public:
	~CPattern2();



};
#endif // CrashPattern_h__