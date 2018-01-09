/*!
 * \file Terrain.h
 * \date 2016/01/14 14:01
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

#ifndef Terrain_h__
#define Terrain_h__

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
class CTerrain : public ENGINE::CGameObject
{
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

	float				m_fCameraAngle;
private:
	HRESULT		Initialize(void);
	HRESULT		AddComponent(void);
	void		Release(void);
	void		SetTransForm(void);
	
public:
	static CTerrain*	Create(LPDIRECT3DDEVICE9 pDevice);
	const ENGINE::VTXTEX*	GetTerrainVertex();

	map<wstring, ENGINE::CComponent*>*	GetmapComponent(void);
	void	SetTexture(ENGINE::CTexture* pTexture);

	void		Update(void);
	void		Render(void);
	

private:
	explicit CTerrain(LPDIRECT3DDEVICE9 pDevice);
public:
	~CTerrain();
};

#endif // Terrain_h__