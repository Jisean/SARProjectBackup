#include "stdafx.h"
#include "ShotTimer.h"

#include "Export_Function.h"
#include "Texture.h"
#include "TextureMgr.h"

CShotTimer::CShotTimer(LPDIRECT3DDEVICE9 pDevice)
: ENGINE::CGameObject(pDevice)
, m_pGraphicDev(ENGINE::Get_GraphicDev())
, m_pTimer(ENGINE::Get_Time())
, m_fFrameCnt(0.f)
, m_fFrameMax(0.f)
, m_bShowStart(false)
, m_bShowOver(false)
, m_bTemp(false)
{
	//
}

CShotTimer::~CShotTimer(void)
{
	Release();
}
CShotTimer* CShotTimer::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CShotTimer*		pLogoBack = new CShotTimer(pDevice);
	if(FAILED(pLogoBack->Initialize()))
		ENGINE::Safe_Delete(pLogoBack);

	return pLogoBack;
}

void CShotTimer::Update(void)
{
	m_fTime += m_pTimer->GetTime();
	m_fTimeStart += m_pTimer->GetTime();
	
	if(m_fTime > 0.01)
	{
		m_fTime = 0;
		iTimeLimit -= 10;
		if(iTimeLimit <= 0)
			iTimeLimit = 0;
	}
	if(m_fTimeStart < 3.f)
	{
		m_bShowStart = true;
	}
	else
	{
		m_bShowStart = false;
	}
	if(iTimeLimit == 0.f)
	{		
		m_bShowOver = true;
	}
	if(m_bTemp == false && iTimeLimit <= 0)
	{
		CSoundMgr::GetInstance()->PlaySkillSound(L"run_timeover_kor.wav");
		m_bTemp = true;
	}
	

}

