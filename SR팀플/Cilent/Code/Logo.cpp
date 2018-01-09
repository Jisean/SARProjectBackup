#include "stdafx.h"
#include "Logo.h"
#include "SceneSelector.h"
#include "Texture.h"

#include "Export_Function.h"
#include "ResourcesMgr.h"
#include "Layer.h"
#include "LogoBack.h"
#include "CollisionMgr.h"
#include "TextureMgr.h"
#include "GraphicDev.h"
ENGINE::CResourcesMgr* CLogo::m_pStaticResourceMgr = NULL;
LPDIRECT3DDEVICE9 CLogo::m_pStaticDevice = NULL;
int	CLogo::m_iImageCnt = 0;
CLogo::CLogo(LPDIRECT3DDEVICE9 pDevice)
: ENGINE::CScene(pDevice)
, m_pManagement(ENGINE::Get_Management())
, m_pResourceMgr(ENGINE::Get_ResourcesMgr())
, m_pGraphicDev(ENGINE::Get_GraphicDev())
{
	m_pStaticResourceMgr = m_pResourceMgr;
	m_pStaticDevice = pDevice;
	m_iImageCnt = 0;
}

CLogo::~CLogo(void)
{
	Release();
}
CLogo* CLogo::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CLogo*		pScene = new CLogo(pDevice);

	if(FAILED(pScene->Initialize()))
		ENGINE::Safe_Delete(pScene);

	return pScene;
}

HRESULT CLogo::Initialize(void)
{
	InitializeCriticalSection(&m_Crt);

	m_hThread = (HANDLE)_beginthreadex(NULL, 0, LoadThread, this, 0, NULL);
	
	HRESULT		hr = NULL;

	ENGINE::CComponent*			pComponent = NULL;
	//SetUp();

	hr = m_pResourceMgr->AddBuffer(m_pDevice, ENGINE::RESOURCE_DYNAMIC, 
		ENGINE::CVIBuffer::BUFFER_RCTEX, L"Buffer_RcTex");
	FAILED_CHECK_MSG(hr, L"Buffer_RcTex CREATE FAILED");

	hr = m_pResourceMgr->AddTexture(m_pDevice, ENGINE::RESOURCE_DYNAMIC, 
		ENGINE::TEX_NORMAL, L"Texture_Logo", L"../bin/Resources/Texture/Loading/Loading%d.jpg", 22);
	FAILED_CHECK(hr);

//	FAILED_CHECK_MSG(Add_Environment_Layer(), L"Environment 초기화 실패");
	FAILED_CHECK_MSG(Add_GameLogic_Layer(), L"GameLogic 초기화 실패");
//	FAILED_CHECK_MSG(Add_UI_Layer(), L"UI 초기화 실패");

	// Buffer
	pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, L"Buffer_RcTex");
	m_pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(m_pBuffer, E_FAIL);

	pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, L"Texture_Logo");
	m_pTexture = dynamic_cast<ENGINE::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	
	return S_OK;
}

void CLogo::Update(void)
{
	ENGINE::CScene::Update();


	if(GetAsyncKeyState(VK_RETURN) /*&& m_iImageCnt == 287*/)
	{
		WaitForSingleObject(m_hThread, INFINITE);
		CloseHandle(m_hThread);

		DeleteCriticalSection(&m_Crt);

		m_pManagement->SceneChange(CSceneSelector(SC_STAGE));
		return;
	}
}

void CLogo::Render(void)
{
	int E_Texture = m_pTexture->m_iImgCnt;
	int C_Texture = g_iImgCnt;

	int iTotal = E_Texture + C_Texture;
	
	int iTemp = int(22.f * (float(iTotal) / 425.f));
	m_pTexture->Render(iTemp);
	m_pBuffer->Render();
	//ENGINE::CScene::Render();	
}

void CLogo::Release(void)
{
	m_pResourceMgr->ResetDynamic();
}

