#include "stdafx.h"
#include "CrossLine.h"

#include "Export_Function.h"
#include "Texture.h"

CCrossLine::CCrossLine(LPDIRECT3DDEVICE9 pDevice)
: ENGINE::CGameObject(pDevice)
, m_pResourceMgr(ENGINE::Get_ResourcesMgr())
{

}

CCrossLine:: ~CCrossLine(void)
{
	Release();
}
HRESULT  CCrossLine::	Initialize(void)
{
	FAILED_CHECK(AddComponent());

	return S_OK;
}
CCrossLine* CCrossLine::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CCrossLine*	pCrossLine = new CCrossLine(pDevice);
	if(FAILED(pCrossLine->Initialize()))
		ENGINE::Safe_Delete(pCrossLine);

	return pCrossLine;
}
HRESULT  CCrossLine::	AddComponent(void)
{
	ENGINE::CComponent*			pComponent = NULL;

	// Buffer
	pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_STATIC, L"Buffer_RcUI");
	m_pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_STATIC, L"Texture_Back");
	m_pTexture = dynamic_cast<ENGINE::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	return S_OK;

}
void CCrossLine::Update(void)
{

}
void CCrossLine::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00000088);
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//m_pResourceMgr->SetVtxInfo(ENGINE::RESOURCE_STATIC, L"Buffer_RcTex", m_pConvertVertex);

	m_pTexture->Render(0);

	m_pBuffer->Render();

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}
void  CCrossLine::	Release(void)
{

}





