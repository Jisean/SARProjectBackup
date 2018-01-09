#include "stdafx.h"
#include "PlayerUI.h"

#include "Export_Function.h"
#include "Texture.h"
#include "Include.h"
#include "BulletCnt.h"

CPlayerUI::CPlayerUI(LPDIRECT3DDEVICE9 pDevice)
: ENGINE::CGameObject(pDevice)
, m_pResourceMgr(ENGINE::Get_ResourcesMgr())
, m_pTimeMgr(ENGINE::Get_Time())
, m_fFrameCnt(0.f)
, m_fFrameMax(0.f)
, m_fFrameSpeed(0.f)
{

}

CPlayerUI::~CPlayerUI(void)
{
	Release();
}

CPlayerUI* CPlayerUI::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CPlayerUI*	pPlayerUI = new CPlayerUI(pDevice);
	if(FAILED(pPlayerUI->Initialize()))
		ENGINE::Safe_Delete(pPlayerUI);

	return pPlayerUI;

}

void CPlayerUI::Update(void)
{
	KeyInput();	
	if(bRifle)
	{
		if(iRifleBullet == 0)
			eType = STATE_LOAD;
	}
	if(bPisTol)
	{
		if(iPistolBullet == 0)
			eType = STATE_LOAD;
	}
}
HRESULT CPlayerUI::AddComponent(void)
{
	ENGINE::CComponent*			pComponent = NULL;

	// Buffer
	pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_STATIC, L"Buffer_RcUI");
	m_pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	// 0 ~ 3 라이플 Texture
	pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_STATIC, L"Texture_RifleReady");
	m_pTexture[0] = dynamic_cast<ENGINE::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_STATIC, L"Texture_RifleStand");
	m_pTexture[1] = dynamic_cast<ENGINE::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_STATIC, L"Texture_RifleShot");
	m_pTexture[2] = dynamic_cast<ENGINE::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_STATIC, L"Texture_RifleLoad");
	m_pTexture[3] = dynamic_cast<ENGINE::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);	
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	// 4 ~ 7 나이프 Texture
	pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_STATIC, L"Texture_KnifeStand");
	m_pTexture[4] = dynamic_cast<ENGINE::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);	
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_STATIC, L"Texture_KnifeReady");
	m_pTexture[5] = dynamic_cast<ENGINE::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);	
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_STATIC, L"Texture_KnifeAttack1");
	m_pTexture[6] = dynamic_cast<ENGINE::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);	
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_STATIC, L"Texture_KnifeAttack2");
	m_pTexture[7] = dynamic_cast<ENGINE::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);	
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	// 8 ~ 11 권총 Texture
	pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_STATIC, L"Texture_PistolStand");
	m_pTexture[8] = dynamic_cast<ENGINE::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);	
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_STATIC, L"Texture_PistolReady");
	m_pTexture[9] = dynamic_cast<ENGINE::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);	
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_STATIC, L"Texture_PistolLoad");
	m_pTexture[10] = dynamic_cast<ENGINE::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);	
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_STATIC, L"Texture_PistolShot");
	m_pTexture[11] = dynamic_cast<ENGINE::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);	
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));






	return S_OK;
}

void CPlayerUI::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00000088);
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//m_pResourceMgr->SetVtxInfo(ENGINE::RESOURCE_STATIC, L"Buffer_RcTex", m_pConvertVertex);

	//m_pTexture[0]->Render(WORD(m_fFrameCnt));

	FrameWork();

	m_pBuffer->Render();

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}

HRESULT CPlayerUI::Initialize(void)
{
	FAILED_CHECK(AddComponent());	

	m_fFrameMax = 29.f;
	eType = STATE_STAND;

	return S_OK;
}


void CPlayerUI::Release(void)
{

}