void CLogo::SetUp(void)
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	D3DXMATRIX		matView, matProj;

	D3DXMatrixLookAtLH(&matView, &D3DXVECTOR3(0.f, 10.f, -20.f), &D3DXVECTOR3(0.f, 0.f, 0.f), 
		&D3DXVECTOR3(0.f, 1.f, 0.f));
	//m_pDevice->SetTransform(D3DTS_VIEW, &matView);

	// 1인자 : 뷰 행렬을 저장하기 위한 행렬 주소값
	// 2인자 : 카메라 포지션 벡터
	// 3인자 : 카메라가 바라보는 지점의 포지션 벡터
	// 4인자 : 업 벡터

	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(45.f), float(WINCX) / float(WINCY), 1.f, 1000.f);

	// 1인자 : 투영 행렬을 저장하긴 위한 행렬 주소값
	// 2인자 : 절두체 사이각(y값 : 절두체는 정사각형이어서 y의 앵글이나 x의 앵글이나 결국 동일)
	// 3인자 : 윈도우 비율 값
	// 4인자 : 가까운 평면 z값
	// 5인자 : 먼 평면 z값


	//m_pDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

HRESULT CLogo::Add_Environment_Layer(void)
{
	return S_OK;
}

HRESULT CLogo::Add_GameLogic_Layer(void)
{
	ENGINE::CLayer*		pLayer = ENGINE::CLayer::Create(m_pDevice);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	ENGINE::CGameObject*		pGameObject = NULL;
	pGameObject = CLogoBack::Create(m_pDevice);
	pLayer->AddObject(L"LogoBack", pGameObject);
	
	m_mapLayer.insert(MAPLAYER::value_type(LAYER_GAMELOGIC, pLayer));	


	return S_OK;
}

