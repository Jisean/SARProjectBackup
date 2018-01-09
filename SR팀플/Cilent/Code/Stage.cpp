#include "stdafx.h"
#include "Stage.h"

#include "Texture.h"
#include "Export_Function.h"
#include "Layer.h"
#include "Player.h"
#include "Include.h"
#include "Terrain.h"
#include "Pipeline.h"
#include "Transform.h"
#include "StaticCamera.h"
#include "CollisionMgr.h"
#include "Monster.h"
#include "CubeObj.h"
#include "PlayerTex.h"
#include "Obstacle.h"
#include "SpinObstacle.h"
#include "UpDownCube.h"
#include "CentryGun.h"
#include "SpinCutter.h"
#include "PlayerUI.h"
#include "CrossLine.h"

#include "LeftToRight.h"
#include "RightToLeft.h"
#include "SpinSnow.h"
#include "CrashMaze.h"
#include "ShrinkingPlatform.h"
#include "..\..\Tool58\CubeStair.h"
#include "..\..\Engine\Utility\Code\Management.h"
#include "SceneSelector.h"
#include "RedBull.h"
#include "HotSix.h"
#include "HP.h"
#include "BulletCnt.h"
#include "Back.h"
#include "ScreenEffect.h"
#include "Score.h"
#include "Door.h"
#include "Respawn.h"


CStage::CStage(LPDIRECT3DDEVICE9 pDevice)
: ENGINE::CScene(pDevice)
, m_pResourceMgr(ENGINE::Get_ResourcesMgr())
, m_pManagement(ENGINE::Get_Management())
, m_bInitHp(false)
{

}

CStage::~CStage(void)
{
	ENGINE::CScene::Release();
	Release();
}
CStage* CStage::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CStage* pStage = new CStage(pDevice);

	if(FAILED(pStage->Initialize()))
		ENGINE::Safe_Delete(pStage);

	return pStage;
}

HRESULT CStage::Initialize(void)
{
	//SetUp();

	HRESULT	hr = NULL;

	hr = m_pResourceMgr->AddBuffer(m_pDevice, ENGINE::RESOURCE_DYNAMIC, 
		ENGINE::CVIBuffer::BUFFER_RCTEX, L"Buffer_RcTex");
	FAILED_CHECK_MSG(hr, L"Buffer_RcTex CREATE FAILED");

	// Buffer
	hr = m_pResourceMgr->AddBuffer(m_pDevice, ENGINE::RESOURCE_DYNAMIC, 
		ENGINE::CVIBuffer::TERRAIN_TEX, L"Buffer_Terrain", VTXCNTX, VTXCNTZ, VTXITV);
		FAILED_CHECK(hr);

		hr = m_pResourceMgr->AddBuffer(m_pDevice, ENGINE::RESOURCE_DYNAMIC, 
			ENGINE::CVIBuffer::BUFFER_RCTEX, L"Buffer_BackY");
		FAILED_CHECK(hr);

	// PlayerUI Buffer
	hr = m_pResourceMgr->AddBuffer(m_pDevice, ENGINE::RESOURCE_STATIC, 
		ENGINE::CVIBuffer::BUFFER_RCUI, L"Buffer_RcUI");

	// Cube Buffer - dynamic
	hr = m_pResourceMgr->AddBuffer(m_pDevice, ENGINE::RESOURCE_DYNAMIC, 
		ENGINE::CVIBuffer::BUFFER_CUBETEX, L"Buffer_CubeTex");
		FAILED_CHECK(hr);

	//// Cube Buffer - static
	//hr = m_pResourceMgr->AddBuffer(m_pDevice, ENGINE::RESOURCE_STATIC, 
	//	ENGINE::CVIBuffer::BUFFER_CUBETEX, L"Buffer_CubeTex");
	//	FAILED_CHECK(hr);

	//ImgPath();

	//	Collision
	hr = CCollisionMgr::GetInstance()->AddColObject(CCollisionMgr::COL_TERRAIN);
	FAILED_CHECK(hr);

	hr = CCollisionMgr::GetInstance()->AddColObject(CCollisionMgr::COL_MOUSE);
	FAILED_CHECK(hr);

	hr = CCollisionMgr::GetInstance()->AddColObject(CCollisionMgr::COL_OBJ);
	FAILED_CHECK(hr);
	

	FAILED_CHECK_MSG(Add_Environment_Layer(), L"Environment 초기화 실패");
	FAILED_CHECK_MSG(Add_GameLogic_Layer(), L"GameLogic 초기화 실패");
	FAILED_CHECK_MSG(Add_UI_Layer(), L"UI 초기화 실패");

	CSoundMgr::GetInstance()->PlayBgm(L"run_bgm1.wav");
	

	return S_OK;
}

