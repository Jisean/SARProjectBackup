#include "Management.h"

IMPLEMENT_SINGLETON(ENGINE::CManagement)

ENGINE::CManagement::CManagement(void)
:m_pDevice(NULL),
m_pRenderer(NULL),
m_pScene(NULL)
{

}

ENGINE::CManagement::~CManagement(void)
{
	Release();
}

HRESULT ENGINE::CManagement::InitManagement(LPDIRECT3DDEVICE9 pDevice)
{
	m_pDevice = pDevice;

	m_pRenderer = CRenderer::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(m_pRenderer, E_FAIL, L"Renderer Create Failed");


	return S_OK;
}

void ENGINE::CManagement::Update(void)
{
	if(m_pScene != NULL)
		m_pScene->Update();
}

void ENGINE::CManagement::Render(float fTime)
{
	if(m_pRenderer != NULL)
		m_pRenderer->Render(fTime);
}

void ENGINE::CManagement::Release(void)
{
	ENGINE::Safe_Delete(m_pScene);
	ENGINE::Safe_Delete(m_pRenderer);
}

const ENGINE::VTXTEX* ENGINE::CManagement::GetTerrainVertex(const WORD& LayerID, 
													const wstring& wstrObjKey)
{
	return m_pScene->GetTerrainVertex(LayerID, wstrObjKey);
}

const ENGINE::VTXCUBE* ENGINE::CManagement::GetCubeVertex(const WORD& LayerID, 
														  const wstring& wstrObjKey)
{
	return m_pScene->GetCubeVertex(LayerID, wstrObjKey);
}

const D3DXVECTOR3* ENGINE::CManagement::GetPos(const WORD& LayerID, const wstring& wstrObjKey)
{
	return m_pScene->GetPos(LayerID,wstrObjKey);
}

list<ENGINE::CGameObject*>*	ENGINE::CManagement::GetList(const WORD& LayerID, const wstring&	wstrObjKey)
{
	return m_pScene->GetList(LayerID,wstrObjKey);
}
