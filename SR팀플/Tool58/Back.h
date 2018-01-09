#pragma once
#include "obj.h"

class CTool58View;
class CCubeObj;
class CStaticCamera;
class CCollisionMgr;
class CTerrain;
class CMouseCol;
namespace ENGINE
{
	class CTransform;
	class CInfoSubject;
}

class CObstacle;
class CCreateCube;
class CBack :
	public CObj
{
private:
	vector<TILE*>		m_vecTile;
	CTool58View*		m_pMainView;
	LPDIRECT3DDEVICE9	m_Device;

	CStaticCamera*		m_pStaticCamera;
	CCollisionMgr*		m_pCollisionMgr;
	D3DXVECTOR3				m_vDestPos;
	CMouseCol*				m_pMouseCol;

	CCreateCube*			m_pCreateCube;

private:
	ENGINE::CInfoSubject*	m_pInfoSubject;

public:
	static vector<CObstacle*>	m_vecCube;
	static	CObstacle*			m_pCubeForDel;
	static ENGINE::CTransform*	m_pTransform;
	static CTerrain*			m_pTerrain;
	static bool					bIsPressed;
	static bool					m_bSelRadio;

public:
	static void MakeACube(D3DXVECTOR3 vPos);

public:
	void SetMainView(CTool58View* pMainView);

	void MiniViewRender(void);	

	const vector<TILE*>*		GetTile(void);

public:
	CBack(void);
	CBack(LPDIRECT3DDEVICE9 pDevice);
	~CBack(void);

public:
	virtual HRESULT Initialize(void);
	virtual int Progress(void);
	virtual void Render(void);
	virtual void Release(void);

};