void CStage::Update(void)
{
	ENGINE::CScene::Update();

	if(bLevel)
	{
		iScore += int(fHp * 10.f);
		CSoundMgr::GetInstance()->StopBga();
		m_pManagement->SceneChange(CSceneSelector(SC_STAGE1));
		return;
	}
	if(m_bInitHp == false)
	{
		m_bInitHp = true;
		fHp = 100.f;
	}

	if(GetAsyncKeyState(VK_F1) /*&& m_iImageCnt == 287*/)
	{
		iScore += int(fHp * 10.f);
		CSoundMgr::GetInstance()->StopBga();
		m_pManagement->SceneChange(CSceneSelector(SC_STAGE1));
		return;
	}
}

void CStage::Render(void)
{
	ENGINE::CScene::Render();
}

void CStage::Release(void)
{
	m_pResourceMgr->ResetDynamic();
	CCollisionMgr::GetInstance()->DestroyInstance();
}

HRESULT CStage::Add_Environment_Layer(void)
{
	return S_OK;
}

HRESULT CStage::Add_GameLogic_Layer(void)
{
	ENGINE::CLayer*		pLayer = ENGINE::CLayer::Create(m_pDevice);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	
	ENGINE::CGameObject*		pGameObject = NULL;

	// PlayerUI
	pGameObject = CPlayerUI::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"PlayerUI", pGameObject);
	
	// Terrain
	pGameObject = CTerrain::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"Terrain", pGameObject);

	// player
	pGameObject = CPlayer::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	((CPlayer*)pGameObject)->GetInfo()->m_vPos = D3DXVECTOR3(10.f, 0.f, 10.f);
	((CPlayer*)pGameObject)->SetStageState(CPlayer::ST_STAGE);
	pLayer->AddObject(L"Player", pGameObject);

	//PlayerTex
	pGameObject = m_pPlayer = CPlayerTex::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"PlayerTex", pGameObject);

	// HotSix
	pGameObject = CHotSix::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	((CHotSix*)pGameObject)->SetPos(D3DXVECTOR3(5.f, 2.f, 20.f));
	pLayer->AddObject(L"HotSix", pGameObject);

	// RedBull
	pGameObject = CRedBull::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	((CRedBull*)pGameObject)->SetPos(D3DXVECTOR3(10.f, 2.f, 20.f));
	pLayer->AddObject(L"RedBull", pGameObject);

	//Door
	pGameObject = CDoor::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	((CDoor*)pGameObject)->SetPos(D3DXVECTOR3(15.f, 2.f, 275.f));
	pLayer->AddObject(L"Door", pGameObject);

	//Respawn
	pGameObject = CRespawn::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	((CRespawn*)pGameObject)->SetPos(D3DXVECTOR3(10.f, 2.f, 15.f));
	pLayer->AddObject(L"Respawn", pGameObject);


	Load(pLayer);

	CreateBack(pLayer);

	
	m_mapLayer.insert(MAPLAYER::value_type(LAYER_GAMELOGIC, pLayer));	

	return S_OK;
}

