/*!
 * \file Management.h
 * \date 2016/01/06 14:29
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

#ifndef Management_h__
#define Management_h__

#include "Engine_Include.h"

#include "Renderer.h"
#include "Scene.h"

BEGIN(ENGINE)

class ENGINE_DLL CManagement
{
	DECLARE_SINGLETON(CManagement)
private:
	LPDIRECT3DDEVICE9		m_pDevice;
	CRenderer*				m_pRenderer;
	CScene*					m_pScene;

public:
	HRESULT	InitManagement(LPDIRECT3DDEVICE9 pDevice);
	void	Update(void);
	void	Render(float fTime);

public:
	template<typename T>
	HRESULT	SceneChange(T& Functor);

	const VTXTEX* GetTerrainVertex(const WORD& LayerID, const wstring& wstrObjKey);
	const VTXCUBE*	GetCubeVertex(const WORD& LayerID, const wstring&	wstrObjKey);
	const D3DXVECTOR3* GetPos(const WORD& LayerID, const wstring&	wstrObjKey);
	list<CGameObject*>*	GetList(const WORD& LayerID, const wstring&	wstrObjKey);
private:
	void Release(void);

private:
	CManagement(void);
	~CManagement(void);
};

template<typename T>
HRESULT ENGINE::CManagement::SceneChange(T& Functor)
{
	if(m_pScene != NULL)
		ENGINE::Safe_Delete(m_pScene);

	FAILED_CHECK(Functor(&m_pScene, m_pDevice));

	m_pRenderer->SetScene(m_pScene);

	return S_OK;
}

END

#endif // Management_h__