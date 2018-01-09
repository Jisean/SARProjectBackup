#include "stdafx.h"
#include "Ending.h"
#include "SceneSelector.h"
#include "Texture.h"

#include "Export_Function.h"
#include "ResourcesMgr.h"
#include "Layer.h"
//#include "Score.h"
#include "CollisionMgr.h"
#include "EndScore.h"
#include "TextureMgr.h"

CEnding::CEnding(LPDIRECT3DDEVICE9 pDevice)
: ENGINE::CScene(pDevice)
, m_pResourceMgr(ENGINE::Get_ResourcesMgr())
, m_pManagement(ENGINE::Get_Management())
, m_pGraphicDev(ENGINE::Get_GraphicDev())
{

}

CEnding::~CEnding(void)
{
	Release();
}

HRESULT CEnding::Initialize(void)
{

	HRESULT	hr = NULL;

	ENGINE::CComponent*			pComponent = NULL;

	// Buffer
	hr = m_pResourceMgr->AddBuffer(m_pDevice, ENGINE::RESOURCE_DYNAMIC, 
		ENGINE::CVIBuffer::BUFFER_RCTEX, L"Buffer_RcTex");
	FAILED_CHECK_MSG(hr, L"Buffer_RcTex CREATE FAILED");

	// 스코어
	hr = m_pResourceMgr->AddTexture(m_pDevice, ENGINE::RESOURCE_DYNAMIC, 
		ENGINE::TEX_NORMAL, L"Texture_Score", L"../bin/Resources/Texture/Ending/%d.png", 1);

	FAILED_CHECK_MSG(Add_GameLogic_Layer(), L"GameLogic 초기화 실패");

	CSoundMgr::GetInstance()->PlayBgm(L"Ending.mp3");

	
	return S_OK;
}
CEnding* CEnding::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CEnding* pEnding = new CEnding(pDevice);

	if(FAILED(pEnding->Initialize()))
		ENGINE::Safe_Delete(pEnding);

	return pEnding;
}

void CEnding::Update(void)
{
	ENGINE::CScene::Update();
}