HRESULT CStage::Add_UI_Layer(void)
{
	ENGINE::CLayer*		pLayer = ENGINE::CLayer::Create(m_pDevice);
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	//PlayerInfo

	MAPLAYER::iterator	iter = m_mapLayer.find(LAYER_GAMELOGIC);
	if(iter == m_mapLayer.end())
		return E_FAIL;

	const ENGINE::CComponent*	pTransform = iter->second->GetComponent(L"Player", L"Transform");
	NULL_CHECK_RETURN(pTransform, E_FAIL);

	((CPlayerTex*)m_pPlayer)->SetPlayerTex(&(((ENGINE::CTransform*)pTransform)->m_vPos));

	
	ENGINE::CGameObject*		pGameObject = NULL;
	pGameObject = CStaticCamera::Create(m_pDevice, dynamic_cast<const ENGINE::CTransform*>(pTransform));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"StaticCamera", pGameObject);

	// CrossLine
	pGameObject = CCrossLine::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"CrossLine", pGameObject);

	// scrreneffect
	pGameObject = CScreenEffect::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"ScreenEffect", pGameObject);

	// BulletCnt10
	pGameObject = CBulletCnt::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	((CBulletCnt*)pGameObject)->SetBulletType(KNIFE);
	((CBulletCnt*)pGameObject)->setDigit(BTEN);
	pLayer->AddObject(L"ZBulletCnt10", pGameObject);

	// BulletCnt1
	pGameObject = CBulletCnt::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	((CBulletCnt*)pGameObject)->SetBulletType(KNIFE);
	((CBulletCnt*)pGameObject)->setDigit(BONE);
	pLayer->AddObject(L"ZBulletCnt1", pGameObject);

	// HP100
	pGameObject = CHP::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	((CHP*)pGameObject)->SetDigit(HUNDRED);
	pLayer->AddObject(L"HP100", pGameObject);

	// HP10
	pGameObject = CHP::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	((CHP*)pGameObject)->SetDigit(TEN);
	pLayer->AddObject(L"HP10", pGameObject);

	// HP1
	pGameObject = CHP::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	((CHP*)pGameObject)->SetDigit(ONE);
	pLayer->AddObject(L"HP1", pGameObject);

	// Score
	pGameObject = CScore::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"Score", pGameObject);

	m_mapLayer.insert(MAPLAYER::value_type(LAYER_UI, pLayer));

	return S_OK;
}

void CStage::SetUp(void)
{
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	D3DXMATRIX		matView, matProj;
	//D3DXMatrixLookAtLH(&matView, &D3DXVECTOR3(0.f, 10.f, -20.f), &D3DXVECTOR3(0.f, 0.f, 0.f), 
	//	&D3DXVECTOR3(0.f, 1.f, 0.f));

	/*ENGINE::CPipeline::MakeViewSpaceMatrix(&matView, &D3DXVECTOR3(0.f, 10.f, -20.f), &D3DXVECTOR3(0.f, 0.f, 0.f),&D3DXVECTOR3(0.f, 1.f, 0.f));

	m_pDevice->SetTransform(D3DTS_VIEW, &matView);*/
	
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(45.f), float(WINCX) / float(WINCY), 1.f, 1000.f);
	//m_pDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

void CStage::ImgPath(void)
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

		m_pResourceMgr->AddTexture(m_pDevice, ENGINE::RESOURCE_STATIC, 
			ENGINE::TEX_CUBE, szObjKey, szImgPath, 1);

		const wstring& wstrTemp = szObjKey;
		wstring wstrKey = wstrTemp;

		m_KeyList.push_back(wstrKey);
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

		m_pResourceMgr->AddTexture(m_pDevice, ENGINE::RESOURCE_STATIC, 
			ENGINE::TEX_NORMAL, wstrCombine2, szImgPath2, iCount);
	}

	LoadFile2.close();	
}