HRESULT CLogo::Add_UI_Layer(void)
{
	return S_OK;
}
unsigned int __stdcall CLogo::LoadThread(void* pArg)
{
	CLogo*		pLogo = (CLogo*)pArg;

	EnterCriticalSection(&pLogo->GetCrt());

	//
	HRESULT	hr = NULL;

	CTextureMgr::GetInstance()->InsertTexture(L"../bin/Resources/Texture/Time/%d.png", L"TimerNumber", TEX_MULTI, L"TimerNumber", 10);
	CTextureMgr::GetInstance()->InsertTexture(L"../bin/Resources/Texture/Time/Time.png", L"Time", TEX_SINGLE);
	CTextureMgr::GetInstance()->InsertTexture(L"../bin/Resources/Texture/Time/Colone.png", L"Colone", TEX_SINGLE);

	CTextureMgr::GetInstance()->InsertTexture(L"../bin/Resources/Texture/Score/score.png", L"Score", TEX_SINGLE);
	CTextureMgr::GetInstance()->InsertTexture(L"../bin/Resources/Texture/Score/score %d.png", L"ScoreNumber", TEX_MULTI, L"ScoreNumber", 10);

	CTextureMgr::GetInstance()->InsertTexture(L"../bin/Resources/Texture/Time/TimeOver.png", L"TimeOver", TEX_SINGLE);
	CTextureMgr::GetInstance()->InsertTexture(L"../bin/Resources/Texture/Time/TimeStart.png", L"TimeStart", TEX_SINGLE);

	ImgPath();

	////	Collision
	//hr = CCollisionMgr::GetInstance()->AddColObject(CCollisionMgr::COL_TERRAIN);
	//FAILED_CHECK(hr);

	//hr = CCollisionMgr::GetInstance()->AddColObject(CCollisionMgr::COL_MOUSE);
	//FAILED_CHECK(hr);

	//hr = CCollisionMgr::GetInstance()->AddColObject(CCollisionMgr::COL_OBJ);
	//FAILED_CHECK(hr);

	// Bullet Numbers
	hr = m_pStaticResourceMgr->AddTexture(m_pStaticDevice, ENGINE::RESOURCE_STATIC, 
		ENGINE::TEX_NORMAL, L"Texture_BNumbers", L"../bin/Resources/Texture/BulletNumbers/B%d.png", 10);
	m_iImageCnt += 10;

	hr = m_pStaticResourceMgr->AddTexture(m_pStaticDevice, ENGINE::RESOURCE_STATIC, 
		ENGINE::TEX_NORMAL, L"Texture_BTenNumbers", L"../bin/Resources/Texture/BulletNumbers/BTen%d.png", 4);
	m_iImageCnt += 4;

	// HP Numbers
	hr = m_pStaticResourceMgr->AddTexture(m_pStaticDevice, ENGINE::RESOURCE_STATIC, 
		ENGINE::TEX_NORMAL, L"Texture_Numbers", L"../bin/Resources/Texture/HpNumbers/%d.png", 10);
	m_iImageCnt += 10;

	hr = m_pStaticResourceMgr->AddTexture(m_pStaticDevice, ENGINE::RESOURCE_STATIC, 
		ENGINE::TEX_NORMAL, L"Texture_TenNumbers", L"../bin/Resources/Texture/HpNumbers/Ten%d.png", 10);
	m_iImageCnt += 10;

	hr = m_pStaticResourceMgr->AddTexture(m_pStaticDevice, ENGINE::RESOURCE_STATIC, 
		ENGINE::TEX_NORMAL, L"Texture_HundredNumbers", L"../bin/Resources/Texture/HpNumbers/Hundred%d.png", 10);
	m_iImageCnt += 10;

	// 핫식스
	hr = m_pStaticResourceMgr->AddTexture(m_pStaticDevice, ENGINE::RESOURCE_STATIC, 
		ENGINE::TEX_NORMAL, L"Texture_HotSix", L"../bin/Resources/Texture/Etc/HotSix.png", 1);
	m_iImageCnt += 1;

	// 레드불
	hr = m_pStaticResourceMgr->AddTexture(m_pStaticDevice, ENGINE::RESOURCE_STATIC, 
		ENGINE::TEX_NORMAL, L"Texture_RedBull", L"../bin/Resources/Texture/Etc/RedBull.png", 1);
	m_iImageCnt += 1;

	// 라이플
	hr = m_pStaticResourceMgr->AddTexture(m_pStaticDevice, ENGINE::RESOURCE_STATIC, 
		ENGINE::TEX_NORMAL, L"Texture_RifleReady", L"../bin/Resources/Texture/Rifle/Ready/%d.png", 29);
	m_iImageCnt += 29;

	hr = m_pStaticResourceMgr->AddTexture(m_pStaticDevice, ENGINE::RESOURCE_STATIC, 
		ENGINE::TEX_NORMAL, L"Texture_RifleStand", L"../bin/Resources/Texture/Rifle/Stand/%d.png", 1);
	m_iImageCnt += 1;

	hr = m_pStaticResourceMgr->AddTexture(m_pStaticDevice, ENGINE::RESOURCE_STATIC, 
		ENGINE::TEX_NORMAL, L"Texture_RifleShot", L"../bin/Resources/Texture/Rifle/Shot/%d.png", 9);
	m_iImageCnt += 9;

	hr = m_pStaticResourceMgr->AddTexture(m_pStaticDevice, ENGINE::RESOURCE_STATIC, 
		ENGINE::TEX_NORMAL, L"Texture_RifleLoad", L"../bin/Resources/Texture/Rifle/Load/%d.png", 40);
	m_iImageCnt += 40;

	// 나이프
	hr = m_pStaticResourceMgr->AddTexture(m_pStaticDevice, ENGINE::RESOURCE_STATIC, 
		ENGINE::TEX_NORMAL, L"Texture_KnifeStand", L"../bin/Resources/Texture/Knife/Stand/%d.png", 1);
	m_iImageCnt += 1;

	hr = m_pStaticResourceMgr->AddTexture(m_pStaticDevice, ENGINE::RESOURCE_STATIC, 
		ENGINE::TEX_NORMAL, L"Texture_KnifeReady", L"../bin/Resources/Texture/Knife/Ready/%d.png", 16);
	m_iImageCnt += 16;

	hr = m_pStaticResourceMgr->AddTexture(m_pStaticDevice, ENGINE::RESOURCE_STATIC, 
		ENGINE::TEX_NORMAL, L"Texture_KnifeAttack1", L"../bin/Resources/Texture/Knife/Attack1/%d.png", 9);
	m_iImageCnt += 9;

	hr = m_pStaticResourceMgr->AddTexture(m_pStaticDevice, ENGINE::RESOURCE_STATIC, 
		ENGINE::TEX_NORMAL, L"Texture_KnifeAttack2", L"../bin/Resources/Texture/Knife/Attack2/%d.png", 20);
	m_iImageCnt += 20;

	// 권총
	hr = m_pStaticResourceMgr->AddTexture(m_pStaticDevice, ENGINE::RESOURCE_STATIC, 
		ENGINE::TEX_NORMAL, L"Texture_PistolStand", L"../bin/Resources/Texture/Pistol/Stand/%d.png", 1);
	m_iImageCnt += 1;

	hr = m_pStaticResourceMgr->AddTexture(m_pStaticDevice, ENGINE::RESOURCE_STATIC, 
		ENGINE::TEX_NORMAL, L"Texture_PistolReady", L"../bin/Resources/Texture/Pistol/Ready/%d.png", 25);
	m_iImageCnt += 25;

	hr = m_pStaticResourceMgr->AddTexture(m_pStaticDevice, ENGINE::RESOURCE_STATIC, 
		ENGINE::TEX_NORMAL, L"Texture_PistolLoad", L"../bin/Resources/Texture/Pistol/Load/%d.png", 39);
	m_iImageCnt += 39;

	hr = m_pStaticResourceMgr->AddTexture(m_pStaticDevice, ENGINE::RESOURCE_STATIC, 
		ENGINE::TEX_NORMAL, L"Texture_PistolShot", L"../bin/Resources/Texture/Pistol/Shot/%d.png", 8);
	m_iImageCnt += 8;

	// 크로스라인
	hr = m_pStaticResourceMgr->AddTexture(m_pStaticDevice, ENGINE::RESOURCE_STATIC, 
		ENGINE::TEX_NORMAL, L"Texture_Back", L"../bin/Resources/Texture/Back/%d.png", 1);
	m_iImageCnt += 1;


	// 체력
	hr = m_pStaticResourceMgr->AddTexture(m_pStaticDevice, ENGINE::RESOURCE_STATIC, 
		ENGINE::TEX_NORMAL, L"Texture_Blood", L"../bin/Resources/Texture/Blood/%d.png", 24);
	m_iImageCnt += 24;

	// 무적
	hr = m_pStaticResourceMgr->AddTexture(m_pStaticDevice, ENGINE::RESOURCE_STATIC, 
		ENGINE::TEX_NORMAL, L"Texture_Unbeatable", L"../bin/Resources/Texture/Unbeatable/%d.png", 20);
	m_iImageCnt += 20;

	//문
	hr = m_pStaticResourceMgr->AddTexture(m_pStaticDevice, ENGINE::RESOURCE_STATIC, 
		ENGINE::TEX_NORMAL, L"Texture_Door", L"../bin/Resources/Texture/Portal/Portal%d.png", 25);
	m_iImageCnt += 25;
	
	//리스폰
	hr = m_pStaticResourceMgr->AddTexture(m_pStaticDevice, ENGINE::RESOURCE_STATIC, 
		ENGINE::TEX_NORMAL, L"Texture_Respawn", L"../bin/Resources/Texture/Etc/Respawn.png", 1);
	m_iImageCnt += 1;

	//리스폰
	hr = m_pStaticResourceMgr->AddTexture(m_pStaticDevice, ENGINE::RESOURCE_STATIC, 
		ENGINE::TEX_CUBE, L"Respawn", L"../bin/Resources/Texture/Respawn.dds", 1);
	//센트리건 총알
	hr = m_pStaticResourceMgr->AddTexture(m_pStaticDevice, ENGINE::RESOURCE_STATIC, 
		ENGINE::TEX_CUBE, L"Fireball", L"../bin/Resources/Texture/Fireball.dds", 1);

	// CBook
	hr = m_pStaticResourceMgr->AddTexture(m_pStaticDevice, ENGINE::RESOURCE_STATIC, 
		ENGINE::TEX_NORMAL, L"Texture_CBook", L"../bin/Resources/Texture/Etc/CBook.png", 1);

	hr = m_pStaticResourceMgr->AddTexture(m_pStaticDevice, ENGINE::RESOURCE_STATIC, 
		ENGINE::TEX_NORMAL, L"Texture_Bang", L"../bin/Resources/Texture/Etc/Bang.png", 1);

	// Dragon
	hr = m_pStaticResourceMgr->AddTexture(m_pStaticDevice, ENGINE::RESOURCE_STATIC, 
		ENGINE::TEX_NORMAL, L"Texture_DragonBook", L"../bin/Resources/Texture/Etc/DragonBook.png", 1);

	// Skull
	hr = m_pStaticResourceMgr->AddTexture(m_pStaticDevice, ENGINE::RESOURCE_STATIC, 
		ENGINE::TEX_NORMAL, L"Texture_SkullBook", L"../bin/Resources/Texture/Etc/SkullBook.png", 1);

	// Miss
	hr = m_pStaticResourceMgr->AddTexture(m_pStaticDevice, ENGINE::RESOURCE_STATIC, 
		ENGINE::TEX_NORMAL, L"Texture_Miss", L"../bin/Resources/Texture/Etc/Miss.png", 1);


	LeaveCriticalSection(&pLogo->GetCrt());
	_endthreadex(0);
	return 0;
}

