#include "stdafx.h"
#include "ScreenEffect.h"

#include "Export_Function.h"
#include "Texture.h"
#include "Include.h"

CScreenEffect::CScreenEffect(LPDIRECT3DDEVICE9 pDevice)
: ENGINE::CGameObject(pDevice)
, m_pResourceMgr(ENGINE::Get_ResourcesMgr())
, m_pTimeMgr(ENGINE::Get_Time())
, test1(false)
, test2(false)
{

}

CScreenEffect::~CScreenEffect(void)
{
	Release();
}
HRESULT CScreenEffect::Initialize(void)
{
	m_fFrameCnt = 0.f;
	m_fFrameMax = 0.f;
	m_fFrameSpeed = 0.5f;
	FAILED_CHECK(AddComponent());

	return S_OK;
}
CScreenEffect* CScreenEffect::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CScreenEffect*	pScreenEffect = new CScreenEffect(pDevice);
	if(FAILED(pScreenEffect->Initialize()))
		ENGINE::Safe_Delete(pScreenEffect);

	return pScreenEffect;
}

void CScreenEffect::Update(void)
{
	if(test1)
	{
		m_wImage = L"Texture_Blood";
		m_fFrameMax = 23.f;
		m_fFrameCnt += m_fFrameMax * m_pTimeMgr->GetTime() * m_fFrameSpeed;

		if(m_fFrameCnt > m_fFrameMax)
		{
			m_fFrameCnt = 0.f;
			CSoundMgr::GetInstance()->Monster1Sound(L"run_immortal");
		}
	}
	if(test2)
	{
		m_wImage = L"Texture_Unbeatable";
		m_fFrameMax = 19.f;
		m_fFrameCnt += m_fFrameMax * m_pTimeMgr->GetTime() * m_fFrameSpeed;

		if(m_fFrameCnt > m_fFrameMax)
		{
			m_fFrameCnt = 0.f;
		}
	}	
}

void CScreenEffect::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00000088);
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//m_pResourceMgr->SetVtxInfo(ENGINE::RESOURCE_STATIC, L"Buffer_RcTex", m_pConvertVertex);

	if(fHp < 20)
	{
		test1 = true;
		test2 = false;
	}
	else if(fHp >= 20)
	{
		test1 = false;
	}
	if(bRedBull)
	{
		test1 = false;
		test2 = true;
	}
	else
	{
		test2 = false;
	}

	if(test1)
		m_pTexture[1]->Render(WORD(m_fFrameCnt));

	if(test2)
		m_pTexture[2]->Render(WORD(m_fFrameCnt));

	m_pBuffer->Render();

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

HRESULT CScreenEffect::AddComponent(void)
{
	ENGINE::CComponent*			pComponent = NULL;

	// Buffer
	pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_STATIC, L"Buffer_RcUI");
	m_pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_STATIC, L"Texture_Blood");
	m_pTexture[1] = dynamic_cast<ENGINE::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_STATIC, L"Texture_Unbeatable");
	m_pTexture[2] = dynamic_cast<ENGINE::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	return S_OK;
}

void CScreenEffect::Release(void)
{

}