void CStage::Load(ENGINE::CLayer* pLayer)
{
	//CObj* pObj = ((CMainFrame*)AfxGetMainWnd())->m_pMainView->m_pBack;
	int iCount = 0;
	int iFlag = 0;

	D3DXVECTOR3 vPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 vScale = D3DXVECTOR3(0.f, 0.f, 0.f);
	float fAngle[3] = {0};
	TCHAR ObjKey[30];
	wstring wstrObjKey = L"";

	FILE* pFile = NULL;

	fopen_s(&pFile, "../../Data/MapData.dat", "rb");

	fread(&iCount, sizeof(int), 1, pFile);

	for(int i = 0; i != iCount; ++i)
	{
		fread(&iFlag, sizeof(int), 1, pFile);
		fread(&vPos, sizeof(D3DXVECTOR3), 1, pFile);
		fread(&vScale, sizeof(D3DXVECTOR3), 1, pFile);
		fread(fAngle, sizeof(float), 3, pFile);
		fread(ObjKey, sizeof(TCHAR), 30, pFile);	

		wstrObjKey = ObjKey;

		//wcscpy_s((wchar_t*)wstrObjKey.c_str(), sizeof(wchar_t) * 30, ObjKey);

		//CCubeObj* pCubeObj = CCubeObj::Create(ENGINE::CGraphicDev::GetInstance(), wstrObjKey);

		ENGINE::CGameObject*		pCubeObj = NULL;

		// 큐브일 때
		if(iFlag == 0)
		{
			pCubeObj = CCubeObj::Create(m_pDevice);
			((CObstacle*)pCubeObj)->GetInfo()->m_vPos = vPos;
			((CObstacle*)pCubeObj)->GetInfo()->m_vScale = vScale;
			((CObstacle*)pCubeObj)->GetInfo()->m_fAngle[ENGINE::ANGLE_X] = fAngle[0];
			((CObstacle*)pCubeObj)->GetInfo()->m_fAngle[ENGINE::ANGLE_Y] = fAngle[1];
			((CObstacle*)pCubeObj)->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = fAngle[2];
			((CCubeObj*)pCubeObj)->SetPos(vPos);
			pLayer->AddObject(L"CubeObj", pCubeObj);
		}
		else if(iFlag == 1)
		{
			pCubeObj = CSpinObstacle::Create(m_pDevice);
			((CObstacle*)pCubeObj)->GetInfo()->m_vPos = vPos;
			((CObstacle*)pCubeObj)->GetInfo()->m_vScale = vScale;
			((CObstacle*)pCubeObj)->GetInfo()->m_fAngle[ENGINE::ANGLE_X] = fAngle[0];
			((CObstacle*)pCubeObj)->GetInfo()->m_fAngle[ENGINE::ANGLE_Y] = fAngle[1];
			((CObstacle*)pCubeObj)->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = fAngle[2];

			pLayer->AddObject(L"SpinObstacle", pCubeObj);
		}
		else if(iFlag == 2)
		{
			pCubeObj = CUpDownCube::Create(m_pDevice);
			((CObstacle*)pCubeObj)->GetInfo()->m_vPos = vPos;
			((CObstacle*)pCubeObj)->GetInfo()->m_vScale = vScale;
			((CObstacle*)pCubeObj)->GetInfo()->m_fAngle[ENGINE::ANGLE_X] = fAngle[0];
			((CObstacle*)pCubeObj)->GetInfo()->m_fAngle[ENGINE::ANGLE_Y] = fAngle[1];
			((CObstacle*)pCubeObj)->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = fAngle[2];
			((CUpDownCube*)pCubeObj)->SetPos(vPos);

			pLayer->AddObject(L"UpDownCube", pCubeObj);

		}
		else if(iFlag == 3)
		{
			pCubeObj = CCentryGun::Create(m_pDevice);
			((CObstacle*)pCubeObj)->GetInfo()->m_vPos = vPos;
			((CObstacle*)pCubeObj)->GetInfo()->m_vScale = vScale;
			((CObstacle*)pCubeObj)->GetInfo()->m_fAngle[ENGINE::ANGLE_X] = fAngle[0];
			((CObstacle*)pCubeObj)->GetInfo()->m_fAngle[ENGINE::ANGLE_Y] = fAngle[1];
			((CObstacle*)pCubeObj)->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = fAngle[2];
			((CCentryGun*)pCubeObj)->SetPos(vPos);

			pLayer->AddObject(L"CentryGun", pCubeObj);
		}
		else if(iFlag == 4)
		{
			pCubeObj = CSpinCutter::Create(m_pDevice);
			((CObstacle*)pCubeObj)->GetInfo()->m_vPos = vPos;
			((CObstacle*)pCubeObj)->GetInfo()->m_vScale = vScale;
			((CObstacle*)pCubeObj)->GetInfo()->m_fAngle[ENGINE::ANGLE_X] = fAngle[0];
			((CObstacle*)pCubeObj)->GetInfo()->m_fAngle[ENGINE::ANGLE_Y] = fAngle[1];
			((CObstacle*)pCubeObj)->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = fAngle[2];
			((CSpinCutter*)pCubeObj)->SetPos(vPos);

			pLayer->AddObject(L"SpinCutter", pCubeObj);
		}
		else if(iFlag == 5)
		{
			pCubeObj = CCrashMaze::Create(m_pDevice);
			((CObstacle*)pCubeObj)->GetInfo()->m_vPos = vPos;
			((CObstacle*)pCubeObj)->GetInfo()->m_vScale = vScale;
			((CObstacle*)pCubeObj)->GetInfo()->m_fAngle[ENGINE::ANGLE_X] = fAngle[0];
			((CObstacle*)pCubeObj)->GetInfo()->m_fAngle[ENGINE::ANGLE_Y] = fAngle[1];
			((CObstacle*)pCubeObj)->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = fAngle[2];
			((CCrashMaze*)pCubeObj)->SetPos(vPos);

			pLayer->AddObject(L"CrashMaze", pCubeObj);
		}
		else if(iFlag == 6)
		{
			pCubeObj = CShrinkingPlatform::Create(m_pDevice);
			((CObstacle*)pCubeObj)->GetInfo()->m_vPos = vPos;
			((CObstacle*)pCubeObj)->GetInfo()->m_vScale = vScale;
			((CObstacle*)pCubeObj)->GetInfo()->m_fAngle[ENGINE::ANGLE_X] = fAngle[0];
			((CObstacle*)pCubeObj)->GetInfo()->m_fAngle[ENGINE::ANGLE_Y] = fAngle[1];
			((CObstacle*)pCubeObj)->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = fAngle[2];
			((CShrinkingPlatform*)pCubeObj)->SetPos(vPos);

			pLayer->AddObject(L"ShrinkingPlatform", pCubeObj);
		}
		else if(iFlag == 7)
		{
			pCubeObj = CLeftToRight::Create(m_pDevice);
			((CObstacle*)pCubeObj)->GetInfo()->m_vPos = vPos;
			((CObstacle*)pCubeObj)->GetInfo()->m_vScale = vScale;
			((CObstacle*)pCubeObj)->GetInfo()->m_fAngle[ENGINE::ANGLE_X] = fAngle[0];
			((CObstacle*)pCubeObj)->GetInfo()->m_fAngle[ENGINE::ANGLE_Y] = fAngle[1];
			((CObstacle*)pCubeObj)->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = fAngle[2];
			((CLeftToRight*)pCubeObj)->SetPos(vPos);

			pLayer->AddObject(L"LeftToRight", pCubeObj);
		}
		else if(iFlag == 8)
		{
			pCubeObj = CRightToLeft::Create(m_pDevice);
			((CObstacle*)pCubeObj)->GetInfo()->m_vPos = vPos;
			((CObstacle*)pCubeObj)->GetInfo()->m_vScale = vScale;
			((CObstacle*)pCubeObj)->GetInfo()->m_fAngle[ENGINE::ANGLE_X] = fAngle[0];
			((CObstacle*)pCubeObj)->GetInfo()->m_fAngle[ENGINE::ANGLE_Y] = fAngle[1];
			((CObstacle*)pCubeObj)->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = fAngle[2];
			((CRightToLeft*)pCubeObj)->SetPos(vPos);

			pLayer->AddObject(L"RightToLeft", pCubeObj);
		}
		else if(iFlag == 9)
		{
			pCubeObj = CSpinSnow::Create(m_pDevice);
			((CObstacle*)pCubeObj)->GetInfo()->m_vPos = vPos;
			((CObstacle*)pCubeObj)->GetInfo()->m_vScale = vScale;
		/*	((CObstacle*)pCubeObj)->GetInfo()->m_fAngle[ENGINE::ANGLE_X] = fAngle[0];
			((CObstacle*)pCubeObj)->GetInfo()->m_fAngle[ENGINE::ANGLE_Y] = fAngle[1];
			((CObstacle*)pCubeObj)->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = fAngle[2];*/
			((CSpinSnow*)pCubeObj)->SetPos(vPos);

			pLayer->AddObject(L"SpinSnow", pCubeObj);
		}
		else if(iFlag == 10)
		{
			pCubeObj = CCubeStair::Create(m_pDevice);
			((CObstacle*)pCubeObj)->GetInfo()->m_vPos = vPos;
			((CObstacle*)pCubeObj)->GetInfo()->m_vScale = vScale;
			((CObstacle*)pCubeObj)->GetInfo()->m_fAngle[ENGINE::ANGLE_X] = fAngle[0];
			((CObstacle*)pCubeObj)->GetInfo()->m_fAngle[ENGINE::ANGLE_Y] = fAngle[1];
			((CObstacle*)pCubeObj)->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = fAngle[2];
			((CRightToLeft*)pCubeObj)->SetPos(vPos);

			pLayer->AddObject(L"CubeStair", pCubeObj);
		}


		/*((CObstacle*)pCubeObj)->GetInfo()->m_vPos = vPos;
		((CObstacle*)pCubeObj)->GetInfo()->m_vScale = vScale;
		((CObstacle*)pCubeObj)->GetInfo()->m_fAngle[ENGINE::ANGLE_X] = fAngle[0];
		((CObstacle*)pCubeObj)->GetInfo()->m_fAngle[ENGINE::ANGLE_Y] = fAngle[1];
		((CObstacle*)pCubeObj)->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = fAngle[2];

		pLayer->AddObject(L"Obstacle", pCubeObj);*/

		ChangeTexture((CObstacle*)pCubeObj, wstrObjKey);
		

		int k = 0;


		//pLayer->AddObject(L"Obstacle", pCubeObj);
	}

	fclose(pFile);
}

