#include "stdafx.h"
#include "EndScore.h"

#include "Export_Function.h"
#include "Texture.h"


CEndScore::CEndScore(LPDIRECT3DDEVICE9 pDevice)
: ENGINE::CGameObject(pDevice)
, m_pResourceMgr(ENGINE::Get_ResourcesMgr())
{

}

CEndScore::~CEndScore(void)
{
	Release();
}
HRESULT CEndScore::Initialize(void)
{
	FAILED_CHECK(AddComponent());	

	return S_OK;
}
CEndScore* CEndScore::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CEndScore*		pEndScore = new CEndScore(pDevice);
	if(FAILED(pEndScore->Initialize()))
		ENGINE::Safe_Delete(pEndScore);

	return pEndScore;
}

void CEndScore::Update(void)
{

}

void CEndScore::Render(void)
{
	m_pTexture->Render(0);

	m_pBuffer->Render();
}

HRESULT CEndScore::AddComponent(void)
{
	ENGINE::CComponent*			pComponent = NULL;

	// Buffer
	pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, L"Buffer_RcTex");
	m_pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	// Texture
	pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, L"Texture_Score");
	m_pTexture = dynamic_cast<ENGINE::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	return S_OK;
}

void CEndScore::Release(void)
{

}

