#include "stdafx.h"
#include "BulletCnt.h"

#include "Export_Function.h"
#include "Texture.h"
#include "Include.h"

GUNTYPE CBulletCnt::m_GunType = KNIFE;
CBulletCnt::CBulletCnt(LPDIRECT3DDEVICE9 pDevice)
: ENGINE::CGameObject(pDevice)
, m_pResourceMgr(ENGINE::Get_ResourcesMgr())
{
	
}

CBulletCnt:: ~CBulletCnt(void)
{
	Release();
}
HRESULT  CBulletCnt::	Initialize(void)
{
	FAILED_CHECK(AddComponent());

	return S_OK;
}
CBulletCnt* CBulletCnt::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CBulletCnt*	pCrossLine = new CBulletCnt(pDevice);
	if(FAILED(pCrossLine->Initialize()))
		ENGINE::Safe_Delete(pCrossLine);

	return pCrossLine;
}
HRESULT  CBulletCnt::	AddComponent(void)
{
	ENGINE::CComponent*			pComponent = NULL;

	// Buffer
	pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_STATIC, L"Buffer_RcUI");
	m_pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_STATIC, L"Texture_BTenNumbers");
	m_pTexture[0] = dynamic_cast<ENGINE::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_STATIC, L"Texture_BNumbers");
	m_pTexture[1] = dynamic_cast<ENGINE::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	return S_OK;

}
void CBulletCnt::Update(void)
{

}
void CBulletCnt::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00000088);
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//m_pResourceMgr->SetVtxInfo(ENGINE::RESOURCE_STATIC, L"Buffer_RcTex", m_pConvertVertex);

	if(m_GunType == KNIFE)
	{
		//m_pTexture[1]->Render(0);
	}
	else if(m_GunType == PISTOL)
	{
		if(m_eDigit == BTEN)
		{
			int iTemp1 = iPistolBullet / 10;
			if(iTemp1 == 0)
			{
				//아무것도 출력하지 않는다.
			}
			else
			{
				m_pTexture[0]->Render(iTemp1);
			}
		}
		else if(m_eDigit == BONE)
		{
			int iTemp2 = iPistolBullet % 10;
			{
				m_pTexture[1]->Render(iTemp2);
			}
		}
	}
	else if(m_GunType == RIFLE)
	{
		if(m_eDigit == BTEN)
		{
			int iTemp1 = iRifleBullet / 10;
			if(iTemp1 == 0)
			{
				//아무것도 출력하지 않는다.
			}
			else
			{
				m_pTexture[0]->Render(iTemp1);
			}
		}
		else if(m_eDigit == BONE)
		{
			int iTemp2 = iRifleBullet % 10;
			{
				m_pTexture[1]->Render(iTemp2);
			}
		}
	}
	m_pBuffer->Render();

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}
void  CBulletCnt::	Release(void)
{

}





