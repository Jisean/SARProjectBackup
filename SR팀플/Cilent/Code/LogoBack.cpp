#include "stdafx.h"
#include "LogoBack.h"

#include "Export_Function.h"
#include "Texture.h"

CLogoBack::CLogoBack(LPDIRECT3DDEVICE9 pDevice)
: ENGINE::CGameObject(pDevice)
, m_pResourceMgr(ENGINE::Get_ResourcesMgr())
, m_pTimeMgr(ENGINE::Get_Time())
, m_fFrameCnt(0.f)
, m_fFrameMax(0.f)
{

}

CLogoBack::~CLogoBack(void)
{
	Release();
}
CLogoBack* CLogoBack::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CLogoBack*		pLogoBack = new CLogoBack(pDevice);
	if(FAILED(pLogoBack->Initialize()))
		ENGINE::Safe_Delete(pLogoBack);

	return pLogoBack;
}

void CLogoBack::Update(void)
{
	m_fFrameCnt += m_fFrameMax * m_pTimeMgr->GetTime() * 0.5f;

	if(m_fFrameCnt > m_fFrameMax)
		m_fFrameCnt = 0.f;

	
}
HRESULT CLogoBack::AddComponent(void)
{
	ENGINE::CComponent*			pComponent = NULL;

	// Buffer
	pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, L"Buffer_RcTex");
	m_pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	// Texture
	pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, L"Texture_Logo");
	m_pTexture = dynamic_cast<ENGINE::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	return S_OK;
}

void CLogoBack::Render(void)
{
	/*MAPCOMPONENT::iterator		iter = m_mapComponent.find(L"Texture");
	((ENGINE::CTexture*)iter->second)->Render(0);

	iter = m_mapComponent.find(L"Buffer");
	((ENGINE::CVIBuffer*)iter->second)->Render();*/

	m_pTexture->Render(WORD(m_fFrameCnt));

	m_pBuffer->Render();
}

HRESULT CLogoBack::Initialize(void)
{
	FAILED_CHECK(AddComponent());	

	m_fFrameMax = 37.f;
	
	return S_OK;
}


void CLogoBack::Release(void)
{

}