void CShotTimer::Render(void)
{
	//const TEXINFO* pTexture = CTextureMgr::GetInstance()->GetTexture(L"LOGO");
	const TEXINFO* pTexture = CTextureMgr::GetInstance()->GetTexture(L"Time");
	const TEXINFO* pTexture2 = CTextureMgr::GetInstance()->GetTexture(L"TimerNumber", L"TimerNumber", iTimeLimit / 10000);
	const TEXINFO* pTexture3 = CTextureMgr::GetInstance()->GetTexture(L"TimerNumber", L"TimerNumber", (iTimeLimit / 1000) % 10);
	const TEXINFO* pTexture4 = CTextureMgr::GetInstance()->GetTexture(L"Colone");
	const TEXINFO* pTexture5 = CTextureMgr::GetInstance()->GetTexture(L"TimerNumber", L"TimerNumber", (iTimeLimit / 100) % 10);
	const TEXINFO* pTexture6 = CTextureMgr::GetInstance()->GetTexture(L"TimerNumber", L"TimerNumber", (iTimeLimit / 10) % 10);

	const TEXINFO* pTextureStart = CTextureMgr::GetInstance()->GetTexture(L"TimeStart");
	const TEXINFO* pTextureOver = CTextureMgr::GetInstance()->GetTexture(L"TimeOver");

	if(pTextureStart == NULL)
		return;

	float	fXStart = pTextureStart->tImgInfo.Width / 2.f;
	float	fYStart = pTextureStart->tImgInfo.Height / 2.f;

	D3DXMATRIX		matTransStart;

	if(pTextureOver == NULL)
		return;

	float	fXOver = pTextureOver->tImgInfo.Width / 2.f;
	float	fYOver = pTextureOver->tImgInfo.Height / 2.f;

	D3DXMATRIX		matTransOver;


	if(pTexture == NULL)
		return;

	float	fX = pTexture->tImgInfo.Width / 2.f;
	float	fY = pTexture->tImgInfo.Height / 2.f;

	D3DXMATRIX		matTrans;

	if(pTexture2 == NULL)
		return;

	float	fX2 = pTexture2->tImgInfo.Width / 2.f;
	float	fY2 = pTexture2->tImgInfo.Height / 2.f;

	D3DXMATRIX		matTrans2;

	if(pTexture3 == NULL)
		return;

	float	fX3 = pTexture3->tImgInfo.Width / 2.f;
	float	fY3 = pTexture3->tImgInfo.Height / 2.f;

	D3DXMATRIX		matTrans3;

	if(pTexture4 == NULL)
		return;

	float	fX4 = pTexture4->tImgInfo.Width / 2.f;
	float	fY4 = pTexture4->tImgInfo.Height / 2.f;

	D3DXMATRIX		matTrans4;

	if(pTexture5 == NULL)
		return;

	float	fX5 = pTexture5->tImgInfo.Width / 2.f;
	float	fY5 = pTexture5->tImgInfo.Height / 2.f;

	D3DXMATRIX		matTrans5;

	if(pTexture6 == NULL)
		return;

	float	fX6 = pTexture6->tImgInfo.Width / 2.f;
	float	fY6 = pTexture6->tImgInfo.Height / 2.f;

	D3DXMATRIX		matTrans6;

	m_pGraphicDev->Render_Begin();

	D3DXMatrixTranslation(&matTrans, WINCX / 2.f - 90.f, WINCY / 2.f - 250.f, 0.f);
	m_pGraphicDev->GetSprite()->SetTransform(&matTrans);
	m_pGraphicDev->GetSprite()->Draw(pTexture->pTexture, 
		NULL, &D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

	D3DXMatrixTranslation(&matTrans2, WINCX / 2.f - 10.f, WINCY / 2.f - 250.f, 0.f);
	m_pGraphicDev->GetSprite()->SetTransform(&matTrans2);
	m_pGraphicDev->GetSprite()->Draw(pTexture2->pTexture, 
		NULL, &D3DXVECTOR3(fX2, fY2, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

	D3DXMatrixTranslation(&matTrans3, WINCX / 2.f + 20.f, WINCY / 2.f - 250.f, 0.f);
	m_pGraphicDev->GetSprite()->SetTransform(&matTrans3);
	m_pGraphicDev->GetSprite()->Draw(pTexture3->pTexture, 
		NULL, &D3DXVECTOR3(fX3, fY3, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

	D3DXMatrixTranslation(&matTrans4, WINCX / 2.f + 50.f, WINCY / 2.f - 250.f, 0.f);
	m_pGraphicDev->GetSprite()->SetTransform(&matTrans4);
	m_pGraphicDev->GetSprite()->Draw(pTexture4->pTexture, 
		NULL, &D3DXVECTOR3(fX4, fY4, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

	D3DXMatrixTranslation(&matTrans5, WINCX / 2.f + 80.f, WINCY / 2.f - 250.f, 0.f);
	m_pGraphicDev->GetSprite()->SetTransform(&matTrans5);
	m_pGraphicDev->GetSprite()->Draw(pTexture5->pTexture, 
		NULL, &D3DXVECTOR3(fX5, fY5, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

	D3DXMatrixTranslation(&matTrans6, WINCX / 2.f + 110.f, WINCY / 2.f - 250.f, 0.f);
	m_pGraphicDev->GetSprite()->SetTransform(&matTrans6);
	m_pGraphicDev->GetSprite()->Draw(pTexture6->pTexture, 
		NULL, &D3DXVECTOR3(fX6, fY6, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));


	if(m_bShowStart == true)
	{
		D3DXMatrixTranslation(&matTransStart, WINCX / 2.f, WINCY / 2.f, 0.f);
		m_pGraphicDev->GetSprite()->SetTransform(&matTransStart);
		m_pGraphicDev->GetSprite()->Draw(pTextureStart->pTexture, 
			NULL, &D3DXVECTOR3(fXStart, fYStart, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	if(m_bShowOver == true)
	{
		D3DXMatrixTranslation(&matTransOver, WINCX / 2.f, WINCY / 2.f, 0.f);
		m_pGraphicDev->GetSprite()->SetTransform(&matTransOver);
		m_pGraphicDev->GetSprite()->Draw(pTextureOver->pTexture, 
			NULL, &D3DXVECTOR3(fXOver, fYOver, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	m_pGraphicDev->Render_End();
}

HRESULT CShotTimer::Initialize(void)
{
	m_fFrameMax = 37.f;
	m_fTime = 0.f;
	m_fTimeStart = 0.f;
	
	return S_OK;
}


void CShotTimer::Release(void)
{

}