void CStage::ChangeTexture(CObstacle* pCubeObj, const wstring& wstrName)
{
	if(pCubeObj == NULL)
		return;

	const ENGINE::CComponent*	pComponent = pCubeObj->GetComponent(L"Texture");

	// 컴포넌트 맵 컨테이너 찾아와 Texture 삭제하기
	map<wstring, ENGINE::CComponent*>::iterator iter = pCubeObj->GetmapComponent()->find(L"Texture");

	pCubeObj->GetmapComponent()->erase(iter);

	ENGINE::Safe_Delete(pComponent);


	// 컴포넌트 만들기
	pCubeObj->m_wstrObjKey = wstrName;

	ENGINE::CResourcesMgr* m_pResourceMgr = ENGINE::Get_ResourcesMgr();
	pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_STATIC, wstrName);


	// 컴포넌트 맵 컨테이너에 추가하기
	ENGINE::CComponent* pComponent2 = (ENGINE::CComponent*)pComponent;

	pCubeObj->GetmapComponent()->insert(map<wstring, ENGINE::CComponent*>::value_type(L"Texture", pComponent2));

	// CubeObj의 Texture 수정하기
	pCubeObj->SetTexture(dynamic_cast<ENGINE::CTexture*>(pComponent2));

	//// 텍스쳐 바꿔주기
	//const ENGINE::CComponent*	pComponent = pCubeObj->GetComponent(L"Texture");

	//map<wstring, ENGINE::CComponent*>::iterator iter = ((CObstacle*)pCubeObj)->GetmapComponent()->find(L"Texture");
	//((CObstacle*)pCubeObj)->GetmapComponent()->erase(iter);

	//ENGINE::Safe_Delete(pComponent);

	//pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, wstrObjKey);

	//ENGINE::CComponent* pComponent2 = (ENGINE::CComponent*)pComponent;

	//((CObstacle*)pCubeObj)->GetmapComponent()->insert(map<wstring, ENGINE::CComponent*>::value_type(L"Texture", pComponent2));
	////((CBack*)pObj)->m_vecCube.push_back(pCubeObj);
}