CRITICAL_SECTION CLogo::GetCrt(void)
{
	return m_Crt;
}
void CLogo::ImgPath(void)
{
	wifstream		LoadFile;

	LoadFile.open(L"../../Data/ImgPath_dds.txt", ios::in);	// 파일 개방

	TCHAR	szObjKey[MIN_STR]	= L"";
	TCHAR	szCount[MIN_STR]	= L"";
	TCHAR	szImgPath[MAX_PATH] = L"";

	wstring	wstrCombine = L"";

	while(!LoadFile.eof())
	{
		LoadFile.getline(szObjKey, MIN_STR, '|');
		//LoadFile.getline(szCount, MIN_STR, '|');
		LoadFile.getline(szImgPath, MAX_PATH);

		int iCount = _ttoi(szCount);

		m_pStaticResourceMgr->AddTexture(m_pStaticDevice, ENGINE::RESOURCE_STATIC, 
			ENGINE::TEX_CUBE, szObjKey, szImgPath, 1);

		const wstring& wstrTemp = szObjKey;
		wstring wstrKey = wstrTemp;
		m_iImageCnt += 1;
	}

	LoadFile.close();

	wifstream		LoadFile2;

	LoadFile2.open(L"../../Data/ImgPath_png.txt", ios::in);	// 파일 개방

	TCHAR	szObjKey2[MIN_STR]	= L"";
	TCHAR	szStateKey2[MIN_STR] = L"";
	TCHAR	szCount2[MIN_STR]	= L"";
	TCHAR	szImgPath2[MAX_PATH] = L"";

	wstring	wstrCombine2 = L"";

	while(!LoadFile2.eof())
	{
		LoadFile2.getline(szObjKey2, MIN_STR, '|');
		LoadFile2.getline(szStateKey2, MIN_STR, '|');
		LoadFile2.getline(szCount2, MIN_STR, '|');
		LoadFile2.getline(szImgPath2, MAX_PATH);

		int iCount = _ttoi(szCount2);
		wstrCombine2 = wstring(szObjKey2) + L"_" + wstring(szStateKey2);

		m_pStaticResourceMgr->AddTexture(m_pStaticDevice, ENGINE::RESOURCE_STATIC, 
			ENGINE::TEX_NORMAL, wstrCombine2, szImgPath2, iCount);

		m_iImageCnt += 1;
	}

	LoadFile2.close();	
}