void CPlayerUI::FrameWork(void)
{

	if(bRifle)
	{
		switch(eType)
		{
		case STATE_READY:
			m_fFrameMax = 29.f;
			m_fFrameSpeed = 1.f;
			m_pTexture[0]->Render(WORD(m_fFrameCnt));
			break;
		case STATE_STAND:
			m_fFrameMax = 1.f;
			m_fFrameSpeed = 1.f;
			m_pTexture[1]->Render(WORD(m_fFrameCnt));
			break;
		case STATE_SHOT1:
			m_fFrameMax = 9.f;
			m_fFrameSpeed = 4.f;
			m_pTexture[2]->Render(WORD(m_fFrameCnt));
			break;
		case STATE_LOAD:
			m_fFrameMax = 40.f;
			m_fFrameSpeed = 0.4f;
			m_pTexture[3]->Render(WORD(m_fFrameCnt));
			break;
		}
	}

	if(bSniper)
	{
		switch(eType)
		{
		case STATE_READY:
			break;
		case STATE_STAND:
			break;
		case STATE_SHOT1:
			break;
		case STATE_LOAD:
			break;
		}
	}
	if(bPisTol)
	{
		switch(eType)
		{
		case STATE_READY:
			m_fFrameMax = 25.f;
			m_fFrameSpeed = 1.f;
			m_pTexture[9]->Render(WORD(m_fFrameCnt));
			break;
		case STATE_STAND:
			m_fFrameMax = 1.f;
			m_fFrameSpeed = 1.f;
			m_pTexture[8]->Render(WORD(m_fFrameCnt));
			break;
		case STATE_SHOT1:
			m_fFrameMax = 8.f;
			m_fFrameSpeed = 3.f;
			m_pTexture[11]->Render(WORD(m_fFrameCnt));
			break;
		case STATE_LOAD:
			m_fFrameMax = 39.f;
			m_fFrameSpeed = 0.5f;
			m_pTexture[10]->Render(WORD(m_fFrameCnt));
			break;
		}
	}
	if(bKnife)
	{
		switch(eType)
		{
		case STATE_STAND:
			m_fFrameMax = 1.f;
			m_fFrameSpeed = 3.f;
			m_pTexture[4]->Render(WORD(m_fFrameCnt));
			break;
		case STATE_READY:
			m_fFrameMax = 16.f;
			m_fFrameSpeed = 3.f;
			m_pTexture[5]->Render(WORD(m_fFrameCnt));
			break;	
		case STATE_SHOT1:
			m_fFrameMax = 9.f;
			m_fFrameSpeed = 3.f;
			m_pTexture[6]->Render(WORD(m_fFrameCnt));
			break;
		case STATE_LOAD:
			m_fFrameMax = 20.f;
			m_fFrameSpeed = 1.5f;
			m_pTexture[7]->Render(WORD(m_fFrameCnt));
			break;
		}
	}

	m_fFrameCnt += m_fFrameMax * m_pTimeMgr->GetTime() * m_fFrameSpeed;

	if(m_fFrameCnt > m_fFrameMax)
	{
		m_fFrameCnt = 0.f;
		bFire = false;
		if(eType == STATE_LOAD)
		{
			if(bRifle)
			{
				iRifleBullet = 30;
			}
			else if(bPisTol)
			{
				iPistolBullet = 15;
			}
		}
		if(bRifle)
		{
			if(eType == STATE_SHOT1)
			{
				CSoundMgr::GetInstance()->PlaySound(L"AKfire.wav");
				--iRifleBullet;
			}
		}
		if(bPisTol)
		{
			if(eType == STATE_SHOT1)
			{
				CSoundMgr::GetInstance()->PlaySound(L"B_fire.wav");
				--iPistolBullet;
			}
		}
		if(eType != STATE_STAND)
			eType = STATE_STAND;
	}

}

void CPlayerUI::KeyInput(void)
{
	if(bRifle)
	{
		if(GetAsyncKeyState(VK_LBUTTON))
		{
			eType = STATE_SHOT1;
			//CSoundMgr::GetInstance()->PlaySound(L"AKfire.wav");
			//iRifleBullet = iRifleBullet - 1;
		}
		if(GetAsyncKeyState('R'))
		{
			eType = STATE_LOAD;
			CSoundMgr::GetInstance()->PlaySound(L"AK_Reload.mp3");
		}
		if(GetAsyncKeyState('3'))
		{
			CBulletCnt::m_GunType = KNIFE;
			bKnife = true;
			bRifle = false;
			eType = STATE_READY;
			CSoundMgr::GetInstance()->PlaySound(L"K_select.wav");
		}
		if(GetAsyncKeyState('2'))
		{
			CBulletCnt::m_GunType = PISTOL;
			bPisTol = true;
			bRifle = false;
			eType = STATE_READY;

			CSoundMgr::GetInstance()->PlaySound(L"B_select.wav");
		}
	}
	if(bPisTol)
	{
		if(GetAsyncKeyState(VK_LBUTTON))
		{
			eType = STATE_SHOT1;
			//CSoundMgr::GetInstance()->PlaySound(L"B_fire.wav");
		}
		if(GetAsyncKeyState('R'))
		{
			eType = STATE_LOAD;
			CSoundMgr::GetInstance()->PlaySound(L"Beretta_Reload.mp3");
		}
		if(GetAsyncKeyState('1'))
		{
			CBulletCnt::m_GunType = RIFLE;
			bRifle = true;
			bPisTol = false;
			eType = STATE_READY;

			CSoundMgr::GetInstance()->PlaySound(L"R_select.wav");
		}
		if(GetAsyncKeyState('3'))
		{
			CBulletCnt::m_GunType = KNIFE;
			bKnife = true;
			bPisTol = false;
			eType = STATE_READY;

			CSoundMgr::GetInstance()->PlaySound(L"K_select.wav");
		}
	}
	if(bKnife)
	{
		if(GetAsyncKeyState(VK_LBUTTON))
		{
			eType = STATE_SHOT1;
			CSoundMgr::GetInstance()->PlaySound(L"K_fire.wav");
		}
		if(GetAsyncKeyState(VK_RBUTTON))
		{
			eType = STATE_LOAD;
			CSoundMgr::GetInstance()->PlaySound(L"K_fire.wav");
		}
		if(GetAsyncKeyState('1'))
		{
			CBulletCnt::m_GunType = RIFLE;
			bRifle = true;
			bKnife = false;
			eType = STATE_READY;

			CSoundMgr::GetInstance()->PlaySound(L"R_select.wav");
		}
		if(GetAsyncKeyState('2'))
		{
			CBulletCnt::m_GunType = PISTOL;
			bPisTol = true;
			bKnife = false;
			eType = STATE_READY;

			CSoundMgr::GetInstance()->PlaySound(L"B_select.wav");
		}
	}

}