void CEnding::Render(void)
{
	ENGINE::CScene::Render();
	const TEXINFO* pTexture2 = CTextureMgr::GetInstance()->GetTexture(L"ScoreNumber", L"ScoreNumber", iScore / 1000);
	const TEXINFO* pTexture3 = CTextureMgr::GetInstance()->GetTexture(L"ScoreNumber", L"ScoreNumber", (iScore / 100) % 10);
	const TEXINFO* pTexture4 = CTextureMgr::GetInstance()->GetTexture(L"ScoreNumber", L"ScoreNumber", (iScore / 10) % 10);
	const TEXINFO* pTexture5 = CTextureMgr::GetInstance()->GetTexture(L"ScoreNumber", L"ScoreNumber", iScore % 10);

	if(pTexture2 == NULL)
		return;

	float	fX2 = pTexture2->tImgInfo.Width / 2.f;
	float	fY2 = pTexture2->tImgInfo.Height / 2.f;
	

	D3DXMATRIX		matTrans2, matScale;

	if(pTexture3 == NULL)
		return;

	float	fX3 = pTexture3->tImgInfo.Width / 2.f;
	float	fY3 = pTexture3->tImgInfo.Height / 2.f;

	D3DXMATRIX		matTrans3;

	if(pTexture4 == NULL)
		return;

	float	fX4 = pTexture4->tImgInfo.Width / 2.f;
	float	fY4 = pTexture4->tImgInfo.Height / 2.f;

	D3DXMATRIX		matTrans4 ;

	if(pTexture5 == NULL)
		return;

	float	fX5 = pTexture5->tImgInfo.Width / 2.f;
	float	fY5 = pTexture5->tImgInfo.Height / 2.f;

	D3DXMATRIX		matTrans5;

	m_pGraphicDev->Render_Begin();

	D3DXMatrixScaling(&matScale, 1.5f, 1.5f, 0);
	if(iScore / 1000 != 0)
	{
		
		D3DXMatrixTranslation(&matTrans2, WINCX - 300.f + 45, WINCY / 2.f - 115.f, 0.f);
		matTrans2 = matScale * matTrans2;
		m_pGraphicDev->GetSprite()->SetTransform(&matTrans2);
		m_pGraphicDev->GetSprite()->Draw(pTexture2->pTexture, 
			NULL, &D3DXVECTOR3(fX2, fY2, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	if(iScore / 100 != 0)
	{
		D3DXMatrixTranslation(&matTrans3, WINCX - 300.f + 85, WINCY / 2.f - 115.f, 0.f);
		matTrans3 = matScale * matTrans3;
		m_pGraphicDev->GetSprite()->SetTransform(&matTrans3);
		m_pGraphicDev->GetSprite()->Draw(pTexture3->pTexture, 
			NULL, &D3DXVECTOR3(fX3, fY3, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	if(iScore / 10 != 0)
	{
		
		D3DXMatrixTranslation(&matTrans4, WINCX - 300.f + 125, WINCY / 2.f - 115.f, 0.f);
		matTrans4 = matScale * matTrans4;
		m_pGraphicDev->GetSprite()->SetTransform(&matTrans4);
		m_pGraphicDev->GetSprite()->Draw(pTexture4->pTexture, 
			NULL, &D3DXVECTOR3(fX4, fY4, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	D3DXMatrixTranslation(&matTrans5, WINCX - 300.f + 165, WINCY / 2.f - 115.f, 0.f);
	matTrans5 = matScale * matTrans5;
	m_pGraphicDev->GetSprite()->SetTransform(&matTrans5);
	m_pGraphicDev->GetSprite()->Draw(pTexture5->pTexture, 
		NULL, &D3DXVECTOR3(fX5, fY5, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));


	////////////////////
	if(iScore / 1000 != 0)
	{
		
		D3DXMatrixTranslation(&matTrans2, WINCX - 300.f + 45, WINCY - 40.f, 0.f);
		matTrans2 = matScale * matTrans2;
		m_pGraphicDev->GetSprite()->SetTransform(&matTrans2);
		m_pGraphicDev->GetSprite()->Draw(pTexture2->pTexture, 
			NULL, &D3DXVECTOR3(fX2, fY2, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	if(iScore / 100 != 0)
	{
		D3DXMatrixTranslation(&matTrans3, WINCX - 300.f + 85, WINCY - 40.f, 0.f);
		matTrans3 = matScale * matTrans3;
		m_pGraphicDev->GetSprite()->SetTransform(&matTrans3);
		m_pGraphicDev->GetSprite()->Draw(pTexture3->pTexture, 
			NULL, &D3DXVECTOR3(fX3, fY3, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	if(iScore / 10 != 0)
	{
		
		D3DXMatrixTranslation(&matTrans4, WINCX - 300.f + 125, WINCY - 40.f, 0.f);
		matTrans4 = matScale * matTrans4;
		m_pGraphicDev->GetSprite()->SetTransform(&matTrans4);
		m_pGraphicDev->GetSprite()->Draw(pTexture4->pTexture, 
			NULL, &D3DXVECTOR3(fX4, fY4, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	D3DXMatrixTranslation(&matTrans5, WINCX - 300.f + 165, WINCY - 40.f, 0.f);
	matTrans5 = matScale * matTrans5;
	m_pGraphicDev->GetSprite()->SetTransform(&matTrans5);
	m_pGraphicDev->GetSprite()->Draw(pTexture5->pTexture, 
		NULL, &D3DXVECTOR3(fX5, fY5, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

	m_pGraphicDev->Render_End();
}

HRESULT CEnding::Add_Environment_Layer(void)
{
	return S_OK;
}

HRESULT CEnding::Add_GameLogic_Layer(void)
{
	ENGINE::CLayer*		pLayer = ENGINE::CLayer::Create(m_pDevice);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	ENGINE::CGameObject*		pGameObject = NULL;
	pGameObject = CEndScore::Create(m_pDevice);
	pLayer->AddObject(L"Score", pGameObject);

	m_mapLayer.insert(MAPLAYER::value_type(LAYER_GAMELOGIC, pLayer));	

	return S_OK;
}

void CEnding::Release(void)
{
	m_pResourceMgr->ResetDynamic();
}

void CEnding::SetUp(void)
{

}
