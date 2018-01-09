#include "stdafx.h"
#include "MainApp.h"

#include "GraphicDev.h"
#include "Include.h"
#include "Export_Function.h"
#include "SceneSelector.h"
#include "KeyMgr.h"
#include "CollisionMgr.h"
#include "TextureMgr.h"
CMainApp::CMainApp(void)
: m_pGraphicDev(ENGINE::Get_GraphicDev())
, m_pManagement(ENGINE::Get_Management())
, m_pTimeMgr(ENGINE::Get_Time())
, m_pResourceMgr(ENGINE::Get_ResourcesMgr())
, m_pDevice(NULL)
{
	CSoundMgr::GetInstance()->Initialize();
	CSoundMgr::GetInstance()->LoadSoundFile();
}

CMainApp::~CMainApp()
{
	Release();
}

CMainApp* CMainApp::Create(void)
{
	CMainApp*	pMainApp = new CMainApp;

	if(FAILED(pMainApp->Initialize()))
	{
		delete pMainApp;
		pMainApp = NULL;
	}

	return pMainApp;
}

HRESULT CMainApp::Initialize(void)
{

	/*if (AllocConsole()) 
	{
		freopen("CONIN$", "rb", stdin);
		freopen("CONOUT$", "wb", stdout);
		freopen("CONOUT$", "wb", stderr);
	}*/

	HRESULT hr = NULL;

	hr = m_pGraphicDev->InitGraphicDev(ENGINE::CGraphicDev::MODE_WIN, 
		g_hWnd, WINCX, WINCY);
	FAILED_CHECK_MSG(hr, L"장치 초기화 실패");

	m_pDevice = m_pGraphicDev->GetDevice();
	NULL_CHECK_RETURN(m_pDevice, E_FAIL);

	hr = m_pManagement->InitManagement(m_pDevice);
	FAILED_CHECK_MSG(hr, L"Management Init Failed");


	hr = m_pManagement->SceneChange(CSceneSelector(SC_LOGO));
	FAILED_CHECK_MSG(hr, L"Scene Change Failed");

	// 조명 관련 코드
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pTimeMgr->InitTime();

	return S_OK;
}

void CMainApp::Update(void)
{
	m_pTimeMgr->SetTime();
	m_pManagement->Update();
	CKeyMgr::GetInstance()->KeyCheck();
}	

void CMainApp::Render(void)
{
	m_pManagement->Render(m_pTimeMgr->GetTime());
}

void CMainApp::Release(void)
{
	ENGINE::Safe_Single_Destory(m_pGraphicDev);
	ENGINE::Safe_Single_Destory(m_pManagement);
	ENGINE::Safe_Single_Destory(m_pTimeMgr);
	CKeyMgr::GetInstance()->DestroyInstance();
	CCollisionMgr::GetInstance()->DestroyInstance();
	CTextureMgr::GetInstance()->DestroyInstance();
	CSoundMgr::GetInstance()->DestroyInstance();
}


