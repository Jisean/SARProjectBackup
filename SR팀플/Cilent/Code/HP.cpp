#include "stdafx.h"
#include "HP.h"

#include "Export_Function.h"
#include "Texture.h"
#include "Include.h"

CHP::CHP(LPDIRECT3DDEVICE9 pDevice)
: ENGINE::CGameObject(pDevice)
, m_pResourceMgr(ENGINE::Get_ResourcesMgr())
{
	m_DigitType = HUNDRED;
}

CHP:: ~CHP(void)
{
	Release();
}
HRESULT  CHP::	Initialize(void)
{
	FAILED_CHECK(AddComponent());

	return S_OK;
}
CHP* CHP::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CHP*	pCrossLine = new CHP(pDevice);
	if(FAILED(pCrossLine->Initialize()))
		ENGINE::Safe_Delete(pCrossLine);

	return pCrossLine;
}
HRESULT  CHP::	AddComponent(void)
{
	ENGINE::CComponent*			pComponent = NULL;

	// Buffer
	pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_STATIC, L"Buffer_RcUI");
	m_pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_STATIC, L"Texture_HundredNumbers");
	m_pTexture[0] = dynamic_cast<ENGINE::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_STATIC, L"Texture_TenNumbers");
	m_pTexture[1] = dynamic_cast<ENGINE::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_STATIC, L"Texture_Numbers");
	m_pTexture[2] = dynamic_cast<ENGINE::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	return S_OK;

}
void CHP::Update(void)
{
	/*if(GetAsyncKeyState('0'))
	{
		fHp -= 1.f;
		if(fHp <= 0)
			fHp = 0;
	}*/
}
void CHP::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00000088);
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//m_pResourceMgr->SetVtxInfo(ENGINE::RESOURCE_STATIC, L"Buffer_RcTex", m_pConvertVertex);

	if(m_DigitType == HUNDRED)
	{
		if(fHp == 100)
			m_pTexture[0]->Render(1);
	}
	else if(m_DigitType == TEN)
	{
		int iTemp = (int)fHp / 10;
		if(iTemp == 10)
			m_pTexture[1]->Render(0);
		else if(iTemp > 0)
			m_pTexture[1]->Render(iTemp);
		else
		{
			//아무것도 출력하지 않는다.
		}
	}
	else if(m_DigitType == ONE)
	{
		int iTemp = (int)fHp % 10;
		if(iTemp == 0)
			m_pTexture[2]->Render(0);
		else
			m_pTexture[2]->Render(iTemp);
	}
	m_pBuffer->Render();

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}
void  CHP::	Release(void)
{

}