HRESULT CStage::CreateBack(ENGINE::CLayer* pLayer)
{
	ENGINE::CGameObject*		pGameObject = NULL;

	// BackY+
	pGameObject = CBack::Create(m_pDevice);
	((CBack*)pGameObject)->m_pInfo->m_vPos = D3DXVECTOR3(20.f, 30.f, 125.f);
	((CBack*)pGameObject)->m_pInfo->m_fAngle[ENGINE::ANGLE_X] = D3DXToRadian(270.f);
	((CBack*)pGameObject)->m_pInfo->m_fAngle[ENGINE::ANGLE_Y] = D3DXToRadian(90.f);
	((CBack*)pGameObject)->m_pInfo->m_vScale = D3DXVECTOR3(250.f, 33.f, 0.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"Back", pGameObject);

	// BackY-
	pGameObject = CBack::Create(m_pDevice);
	((CBack*)pGameObject)->m_pInfo->m_vPos = D3DXVECTOR3(20.f, -3.f, 125.f);
	((CBack*)pGameObject)->m_pInfo->m_fAngle[ENGINE::ANGLE_X] = D3DXToRadian(90.f);
	((CBack*)pGameObject)->m_pInfo->m_fAngle[ENGINE::ANGLE_Y] = D3DXToRadian(90.f);
	((CBack*)pGameObject)->m_pInfo->m_vScale = D3DXVECTOR3(250.f, 33.f, 0.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"Back", pGameObject);


	// BackX-
	pGameObject = CBack::Create(m_pDevice);
	((CBack*)pGameObject)->m_pInfo->m_vPos = D3DXVECTOR3(-10.f, 30.f, 125.f);
	((CBack*)pGameObject)->m_pInfo->m_fAngle[ENGINE::ANGLE_X] = D3DXToRadian(270.f);
	((CBack*)pGameObject)->m_pInfo->m_fAngle[ENGINE::ANGLE_Y] = D3DXToRadian(90.f);
	((CBack*)pGameObject)->m_pInfo->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(90.f);
	((CBack*)pGameObject)->m_pInfo->m_vScale = D3DXVECTOR3(250.f, 33.f, 0.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"Back", pGameObject);

	// BackX+
	pGameObject = CBack::Create(m_pDevice);
	((CBack*)pGameObject)->m_pInfo->m_vPos = D3DXVECTOR3(50.f, 30.f, 125.f);
	((CBack*)pGameObject)->m_pInfo->m_fAngle[ENGINE::ANGLE_X] = D3DXToRadian(270.f);
	((CBack*)pGameObject)->m_pInfo->m_fAngle[ENGINE::ANGLE_Y] = D3DXToRadian(90.f);
	((CBack*)pGameObject)->m_pInfo->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(270.f);
	((CBack*)pGameObject)->m_pInfo->m_vScale = D3DXVECTOR3(250.f, 33.f, 0.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"Back", pGameObject);

	// BackZ-
	pGameObject = CBack::Create(m_pDevice);
	((CBack*)pGameObject)->m_pInfo->m_vPos = D3DXVECTOR3(20.f, 30.f, 350.f);
	//((CBack*)pGameObject)->m_pInfo->m_fAngle[ENGINE::ANGLE_X] = D3DXToRadian(180.f);
	//((CBack*)pGameObject)->m_pInfo->m_fAngle[ENGINE::ANGLE_Y] = D3DXToRadian(90.f);
	((CBack*)pGameObject)->m_pInfo->m_vScale = D3DXVECTOR3(33.f, 33.f, 0.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"Back", pGameObject);

	// BackZ+
	pGameObject = CBack::Create(m_pDevice);
	((CBack*)pGameObject)->m_pInfo->m_vPos = D3DXVECTOR3(20.f, 30.f, -30.f);
	((CBack*)pGameObject)->m_pInfo->m_fAngle[ENGINE::ANGLE_X] = D3DXToRadian(180.f);
	//((CBack*)pGameObject)->m_pInfo->m_fAngle[ENGINE::ANGLE_Y] = D3DXToRadian(90.f);
	((CBack*)pGameObject)->m_pInfo->m_vScale = D3DXVECTOR3(33.f, 33.f, 0.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"Back", pGameObject);

	return S_OK;
}

