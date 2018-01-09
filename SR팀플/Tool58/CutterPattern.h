/*!
 * \file Pattern1.h
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

#ifndef CutterPattern_h__
#define CutterPattern_h__

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

class CPattern1 : public CObstacle
{
private:
	float				m_fSpeed;
	float				m_fDistance;
	float				m_fCameraAngle;
	float				m_fAngle;
	float				m_fMemAngle;

public:
	enum ROTATEAXIS
	{
		ROT_X,
		ROT_Y,
		ROT_Z,
		ROT_END
	};
	ROTATEAXIS eRotState;

private:
	ENGINE::CResourcesMgr*	m_pResourceMgr;
	ENGINE::CManagement*	m_pManagement;
	ENGINE::CTimeMgr*		m_pTimeMgr;
	ENGINE::CVIBuffer*		m_pBuffer;
	D3DXMATRIX				matRot;
	D3DXVECTOR3				memScale;
	D3DXVECTOR3				memTrans;
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
private:
	void		SetTransForm(void);
	void		KeyInput(void);
	void		Move(void);
public:
	static  CPattern1* Create(LPDIRECT3DDEVICE9 pDevice);
	void		Update(void);
	void		Render(void);
	void		SetPos(const D3DXVECTOR3& vPos);
	void		SetScale(const D3DXVECTOR3& vScale);
	void		SetTrans(const D3DXVECTOR3& vTrans);
	void		SetRotState(const ROTATEAXIS& eState);
	void		SetRotAngle(const float& fAngle);

	D3DXVECTOR3* GetPos(void);
	const	ENGINE::VTXCUBE*	GetCubeVertex();

private:
	HRESULT		Initialize(void);
	HRESULT		AddComponent(void);
	void		Release(void);
	D3DXVECTOR3	GetMaxPos(const ENGINE::VTXCUBE* pCube);
	D3DXVECTOR3	GetMinPos(const ENGINE::VTXCUBE* pCube);

private:
	explicit CPattern1(LPDIRECT3DDEVICE9 pDevice);
public:
	~CPattern1();



};


#endif // CutterPattern_h__