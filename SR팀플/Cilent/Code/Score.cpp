#include "stdafx.h"
#include "Score.h"

#include "Export_Function.h"
#include "Texture.h"
#include "TextureMgr.h"

CScore::CScore(LPDIRECT3DDEVICE9 pDevice)
: ENGINE::CGameObject(pDevice)
, m_pGraphicDev(ENGINE::Get_GraphicDev())
, m_pTimer(ENGINE::Get_Time())
, m_fFrameCnt(0.f)
, m_fFrameMax(0.f)
{

}

CScore::~CScore(void)
{
	Release();
}
CScore* CScore::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CScore*		pLogoBack = new CScore(pDevice);
	if(FAILED(pLogoBack->Initialize()))
		ENGINE::Safe_Delete(pLogoBack);

	return pLogoBack;
}

void CScore::Update(void)
{
	
}

void CScore::Render(void)
{
	const TEXINFO* pTexture = CTextureMgr::GetInstance()->GetTexture(L"Score");
	const TEXINFO* pTexture2 = CTextureMgr::GetInstance()->GetTexture(L"ScoreNumber", L"ScoreNumber", iScore / 1000);
	const TEXINFO* pTexture3 = CTextureMgr::GetInstance()->GetTexture(L"ScoreNumber", L"ScoreNumber", (iScore / 100) % 10);
	const TEXINFO* pTexture4 = CTextureMgr::GetInstance()->GetTexture(L"ScoreNumber", L"ScoreNumber", (iScore / 10) % 10);
	const TEXINFO* pTexture5 = CTextureMgr::GetInstance()->GetTexture(L"ScoreNumber", L"ScoreNumber", iScore % 10);

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

	m_pGraphicDev->Render_Begin();

	D3DXMatrixTranslation(&matTrans, WINCX / 2.f - 350.f, WINCY / 2.f - 150.f, 0.f);
	m_pGraphicDev->GetSprite()->SetTransform(&matTrans);
	m_pGraphicDev->GetSprite()->Draw(pTexture->pTexture, 
		NULL, &D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

	if(iScore / 1000 != 0)
	{
		D3DXMatrixTranslation(&matTrans2, WINCX / 2.f - 350.f + 50, WINCY / 2.f - 150.f, 0.f);
		m_pGraphicDev->GetSprite()->SetTransform(&matTrans2);
		m_pGraphicDev->GetSprite()->Draw(pTexture2->pTexture, 
			NULL, &D3DXVECTOR3(fX2, fY2, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	if(iScore / 100 != 0)
	{
		D3DXMatrixTranslation(&matTrans3, WINCX / 2.f - 350.f + 70, WINCY / 2.f - 150.f, 0.f);
		m_pGraphicDev->GetSprite()->SetTransform(&matTrans3);
		m_pGraphicDev->GetSprite()->Draw(pTexture3->pTexture, 
			NULL, &D3DXVECTOR3(fX3, fY3, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	if(iScore / 10 != 0)
	{
		D3DXMatrixTranslation(&matTrans4, WINCX / 2.f - 350.f + 90, WINCY / 2.f - 150.f, 0.f);
		m_pGraphicDev->GetSprite()->SetTransform(&matTrans4);
		m_pGraphicDev->GetSprite()->Draw(pTexture4->pTexture, 
			NULL, &D3DXVECTOR3(fX4, fY4, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	D3DXMatrixTranslation(&matTrans5, WINCX / 2.f - 350.f + 110, WINCY / 2.f - 150.f, 0.f);
	m_pGraphicDev->GetSprite()->SetTransform(&matTrans5);
	m_pGraphicDev->GetSprite()->Draw(pTexture5->pTexture, 
		NULL, &D3DXVECTOR3(fX5, fY5, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

	m_pGraphicDev->Render_End();
}

HRESULT CScore::Initialize(void)
{
	m_fFrameMax = 37.f;
	m_fTime = 0.f;

	return S_OK;
}


void CScore::Release(void)
{

}



