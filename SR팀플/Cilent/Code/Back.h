/*!
 * \file Back.h
 * \date 2016/02/16 19:13
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


#ifndef Back_h__
#define Back_h__


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
class CBack : public ENGINE::CGameObject
{
private:
	ENGINE::CResourcesMgr*	m_pResourceMgr;
	ENGINE::CManagement*	m_pManagement;
	ENGINE::CTimeMgr*		m_pTimeMgr;

	ENGINE::CVIBuffer*		m_pBuffer;
	ENGINE::CTexture*		m_pTexture;

public:
	ENGINE::CTransform*		m_pInfo;

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
	static CBack*	Create(LPDIRECT3DDEVICE9 pDevice);
	const ENGINE::VTXTEX*	GetTerrainVertex();

	void		Update(void);
	void		Render(void);

	map<wstring, ENGINE::CComponent*>* GetmapComponent(void);
	void SetTexture(ENGINE::CTexture* pTexture);

private:
	explicit CBack(LPDIRECT3DDEVICE9 pDevice);
public:
	~CBack();
};


#endif // Back_h__