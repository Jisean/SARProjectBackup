#include "stdafx.h"
#include "Player.h"

#include "Include.h"
#include "Texture.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Pipeline.h"
#include "CameraObserver.h"
#include "CollisionMgr.h"
#include "TerrainCol.h"
#include "MouseCol.h"
#include "Stage.h"
#include "ObjectCol.h"
#include "KeyMgr.h"
#include "CubeObj.h"
#include "PlayerTex.h"
#include "Monster.h"
#include "MonsterTex.h"
#include "Spinblade.h"
#include "SpinObstacle.h"
#include "UpDownCube.h"
#include "SpinCutter.h"
#include "CutterPattern.h"
#include "CrashPattern.h"
#include "SpinSnow.h"
#include "SpinSnowMan.h"
#include "ShrinkingPlatform.h"
#include "CrashMaze.h"
#include "CentryGun.h"
#include "..\..\Tool58\LeftToRight.h"
#include "..\..\Tool58\RightToLeft.h"
#include "..\..\Tool58\CubeStair.h"
#include "RedBull.h"
#include "RedBullTex.h"
#include "HotSix.h"
#include "HotSixTex.h"
#include "Target.h"
#include "Shot.h"
#include "Door.h"
#include "Respawn.h"
#include "Bullet.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pDevice)
: CGameObject(pDevice)
, m_pResourceMgr(ENGINE::Get_ResourcesMgr())
, m_pManagement(ENGINE::Get_Management())
, m_pTimeMgr(ENGINE::Get_Time())
, m_pInfoSubject(ENGINE::Get_InfoSubject())
, m_pCollisionMgr(CCollisionMgr::GetInstance())
, m_pCameraObserver(NULL)
, m_pTerrainVertex(NULL)
, m_pConvertVertex(NULL)
, m_pVertex(NULL)
, m_pTerrainCol(NULL)
, m_pMouseCol(NULL)
, m_pObjectCol(NULL)
, m_pCubeVertex(NULL)
, m_dwVtxCnt(0)
, m_bMove(false)
, m_fJumpAcc(0.f)
, m_fPeak(0.f)
, m_bUp(false)
, m_bDown(true)
, m_bUpDowncol(false)
, m_bYcol(false)
, m_bZcol(false)
, m_pPlayerTex(NULL)
, m_bOnShrink(false)
, m_bFired(false)
, m_fRedBullTime(0)
, m_RespawnPos(0.f,0.f,0.f)
{

}

CPlayer::~CPlayer()
{
	Release();
}

HRESULT CPlayer::Initialize(void)
{
	m_pCameraObserver = CCameraObserver::Create();
	m_pInfoSubject->Subscribe(m_pCameraObserver);

	m_fSpeed = 5.f;
	m_fDistance = 10.f;
	m_fCameraAngle = D3DXToRadian(45.f);

	m_dwVtxCnt = 8;

	m_pVertex			= new ENGINE::VTXCUBE[m_dwVtxCnt];
	m_pConvertVertex	= new ENGINE::VTXCUBE[m_dwVtxCnt];
	m_pColVtx			= new ENGINE::VTXCUBE[m_dwVtxCnt];
	m_pYColVtx			= new ENGINE::VTXCUBE[m_dwVtxCnt];

	
	m_pResourceMgr->GetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer_CubeTex", m_pVertex);

	FAILED_CHECK(AddComponent());
	
	m_pPlayerTex = CPlayerTex::Create(m_pDevice);
	NULL_CHECK_RETURN(m_pPlayerTex, E_FAIL);

	m_RespawnPos = m_pInfo->m_vPos;

	m_Time = GetTickCount();
	m_iOriginX = WINCX / 2;
	m_bTemp = false;

	return S_OK;
}

void CPlayer::Update(void)
{
	KeyInput();
	
	m_pTerrainVertex = m_pManagement->GetTerrainVertex(CStage::LAYER_GAMELOGIC, L"Terrain");

	//m_pCubeVertex = m_pManagement->GetCubeVertex(CStage::LAYER_GAMELOGIC,L"CubeObj");


	Jump();
	if(m_bDown && m_bYcol == false)
	{
		m_pInfo->m_vPos.y = m_pInfo->m_vPos.y - 0.2f;
		if(m_pInfo->m_vPos.y < 1.f)
			m_pTerrainCol->SetColInfo(&m_pInfo->m_vPos, m_pTerrainVertex);		
	}
	if(m_bYcol == true && m_bDown == true && m_bUp == false)
	{
		m_bYcol = false;
	}
	if(m_bYcol)
	{	
		D3DXVECTOR3 SelfMax, SelfMin;
		SelfMax = GetMaxPos(m_pColVtx);
		SelfMin = GetMinPos(m_pColVtx);

		if(SelfMax.x > m_vColObjMaxPos.x)
			m_fColDisX =  fabs(SelfMax.x- m_vColObjMaxPos.x);
		else if(SelfMax.x < m_vColObjMaxPos.x)
			m_fColDisX =  fabs(m_vColObjMinPos.x - SelfMin.x);

		if(SelfMax.z > m_vColObjMaxPos.z)
			m_fColDisZ =  fabs(SelfMax.z- m_vColObjMaxPos.z);
		else if(SelfMax.z < m_vColObjMaxPos.z)
			m_fColDisZ =  fabs(m_vColObjMinPos.z - SelfMin.z);

		/*cout << "X 거리 : " << m_fColDisX << endl;
		cout << "Z 거리 : " << m_fColDisZ << endl;*/
		float fDistanceX, fDistanceZ;
		fDistanceX = m_vColObjMaxPos.x - m_vColObjMinPos.x;
		fDistanceZ = m_vColObjMaxPos.z - m_vColObjMinPos.z;

		
		if(m_fColDisX > fDistanceX || m_fColDisZ > fDistanceZ)
		{
			m_bYcol = false;
			m_bDown = true;
		}
	}
	

	cout << "x : " << m_pInfo->m_vPos.x << endl;
	cout << "y : " << m_pInfo->m_vPos.y << endl;
	cout << "z : " << m_pInfo->m_vPos.z << endl;


	D3DXVec3TransformNormal(&m_pInfo->m_vDir, &g_vLook, &m_pInfo->m_matWorld);
	D3DXVec3Normalize(&m_pInfo->m_vDir, &m_pInfo->m_vDir);

	if(m_bMove)
		Move();

	ENGINE::CGameObject::Update();

	CubeCollision();


	SetTransForm();
	
	if(m_pPlayerTex != NULL)
	{
		m_pPlayerTex->SetPlayerTex(&m_pInfo->m_vPos);
		m_pPlayerTex->Update();
	}

	/*if(bFire == true && m_bFired == false)
	{
		FireBullet();
		m_bFired = true;
	}
	if(bFire == false)
		m_bFired = false;*/

	if(!bKnife)
	{
		if(CKeyMgr::GetInstance()->KeyDown(VK_LBUTTON))
			FireBullet();
	}

	if(bStage2)
		BookCheck();

	if(bStage1)
		CentryCheck();

	if(bRedBull)
	{
		m_fRedBullTime += m_pTimeMgr->GetTime();
	
		
	
		if(m_fRedBullTime > 10.f)
		{
			m_fRedBullTime = 0.f;
			bRedBull = false;
		}
	}
	
}

void CPlayer::Render(void)
{
	//m_pDevice->SetTransform(D3DTS_WORLD, &m_pInfo->m_matWorld);

	//m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00000088);
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pResourceMgr->SetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer_CubeTex", m_pConvertVertex);

	//m_pTexture->Render(0);

	//m_pBuffer->Render();

	if(m_pPlayerTex != NULL)
		m_pPlayerTex->Render();

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	
}

void CPlayer::Release(void)
{
	m_pInfoSubject->UnSubscribe(m_pCameraObserver);
	ENGINE::Safe_Delete(m_pCameraObserver);

	if(m_pVertex)
		ENGINE::Safe_Delete_Array(m_pVertex);

	if(m_pConvertVertex)
		ENGINE::Safe_Delete_Array(m_pConvertVertex);
	
	if(m_pColVtx)
		ENGINE::Safe_Delete_Array(m_pColVtx);

	if(m_pYColVtx)
		ENGINE::Safe_Delete_Array(m_pYColVtx);

	if(m_pCameraObserver)
		ENGINE::Safe_Delete(m_pCameraObserver);

	if(m_pPlayerTex)
		ENGINE::Safe_Delete(m_pPlayerTex);
}

void CPlayer::KeyInput(void)
{
	float fTime = m_pTimeMgr->GetTime();
	
	D3DXVECTOR3 vSide;
	D3DXVECTOR3 vUp;
	D3DXVECTOR3 vLook;
	memcpy(&vUp, &m_pInfo->m_matWorld.m[1][0], sizeof(float) * 3);
	memcpy(&vLook, &m_pInfo->m_matWorld.m[2][0], sizeof(float) * 3);

	POINT		ptMouse;

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	ptMouse.x;
	ptMouse.y;
	if(GetAsyncKeyState('W'))
	{
		m_pInfo->m_vPos += m_pInfo->m_vDir * fTime * m_fSpeed * 2;
	}

	if(GetAsyncKeyState('S'))
	{
		m_pInfo->m_vPos -= m_pInfo->m_vDir * fTime * m_fSpeed * 2;
	}

	if(GetAsyncKeyState('D'))
	{
		D3DXVec3Cross(&vSide, &vUp, &vLook);
		m_pInfo->m_vPos += vSide * fTime * m_fSpeed;
	}

	if(GetAsyncKeyState('A'))
	{
		D3DXVec3Cross(&vSide, &vLook, &vUp);
		m_pInfo->m_vPos += vSide * fTime * m_fSpeed;
	}

	if(GetAsyncKeyState('T') && 0x8000)
	{
		m_pInfo->m_vPos = m_RespawnPos;
	}
	if(m_Time + 50 < GetTickCount())
	{
		m_Time = GetTickCount();
		m_iOriginX = ptMouse.x;
	}
	if(ptMouse.x < m_iOriginX)
	{
		m_pInfo->m_fAngle[ENGINE::ANGLE_Y] -= D3DXToRadian(100.f) * fTime * 1.5f;
	}

	if(ptMouse.x > m_iOriginX)
	{
		m_pInfo->m_fAngle[ENGINE::ANGLE_Y] += D3DXToRadian(100.f) * fTime * 1.5f;
	}
	//if(ptMouse.x >= WINCX || ptMouse.x <= 0)
	//{
		POINT pt = { 0, 0};
		ClientToScreen(g_hWnd, &pt);
		SetCursorPos(pt.x + WINCX / 2, pt.y +ptMouse.y);
		m_iOriginX = WINCX / 2;
	//}


	/*if(ptMouse.y <= WINCY - 600)
	{
		m_pInfo->m_fAngle[ENGINE::ANGLE_X] += D3DXToRadian(40.f) * fTime;
	}
	if(ptMouse.y >= WINCY)
	{
		m_pInfo->m_fAngle[ENGINE::ANGLE_X] -= D3DXToRadian(40.f) * fTime;
	}*/

	if(CKeyMgr::GetInstance()->KeyPressed(KEY_SPACE))
	{
		m_fBeforY = m_pInfo->m_vPos.y;
		m_bDown = false;
		m_bYcol = false;
		m_bUp = true;
	}
	/*if(GetAsyncKeyState('1'))
	{
		cout << "y :" << m_bYcol << endl;
		cout << "up :" <<m_bUp << endl;
		cout << "down :" <<m_bDown << endl;
	}*/
	//cout << m_pInfo->m_vPos.y << endl;

	//if(GetAsyncKeyState(VK_LBUTTON))
	//{
	//	m_bMove = true;

	//	m_pMouseCol->PickTerrain(&m_vDestPos, m_pTerrainVertex);

	//	m_pMouseCol->PickObject(&m_vDestPos, m_pConvertVertex, &m_pInfo->m_matWorld);
	//}

	//D3DXComputeBoundingBox()
}

HRESULT CPlayer::AddComponent(void)
{
	ENGINE::CComponent*			pComponent = NULL;

	// Transform
	pComponent = m_pInfo = ENGINE::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	// Buffer
	pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, L"Buffer_CubeTex");
	m_pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	// Collision Mouse
	pComponent = m_pCollisionMgr->CloneColObject(CCollisionMgr::COL_MOUSE);
	m_pMouseCol = dynamic_cast<CMouseCol*>(pComponent);
	NULL_CHECK_RETURN(m_pMouseCol, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"MouseCol", pComponent));

	// Collision Terrain
	pComponent = m_pCollisionMgr->CloneColObject(CCollisionMgr::COL_TERRAIN);
	m_pTerrainCol = dynamic_cast<CTerrainCol*>(pComponent);
	NULL_CHECK_RETURN(m_pTerrainCol, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"TerrainCol", pComponent));

	// Collision Object
	pComponent = m_pCollisionMgr->CloneColObject(CCollisionMgr::COL_OBJ);
	m_pObjectCol = dynamic_cast<CObjectCol*>(pComponent);
	NULL_CHECK_RETURN(m_pObjectCol, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"ObjectCol", pComponent));


	// Texture
	pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_STATIC, L"Monster1");
	m_pTexture = dynamic_cast<ENGINE::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));


	return S_OK;
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CPlayer* pPlayer = new CPlayer(pDevice);

	if(FAILED(pPlayer->Initialize()))
		ENGINE::Safe_Delete(pPlayer);

	return pPlayer;
}


void CPlayer::SetTransForm(void)
{
	/*for(int i = 0 ; i < m_dwVtxCnt; ++i)
	{
		m_pVertex[i] *= matworld;
		m_pVertex[i] *= matView;
		m_pVertex[i] *= matProj;
	}*/

	const D3DXMATRIX*		pMatView = m_pCameraObserver->GetView();
	const D3DXMATRIX*		pMatProj = m_pCameraObserver->GetProj();

	for(size_t i = 0; i < m_dwVtxCnt; ++i)
	{
		m_pConvertVertex[i] = m_pVertex[i];

		// world
		ENGINE::CPipeline::MyTransformCoord(&m_pConvertVertex[i].vPos, 
			&m_pConvertVertex[i].vPos, &m_pInfo->m_matWorld);
		m_pColVtx[i] = m_pConvertVertex[i];

		// view
		ENGINE::CPipeline::MyTransformCoord(&m_pConvertVertex[i].vPos, 
			&m_pConvertVertex[i].vPos, pMatView);

		if(m_pConvertVertex[i].vPos.z < 1.f)
			m_pConvertVertex[i].vPos.z = 1.f;

		// proj
		ENGINE::CPipeline::MyTransformCoord(&m_pConvertVertex[i].vPos, 
			&m_pConvertVertex[i].vPos, pMatProj);
	}

}

void CPlayer::Move(void)
{
	D3DXVECTOR3		vDir;

	vDir = m_vDestPos - m_pInfo->m_vPos;

	float fDistance = D3DXVec3Length(&vDir);
	D3DXVec3Normalize(&vDir, &vDir);

	m_pInfo->m_vPos += vDir * m_fSpeed * m_pTimeMgr->GetTime();

	if(fDistance < 1.f)
		m_bMove = false;
}

void CPlayer::Jump(void)
{
	D3DXVECTOR3 vHeight = m_pInfo->m_vPos;
	m_pTerrainCol->SetColInfo(&vHeight, m_pTerrainVertex);
	if(m_bUp)
	{	
		m_fJumpAcc += 0.1f;
		m_pInfo->m_vPos.y -= 0.98f * m_fJumpAcc * m_fJumpAcc * m_pTimeMgr->GetTime() * 0.2f - 0.1f;
		if(m_bUpDowncol == true)
			m_pInfo->m_vPos -= m_pInfo->m_vDir * m_fSpeed * m_pTimeMgr->GetTime();
	}
	if(m_bYcol == false && m_pInfo->m_vPos.y <= vHeight.y)
	{
		m_bUp	 = false;
		m_bDown  = true;
		m_fJumpAcc = 0.f;
		m_fPeak = 0.f;	
		m_bUpDowncol = false;
	}
	if(m_bYcol == true)
	{
		m_bUp	 = false;
		m_fJumpAcc = 0.f; 
		m_fPeak = 0.f;
		m_bUpDowncol = false;
	}
	
	
}
void CPlayer::CubeCollision(void)
{
	D3DXVECTOR3 SelfMax, SelfMin;
	D3DXVECTOR3 TargetMax, TargetMin;


	SelfMax = GetMaxPos(m_pColVtx);
	SelfMin = GetMinPos(m_pColVtx);

	list<ENGINE::CGameObject*>* pList = m_pManagement->GetList(CStage::LAYER_GAMELOGIC, L"CubeObj");

	if(pList != NULL)
	{
		list<ENGINE::CGameObject*>::iterator iter = pList->begin();
		list<ENGINE::CGameObject*>::iterator iter_end = pList->end();

		for(; iter != iter_end; ++iter)
		{
			D3DXVECTOR3 vPushDir = m_pInfo->m_vPos - *(((CCubeObj*)(*iter))->GetPos());
			D3DXVec3Normalize(&vPushDir, &vPushDir);
			TargetMax = GetMaxPos((*iter)->GetCubeVertex());
			TargetMin = GetMinPos((*iter)->GetCubeVertex());

			const D3DXVECTOR3* fCubePos = (*iter)->GetPos();

			float fItvX = fabs(m_pInfo->m_vPos.x - fCubePos->x) / ((CCubeObj*)(*iter))->GetInfo()->m_vScale.x;
			float fItvY = fabs(m_pInfo->m_vPos.y - fCubePos->y) / ((CCubeObj*)(*iter))->GetInfo()->m_vScale.y;
			float fItvZ = fabs(m_pInfo->m_vPos.z - fCubePos->z) / ((CCubeObj*)(*iter))->GetInfo()->m_vScale.z;
			

			if((m_pObjectCol->SetColCheck2(&SelfMax,&SelfMin,&TargetMax,&TargetMin) == true))
			{			
				if(fItvZ > fItvY && fItvZ > fItvX)
				{
					m_pInfo->m_vPos += vPushDir * 0.4f;
					m_bZcol = false;
				}
				else if(fItvX > fItvY && fItvX > fItvZ)
				{
					m_pInfo->m_vPos += vPushDir * 0.4f;
					m_bZcol = false;
				}

				else if(fItvY > fItvX && fItvY > fItvZ)
				{				
					/*m_vColObjMaxPos = GetMaxPos((*iter)->GetCubeVertex());
					m_vColObjMinPos = GetMinPos((*iter)->GetCubeVertex());
					m_pInfo->m_vPos.y = m_pInfo->m_vPos.y + 0.1f;
					m_bYcol = true;*/
				}
			}
		}
	}

	list<ENGINE::CGameObject*>* pRedBullList = m_pManagement->GetList(CStage::LAYER_GAMELOGIC, L"RedBull");
	if(pRedBullList != NULL)
	{
		list<ENGINE::CGameObject*>::iterator iter = pRedBullList->begin();
		list<ENGINE::CGameObject*>::iterator iter_end = pRedBullList->end();

		for(; iter != iter_end; )
		{
			D3DXVECTOR3 vPushDir = m_pInfo->m_vPos - *(((CRedBull*)(*iter))->GetPos());
			float fDistance = D3DXVec3Length(&vPushDir);
			if(fDistance < 1.f)
			{
				ENGINE::Safe_Delete((*iter));
				iter = pRedBullList->erase(iter);
				CSoundMgr::GetInstance()->PlaySkillSound(L"Unbeatable.mp3");
				bRedBull = true;
			}
			else
				++iter;
		}
	}
	list<ENGINE::CGameObject*>* pHotSixList = m_pManagement->GetList(CStage::LAYER_GAMELOGIC, L"HotSix");
	if(pHotSixList != NULL)
	{
		list<ENGINE::CGameObject*>::iterator iter = pHotSixList->begin();
		list<ENGINE::CGameObject*>::iterator iter_end = pHotSixList->end();

		for(; iter != iter_end; )
		{
			D3DXVECTOR3 vPushDir = m_pInfo->m_vPos - *(((CHotSix*)(*iter))->GetPos());
			float fDistance = D3DXVec3Length(&vPushDir);
			if(fDistance < 1.f)
			{
				ENGINE::Safe_Delete((*iter));
				iter = pHotSixList->erase(iter);
				fHp += 30;
				if(fHp >= 100)
					fHp = 100;
			}
			else
				++iter;
		}
	}

	list<ENGINE::CGameObject*>* pStairList = m_pManagement->GetList(CStage::LAYER_GAMELOGIC, L"CubeStair");

	if(pStairList != NULL)
	{
		list<ENGINE::CGameObject*>::iterator iter = pStairList->begin();
		list<ENGINE::CGameObject*>::iterator iter_end = pStairList->end();

		for(; iter != iter_end; ++iter)
		{
			D3DXVECTOR3 vPushDir = m_pInfo->m_vPos - *(((CCubeStair*)(*iter))->GetPos());
			D3DXVec3Normalize(&vPushDir, &vPushDir);
			TargetMax = GetMaxPos((*iter)->GetCubeVertex());
			TargetMin = GetMinPos((*iter)->GetCubeVertex());

			const D3DXVECTOR3* fCubePos = (*iter)->GetPos();

			float fItvX = fabs(m_pInfo->m_vPos.x - fCubePos->x) / ((CCubeStair*)(*iter))->GetInfo()->m_vScale.x;
			float fItvY = fabs(m_pInfo->m_vPos.y - fCubePos->y) / ((CCubeStair*)(*iter))->GetInfo()->m_vScale.y;
			float fItvZ = fabs(m_pInfo->m_vPos.z - fCubePos->z) / ((CCubeStair*)(*iter))->GetInfo()->m_vScale.z;
			

			if((m_pObjectCol->SetColCheck2(&SelfMax,&SelfMin,&TargetMax,&TargetMin) == true))
			{			
				if(fItvZ > fItvY && fItvZ > fItvX)
				{
					m_pInfo->m_vPos += vPushDir * 0.4f;
					m_bZcol = false;
				}
				else if(fItvX > fItvY && fItvX > fItvZ)
				{
					m_pInfo->m_vPos += vPushDir * 0.4f;
					m_bZcol = false;
				}

				else if(fItvY > fItvX && fItvY > fItvZ)
				{				
					m_vColObjMaxPos = GetMaxPos((*iter)->GetCubeVertex());
					m_vColObjMinPos = GetMinPos((*iter)->GetCubeVertex());
					m_pInfo->m_vPos.y = m_pInfo->m_vPos.y + 0.1f;
					m_bYcol = true;
				}
			}
		}
	}

	list<ENGINE::CGameObject*>* pLeftToRight = m_pManagement->GetList(CStage::LAYER_GAMELOGIC, L"LeftToRight");
	if(pLeftToRight != NULL)
	{
		list<ENGINE::CGameObject*>::iterator iter = pLeftToRight->begin();
		list<ENGINE::CGameObject*>::iterator iter_end = pLeftToRight->end();

		for(; iter != iter_end; ++iter)
		{
			D3DXVECTOR3 vPushDir = m_pInfo->m_vPos - *(((CLeftToRight*)(*iter))->GetPos());
			D3DXVec3Normalize(&vPushDir, &vPushDir);
			TargetMax = GetMaxPos((*iter)->GetCubeVertex());
			TargetMin = GetMinPos((*iter)->GetCubeVertex());

			const D3DXVECTOR3* fCubePos = (*iter)->GetPos();

			float fItvX = fabs(m_pInfo->m_vPos.x - fCubePos->x) / ((CLeftToRight*)(*iter))->GetInfo()->m_vScale.x;
			float fItvY = fabs(m_pInfo->m_vPos.y - fCubePos->y) / ((CLeftToRight*)(*iter))->GetInfo()->m_vScale.y;
			float fItvZ = fabs(m_pInfo->m_vPos.z - fCubePos->z) / ((CLeftToRight*)(*iter))->GetInfo()->m_vScale.z;


			if((m_pObjectCol->SetColCheck2(&SelfMax,&SelfMin,&TargetMax,&TargetMin) == true))
			{		
				if(!bRedBull)
					fHp -= 0.2f;
				if(fItvZ > fItvY && fItvZ > fItvX)
				{
					m_pInfo->m_vPos += vPushDir * 0.1f;
					m_bZcol = false;
				}
				else if(fItvX > fItvY && fItvX > fItvZ)
				{
					m_pInfo->m_vPos += vPushDir * 0.1f;
					m_bZcol = false;
				}

				else if(fItvY > fItvX && fItvY > fItvZ)
				{				
					m_vColObjMaxPos = GetMaxPos((*iter)->GetCubeVertex());
					m_vColObjMinPos = GetMinPos((*iter)->GetCubeVertex());
					m_pInfo->m_vPos.y = m_pInfo->m_vPos.y + 0.1f;
					m_bYcol = true;
				}
			}
		}
	}

	list<ENGINE::CGameObject*>* pRightToLeft = m_pManagement->GetList(CStage::LAYER_GAMELOGIC, L"RightToLeft");
	if(pRightToLeft != NULL)
	{
		list<ENGINE::CGameObject*>::iterator iter = pRightToLeft->begin();
		list<ENGINE::CGameObject*>::iterator iter_end = pRightToLeft->end();

		for(; iter != iter_end; ++iter)
		{
			D3DXVECTOR3 vPushDir = m_pInfo->m_vPos - *(((CRightToLeft*)(*iter))->GetPos());
			D3DXVec3Normalize(&vPushDir, &vPushDir);
			TargetMax = GetMaxPos((*iter)->GetCubeVertex());
			TargetMin = GetMinPos((*iter)->GetCubeVertex());

			const D3DXVECTOR3* fCubePos = (*iter)->GetPos();

			float fItvX = fabs(m_pInfo->m_vPos.x - fCubePos->x) / ((CRightToLeft*)(*iter))->GetInfo()->m_vScale.x;
			float fItvY = fabs(m_pInfo->m_vPos.y - fCubePos->y) / ((CRightToLeft*)(*iter))->GetInfo()->m_vScale.y;
			float fItvZ = fabs(m_pInfo->m_vPos.z - fCubePos->z) / ((CRightToLeft*)(*iter))->GetInfo()->m_vScale.z;


			if((m_pObjectCol->SetColCheck2(&SelfMax,&SelfMin,&TargetMax,&TargetMin) == true))
			{	
				if(!bRedBull)
					fHp -= 0.2f;
				if(fItvZ > fItvY && fItvZ > fItvX)
				{
					m_pInfo->m_vPos += vPushDir * 0.1f;
					m_bZcol = false;
				}
				else if(fItvX > fItvY && fItvX > fItvZ)
				{
					m_pInfo->m_vPos += vPushDir * 0.1f;
					m_bZcol = false;
				}	

				else if(fItvY > fItvX && fItvY > fItvZ)
				{				
					m_vColObjMaxPos = GetMaxPos((*iter)->GetCubeVertex());
					m_vColObjMinPos = GetMinPos((*iter)->GetCubeVertex());
					m_pInfo->m_vPos.y = m_pInfo->m_vPos.y + 0.1f;
					m_bYcol = true;
				}
			}
		}
	}

	list<ENGINE::CGameObject*>* pMazeList = m_pManagement->GetList(CStage::LAYER_GAMELOGIC, L"CrashMaze");

	if(pMazeList != NULL)
	{
		list<ENGINE::CGameObject*>::iterator iter = pMazeList->begin();
		list<ENGINE::CGameObject*>::iterator iter_end = pMazeList->end();

		for(; iter != iter_end; ++iter)
		{
			if(((CCrashMaze*)(*iter))->m_pPattern1 != NULL)
			{
				D3DXVECTOR3 vPushDir = m_pInfo->m_vPos - *(((CCrashMaze*)(*iter))->m_pPattern1->GetPos());
				D3DXVec3Normalize(&vPushDir, &vPushDir);
				TargetMax = GetMaxPos(((CCrashMaze*)(*iter))->m_pPattern1->GetCubeVertex());
				TargetMin = GetMinPos(((CCrashMaze*)(*iter))->m_pPattern1->GetCubeVertex());

				const D3DXVECTOR3* fCubePos = ((CCrashMaze*)(*iter))->m_pPattern1->GetPos();

				float fItvX = fabs(m_pInfo->m_vPos.x - fCubePos->x) / ((CPattern2*)((CCrashMaze*)(*iter))->m_pPattern1)->GetInfo()->m_vScale.x;
				float fItvY = fabs(m_pInfo->m_vPos.y - fCubePos->y) / ((CPattern2*)((CCrashMaze*)(*iter))->m_pPattern1)->GetInfo()->m_vScale.y;
				float fItvZ = fabs(m_pInfo->m_vPos.z - fCubePos->z) / ((CPattern2*)((CCrashMaze*)(*iter))->m_pPattern1)->GetInfo()->m_vScale.z;


				if((m_pObjectCol->SetColCheck2(&SelfMax,&SelfMin,&TargetMax,&TargetMin) == true))
				{	
					if(!bRedBull)
						fHp -= 0.2f;
					if(fItvZ > fItvY && fItvZ > fItvX)
					{
						m_pInfo->m_vPos += vPushDir * 0.1f;
						m_bZcol = false;
					}
					else if(fItvX > fItvY && fItvX > fItvZ)
					{
						m_pInfo->m_vPos += vPushDir * 0.1f;
						m_bZcol = false;
					}

					else if(fItvY > fItvX && fItvY > fItvZ)
					{				
						m_vColObjMaxPos = GetMaxPos(((CCrashMaze*)(*iter))->m_pPattern1->GetCubeVertex());
						m_vColObjMinPos = GetMinPos(((CCrashMaze*)(*iter))->m_pPattern1->GetCubeVertex());
						m_pInfo->m_vPos.y = m_pInfo->m_vPos.y + 0.1f;
						m_bYcol = true;
					}
				}
			}
			if(((CCrashMaze*)(*iter))->m_pPattern2 != NULL)
			{
				D3DXVECTOR3 vPushDir = m_pInfo->m_vPos - *(((CCrashMaze*)(*iter))->m_pPattern2->GetPos());
				D3DXVec3Normalize(&vPushDir, &vPushDir);
				TargetMax = GetMaxPos(((CCrashMaze*)(*iter))->m_pPattern2->GetCubeVertex());
				TargetMin = GetMinPos(((CCrashMaze*)(*iter))->m_pPattern2->GetCubeVertex());

				const D3DXVECTOR3* fCubePos = ((CCrashMaze*)(*iter))->m_pPattern2->GetPos();

				float fItvX = fabs(m_pInfo->m_vPos.x - fCubePos->x) / ((CPattern2*)((CCrashMaze*)(*iter))->m_pPattern2)->GetInfo()->m_vScale.x;
				float fItvY = fabs(m_pInfo->m_vPos.y - fCubePos->y) / ((CPattern2*)((CCrashMaze*)(*iter))->m_pPattern2)->GetInfo()->m_vScale.y;
				float fItvZ = fabs(m_pInfo->m_vPos.z - fCubePos->z) / ((CPattern2*)((CCrashMaze*)(*iter))->m_pPattern2)->GetInfo()->m_vScale.z;


				if((m_pObjectCol->SetColCheck2(&SelfMax,&SelfMin,&TargetMax,&TargetMin) == true))
				{	
					if(!bRedBull)
						fHp -= 0.2f;
					if(fItvZ > fItvY && fItvZ > fItvX)
					{
						m_pInfo->m_vPos += vPushDir * 0.1f;
						m_bZcol = false;
					}
					else if(fItvX > fItvY && fItvX > fItvZ)
					{
						m_pInfo->m_vPos += vPushDir * 0.1f;
						m_bZcol = false;
					}

					else if(fItvY > fItvX && fItvY > fItvZ)
					{				
						m_vColObjMaxPos = GetMaxPos(((CCrashMaze*)(*iter))->m_pPattern2->GetCubeVertex());
						m_vColObjMinPos = GetMinPos(((CCrashMaze*)(*iter))->m_pPattern2->GetCubeVertex());
						m_pInfo->m_vPos.y = m_pInfo->m_vPos.y + 0.1f;
						m_bYcol = true;
					}
				}
			}
			if(((CCrashMaze*)(*iter))->m_pPattern3 != NULL)
			{
				D3DXVECTOR3 vPushDir = m_pInfo->m_vPos - *(((CCrashMaze*)(*iter))->m_pPattern3->GetPos());
				D3DXVec3Normalize(&vPushDir, &vPushDir);
				TargetMax = GetMaxPos(((CCrashMaze*)(*iter))->m_pPattern3->GetCubeVertex());
				TargetMin = GetMinPos(((CCrashMaze*)(*iter))->m_pPattern3->GetCubeVertex());

				const D3DXVECTOR3* fCubePos = ((CCrashMaze*)(*iter))->m_pPattern3->GetPos();

				float fItvX = fabs(m_pInfo->m_vPos.x - fCubePos->x) / ((CPattern2*)((CCrashMaze*)(*iter))->m_pPattern3)->GetInfo()->m_vScale.x;
				float fItvY = fabs(m_pInfo->m_vPos.y - fCubePos->y) / ((CPattern2*)((CCrashMaze*)(*iter))->m_pPattern3)->GetInfo()->m_vScale.y;
				float fItvZ = fabs(m_pInfo->m_vPos.z - fCubePos->z) / ((CPattern2*)((CCrashMaze*)(*iter))->m_pPattern3)->GetInfo()->m_vScale.z;


				if((m_pObjectCol->SetColCheck2(&SelfMax,&SelfMin,&TargetMax,&TargetMin) == true))
				{	
					if(!bRedBull)
						fHp -= 0.2f;
					if(fItvZ > fItvY && fItvZ > fItvX)
					{
						m_pInfo->m_vPos += vPushDir * 0.1f;
						m_bZcol = false;
					}
					else if(fItvX > fItvY && fItvX > fItvZ)
					{
						m_pInfo->m_vPos += vPushDir * 0.1f;
						m_bZcol = false;
					}

					else if(fItvY > fItvX && fItvY > fItvZ)
					{				
						m_vColObjMaxPos = GetMaxPos(((CCrashMaze*)(*iter))->m_pPattern3->GetCubeVertex());
						m_vColObjMinPos = GetMinPos(((CCrashMaze*)(*iter))->m_pPattern3->GetCubeVertex());
						m_pInfo->m_vPos.y = m_pInfo->m_vPos.y + 0.1f;
						m_bYcol = true;
					}
				}
			}
			if(((CCrashMaze*)(*iter))->m_pPattern4 != NULL)
			{
				D3DXVECTOR3 vPushDir = m_pInfo->m_vPos - *(((CCrashMaze*)(*iter))->m_pPattern4->GetPos());
				D3DXVec3Normalize(&vPushDir, &vPushDir);
				TargetMax = GetMaxPos(((CCrashMaze*)(*iter))->m_pPattern4->GetCubeVertex());
				TargetMin = GetMinPos(((CCrashMaze*)(*iter))->m_pPattern4->GetCubeVertex());

				const D3DXVECTOR3* fCubePos = ((CCrashMaze*)(*iter))->m_pPattern4->GetPos();

				float fItvX = fabs(m_pInfo->m_vPos.x - fCubePos->x) / ((CPattern2*)((CCrashMaze*)(*iter))->m_pPattern4)->GetInfo()->m_vScale.x;
				float fItvY = fabs(m_pInfo->m_vPos.y - fCubePos->y) / ((CPattern2*)((CCrashMaze*)(*iter))->m_pPattern4)->GetInfo()->m_vScale.y;
				float fItvZ = fabs(m_pInfo->m_vPos.z - fCubePos->z) / ((CPattern2*)((CCrashMaze*)(*iter))->m_pPattern4)->GetInfo()->m_vScale.z;


				if((m_pObjectCol->SetColCheck2(&SelfMax,&SelfMin,&TargetMax,&TargetMin) == true))
				{	
					if(!bRedBull)
						fHp -= 0.2f;
					if(fItvZ > fItvY && fItvZ > fItvX)
					{
						m_pInfo->m_vPos += vPushDir * 0.1f;
						m_bZcol = false;
					}
					else if(fItvX > fItvY && fItvX > fItvZ)
					{
						m_pInfo->m_vPos += vPushDir * 0.1f;
						m_bZcol = false;
					}

					else if(fItvY > fItvX && fItvY > fItvZ)
					{				
						m_vColObjMaxPos = GetMaxPos(((CCrashMaze*)(*iter))->m_pPattern4->GetCubeVertex());
						m_vColObjMinPos = GetMinPos(((CCrashMaze*)(*iter))->m_pPattern4->GetCubeVertex());
						m_pInfo->m_vPos.y = m_pInfo->m_vPos.y + 0.1f;
						m_bYcol = true;
					}
				}
			}
			if(((CCrashMaze*)(*iter))->m_pPattern5 != NULL)
			{
				D3DXVECTOR3 vPushDir = m_pInfo->m_vPos - *(((CCrashMaze*)(*iter))->m_pPattern5->GetPos());
				D3DXVec3Normalize(&vPushDir, &vPushDir);
				TargetMax = GetMaxPos(((CCrashMaze*)(*iter))->m_pPattern5->GetCubeVertex());
				TargetMin = GetMinPos(((CCrashMaze*)(*iter))->m_pPattern5->GetCubeVertex());

				const D3DXVECTOR3* fCubePos = ((CCrashMaze*)(*iter))->m_pPattern5->GetPos();

				float fItvX = fabs(m_pInfo->m_vPos.x - fCubePos->x) / ((CPattern2*)((CCrashMaze*)(*iter))->m_pPattern5)->GetInfo()->m_vScale.x;
				float fItvY = fabs(m_pInfo->m_vPos.y - fCubePos->y) / ((CPattern2*)((CCrashMaze*)(*iter))->m_pPattern5)->GetInfo()->m_vScale.y;
				float fItvZ = fabs(m_pInfo->m_vPos.z - fCubePos->z) / ((CPattern2*)((CCrashMaze*)(*iter))->m_pPattern5)->GetInfo()->m_vScale.z;


				if((m_pObjectCol->SetColCheck2(&SelfMax,&SelfMin,&TargetMax,&TargetMin) == true))
				{	
					if(!bRedBull)
						fHp -= 0.2f;
					if(fItvZ > fItvY && fItvZ > fItvX)
					{
						m_pInfo->m_vPos += vPushDir * 0.1f;
						m_bZcol = false;
					}
					else if(fItvX > fItvY && fItvX > fItvZ)
					{
						m_pInfo->m_vPos += vPushDir * 0.1f;
						m_bZcol = false;
					}

					else if(fItvY > fItvX && fItvY > fItvZ)
					{				
						m_vColObjMaxPos = GetMaxPos(((CCrashMaze*)(*iter))->m_pPattern5->GetCubeVertex());
						m_vColObjMinPos = GetMinPos(((CCrashMaze*)(*iter))->m_pPattern5->GetCubeVertex());
						m_pInfo->m_vPos.y = m_pInfo->m_vPos.y + 0.1f;
						m_bYcol = true;
					}
				}
			}
		}
	}

	list<ENGINE::CGameObject*>* pShrink = m_pManagement->GetList(CStage::LAYER_GAMELOGIC, L"ShrinkingPlatform");
	if(pShrink != NULL)
	{
		list<ENGINE::CGameObject*>::iterator iter = pShrink->begin();
		list<ENGINE::CGameObject*>::iterator iter_end = pShrink->end();

		for(; iter != iter_end; ++iter)
		{
			if(((CShrinkingPlatform*)(*iter))->m_pPattern1 != NULL)
			{
				m_bOnShrink = true;
				D3DXVECTOR3 vPushDir = m_pInfo->m_vPos - *((CShrinkingPlatform*)(*iter))->m_pPattern1->GetPos();
				D3DXVec3Normalize(&vPushDir, &vPushDir);
				TargetMax = GetMaxPos(((CShrinkingPlatform*)(*iter))->m_pPattern1->GetCubeVertex());
				TargetMin = GetMinPos(((CShrinkingPlatform*)(*iter))->m_pPattern1->GetCubeVertex());

				const D3DXVECTOR3* fCubePos = ((CShrinkingPlatform*)(*iter))->m_pPattern1->GetPos();

				// x - 10 y - 0.5 z -5
				float fItvX = fabs(m_pInfo->m_vPos.x - fCubePos->x) / ((CPattern2*)((CShrinkingPlatform*)(*iter))->m_pPattern1)->GetInfo()->m_vScale.x;
				float fItvY = fabs(m_pInfo->m_vPos.y - fCubePos->y) / ((CPattern2*)((CShrinkingPlatform*)(*iter))->m_pPattern1)->GetInfo()->m_vScale.y;
				float fItvZ = fabs(m_pInfo->m_vPos.z - fCubePos->z) / ((CPattern2*)((CShrinkingPlatform*)(*iter))->m_pPattern1)->GetInfo()->m_vScale.z;


				fItvX = fItvX / 10;
				fItvY = fItvY * 2;
				fItvZ = fItvZ / 5;

				if((m_pObjectCol->SetColCheck2(&SelfMax,&SelfMin,&TargetMax,&TargetMin) == true))
				{
					if(fItvZ > fItvY && fItvZ > fItvX && m_bYcol == false)
					{
						m_pInfo->m_vPos += vPushDir * 0.1f;
						m_bZcol = false;
					}
					else if(fItvX > fItvY && fItvX > fItvZ && m_bYcol == false)
					{
						m_pInfo->m_vPos += vPushDir * 0.1f;
						m_bZcol = false;
					}
					else if(fItvY > fItvX && fItvY > fItvZ)
					{				
						m_vColObjMaxPos = GetMaxPos(((CShrinkingPlatform*)(*iter))->m_pPattern2->GetCubeVertex());
						m_vColObjMinPos = GetMinPos(((CShrinkingPlatform*)(*iter))->m_pPattern2->GetCubeVertex());
						m_pInfo->m_vPos.y = m_pInfo->m_vPos.y + 0.1f;
						m_bYcol = true;
						m_bDown = true;
						m_bUp = false;
					}
				}
			}
			else
				m_bOnShrink = false;
			if(((CShrinkingPlatform*)(*iter))->m_pPattern2 != NULL)
			{
				m_bOnShrink = true;
				D3DXVECTOR3 vPushDir = m_pInfo->m_vPos - *((CShrinkingPlatform*)(*iter))->m_pPattern2->GetPos();
				D3DXVec3Normalize(&vPushDir, &vPushDir);
				TargetMax = GetMaxPos(((CShrinkingPlatform*)(*iter))->m_pPattern2->GetCubeVertex());
				TargetMin = GetMinPos(((CShrinkingPlatform*)(*iter))->m_pPattern2->GetCubeVertex());

				const D3DXVECTOR3* fCubePos = ((CShrinkingPlatform*)(*iter))->m_pPattern2->GetPos();
				float fItvX = fabs(m_pInfo->m_vPos.x - fCubePos->x) / ((CPattern2*)((CShrinkingPlatform*)(*iter))->m_pPattern2)->GetInfo()->m_vScale.x;
				float fItvY = fabs(m_pInfo->m_vPos.y - fCubePos->y) / ((CPattern2*)((CShrinkingPlatform*)(*iter))->m_pPattern2)->GetInfo()->m_vScale.y;
				float fItvZ = fabs(m_pInfo->m_vPos.z - fCubePos->z) / ((CPattern2*)((CShrinkingPlatform*)(*iter))->m_pPattern2)->GetInfo()->m_vScale.z;

				fItvX = fItvX / 10;
				fItvY = fItvY * 2;
				fItvZ = fItvZ / 5;

				if((m_pObjectCol->SetColCheck2(&SelfMax,&SelfMin,&TargetMax,&TargetMin) == true))
				{			
					if(fItvZ > fItvY && fItvZ > fItvX)
					{
						m_pInfo->m_vPos += vPushDir * 0.1f;
						m_bZcol = false;
					}
					else if(fItvX > fItvY && fItvX > fItvZ)
					{
						m_pInfo->m_vPos += vPushDir * 0.1f;
						m_bZcol = false;
					}

					else if(fItvY > fItvX && fItvY > fItvZ)
					{				
						m_vColObjMaxPos = GetMaxPos(((CShrinkingPlatform*)(*iter))->m_pPattern2->GetCubeVertex());
						m_vColObjMinPos = GetMinPos(((CShrinkingPlatform*)(*iter))->m_pPattern2->GetCubeVertex());
						m_pInfo->m_vPos.y = m_pInfo->m_vPos.y + 0.1f;
						m_bYcol = true;
						m_bDown = true;
						m_bUp = false;
					}
				}
			}
			else
				m_bOnShrink = false;
		}
	}
	list<ENGINE::CGameObject*>* pUpDownCube = m_pManagement->GetList(CStage::LAYER_GAMELOGIC,L"UpDownCube");
	if(pUpDownCube != NULL)
	{
		list<ENGINE::CGameObject*>::iterator iter = pUpDownCube->begin();
		list<ENGINE::CGameObject*>::iterator iter_end = pUpDownCube->end();

		for(; iter != iter_end; ++iter)
		{
			D3DXVECTOR3 vPushDir = m_pInfo->m_vPos - *(((CUpDownCube*)(*iter))->GetPos());
			D3DXVec3Normalize(&vPushDir, &vPushDir);
			TargetMax = GetMaxPos((*iter)->GetCubeVertex());
			TargetMin = GetMinPos((*iter)->GetCubeVertex());

			const D3DXVECTOR3* fCubePos = (*iter)->GetPos();

			float fItvX = fabs(m_pInfo->m_vPos.x - fCubePos->x) / ((CUpDownCube*)(*iter))->GetInfo()->m_vScale.x;
			float fItvY = fabs(m_pInfo->m_vPos.y - fCubePos->y) / ((CUpDownCube*)(*iter))->GetInfo()->m_vScale.y;
			float fItvZ = fabs(m_pInfo->m_vPos.z - fCubePos->z) / ((CUpDownCube*)(*iter))->GetInfo()->m_vScale.z;
			

			if((m_pObjectCol->SetColCheck2(&SelfMax,&SelfMin,&TargetMax,&TargetMin) == true))
			{	
				if(!bRedBull)
					fHp -= 0.2f;
				if(fItvZ > fItvY && fItvZ > fItvX)
				{
					m_pInfo->m_vPos += vPushDir * 0.1f;
					m_bZcol = false;
				}
				else if(fItvX > fItvY && fItvX > fItvZ)
				{
					m_pInfo->m_vPos += vPushDir * 0.1f;
					m_bZcol = false;
				}
				else if(fItvY > fItvX && fItvY > fItvZ)
				{
					m_bUp = true;
					m_bDown = false;
					m_bUpDowncol = true;
				}
			}	
			
		}
	}
	list<ENGINE::CGameObject*>* pSpinCutter = m_pManagement->GetList(CStage::LAYER_GAMELOGIC, L"SpinCutter");
	if(pSpinCutter != NULL)
	{
		list<ENGINE::CGameObject*>::iterator iter = pSpinCutter->begin();
		list<ENGINE::CGameObject*>::iterator iter_end = pSpinCutter->end();

		for(; iter != iter_end; ++iter)
		{
			if(((CSpinCutter*)(*iter))->m_pPattern1 != NULL)
			{
				D3DXVECTOR3 vPushDir = m_pInfo->m_vPos - *((CPattern1*)(((CSpinCutter*)(*iter))->m_pPattern1))->GetPos();
				D3DXVec3Normalize(&vPushDir, &vPushDir);
				TargetMax = GetMaxPos(((CPattern1*)(((CSpinCutter*)(*iter))->m_pPattern1))->GetCubeVertex());
				TargetMin = GetMinPos(((CPattern1*)(((CSpinCutter*)(*iter))->m_pPattern1))->GetCubeVertex());

				const D3DXVECTOR3* fCubePos = ((CPattern1*)(((CSpinCutter*)(*iter))->m_pPattern1))->GetPos();

				float fItvX = fabs(m_pInfo->m_vPos.x - fCubePos->x) / ((CPattern1*)(((CSpinCutter*)(*iter))->m_pPattern1))->GetInfo()->m_vScale.x;
				float fItvY = fabs(m_pInfo->m_vPos.y - fCubePos->y) / ((CPattern1*)(((CSpinCutter*)(*iter))->m_pPattern1))->GetInfo()->m_vScale.y;
				float fItvZ = fabs(m_pInfo->m_vPos.z - fCubePos->z) / ((CPattern1*)(((CSpinCutter*)(*iter))->m_pPattern1))->GetInfo()->m_vScale.z;

				if((m_pObjectCol->SetColCheck2(&SelfMax,&SelfMin,&TargetMax,&TargetMin) == true))
				{	
					if(!bRedBull)
						fHp -= 0.2f;
					if(fItvZ > fItvY && fItvZ > fItvX)
					{
						m_pInfo->m_vPos.x += 0.1f;
						m_bZcol = false;
					}
					else if(fItvX > fItvY && fItvX > fItvZ)
					{
						m_pInfo->m_vPos.x += 0.1f;
						m_bZcol = false;
					}

					else if(fItvY > fItvX && fItvY > fItvZ)
					{	
						m_pInfo->m_vPos.x += 0.1f;
						//m_pInfo->m_vPos += vPushDir * 0.1f;
						/*m_vColObjMaxPos = GetMaxPos(((CPattern1*)(((CSpinCutter*)(*iter))->m_pPattern1))->GetCubeVertex());
						m_vColObjMinPos = GetMinPos(((CPattern1*)(((CSpinCutter*)(*iter))->m_pPattern1))->GetCubeVertex());
						m_pInfo->m_vPos.y = m_pInfo->m_vPos.y + 0.1f;
						m_bYcol = true;*/
					}
				}	
			}
			if(((CSpinCutter*)(*iter))->m_pPattern2 != NULL)
			{
				D3DXVECTOR3 vPushDir = m_pInfo->m_vPos - *((CPattern1*)(((CSpinCutter*)(*iter))->m_pPattern2))->GetPos();
				D3DXVec3Normalize(&vPushDir, &vPushDir);
				TargetMax = GetMaxPos(((CPattern1*)(((CSpinCutter*)(*iter))->m_pPattern2))->GetCubeVertex());
				TargetMin = GetMinPos(((CPattern1*)(((CSpinCutter*)(*iter))->m_pPattern2))->GetCubeVertex());

				const D3DXVECTOR3* fCubePos = ((CPattern1*)(((CSpinCutter*)(*iter))->m_pPattern2))->GetPos();

				float fItvX = fabs(m_pInfo->m_vPos.x - fCubePos->x) / ((CPattern1*)(((CSpinCutter*)(*iter))->m_pPattern2))->GetInfo()->m_vScale.x;
				float fItvY = fabs(m_pInfo->m_vPos.y - fCubePos->y) / ((CPattern1*)(((CSpinCutter*)(*iter))->m_pPattern2))->GetInfo()->m_vScale.y;
				float fItvZ = fabs(m_pInfo->m_vPos.z - fCubePos->z) / ((CPattern1*)(((CSpinCutter*)(*iter))->m_pPattern2))->GetInfo()->m_vScale.z;

				if((m_pObjectCol->SetColCheck2(&SelfMax,&SelfMin,&TargetMax,&TargetMin) == true))
				{	
					if(!bRedBull)
						fHp -= 0.2f;
					if(fItvZ > fItvY && fItvZ > fItvX)
					{
						m_pInfo->m_vPos.x += 0.1f;
						m_bZcol = false;
					}
					else if(fItvX > fItvY && fItvX > fItvZ)
					{
						m_pInfo->m_vPos.x += 0.1f;
						m_bZcol = false;
					}

					else if(fItvY > fItvX && fItvY > fItvZ)
					{		
						m_pInfo->m_vPos.x += 0.1f;
						//m_pInfo->m_vPos += vPushDir * 0.1f;
						/*m_vColObjMaxPos = GetMaxPos(((CPattern1*)(((CSpinCutter*)(*iter))->m_pPattern2))->GetCubeVertex());
						m_vColObjMinPos = GetMinPos(((CPattern1*)(((CSpinCutter*)(*iter))->m_pPattern2))->GetCubeVertex());
						m_pInfo->m_vPos.y = m_pInfo->m_vPos.y + 0.1f;
						m_bYcol = true;*/
					}
				}
			}
			if(((CSpinCutter*)(*iter))->m_pPattern3 != NULL)
			{
				D3DXVECTOR3 vPushDir = m_pInfo->m_vPos - *((CPattern1*)(((CSpinCutter*)(*iter))->m_pPattern3))->GetPos();
				D3DXVec3Normalize(&vPushDir, &vPushDir);
				TargetMax = GetMaxPos(((CPattern1*)(((CSpinCutter*)(*iter))->m_pPattern3))->GetCubeVertex());
				TargetMin = GetMinPos(((CPattern1*)(((CSpinCutter*)(*iter))->m_pPattern3))->GetCubeVertex());

				const D3DXVECTOR3* fCubePos = ((CPattern1*)(((CSpinCutter*)(*iter))->m_pPattern3))->GetPos();

				float fItvX = fabs(m_pInfo->m_vPos.x - fCubePos->x) / ((CPattern1*)(((CSpinCutter*)(*iter))->m_pPattern3))->GetInfo()->m_vScale.x;
				float fItvY = fabs(m_pInfo->m_vPos.y - fCubePos->y) / ((CPattern1*)(((CSpinCutter*)(*iter))->m_pPattern3))->GetInfo()->m_vScale.y;
				float fItvZ = fabs(m_pInfo->m_vPos.z - fCubePos->z) / ((CPattern1*)(((CSpinCutter*)(*iter))->m_pPattern3))->GetInfo()->m_vScale.z;

				if((m_pObjectCol->SetColCheck2(&SelfMax,&SelfMin,&TargetMax,&TargetMin) == true))
				{	
					if(!bRedBull)
						fHp -= 0.2f;
					if(fItvZ > fItvY && fItvZ > fItvX)
					{
						m_pInfo->m_vPos.x += 0.1f;
						m_bZcol = false;
					}
					else if(fItvX > fItvY && fItvX > fItvZ)
					{
						m_pInfo->m_vPos.x += 0.1f;
						m_bZcol = false;
					}

					else if(fItvY > fItvX && fItvY > fItvZ)
					{		
						m_pInfo->m_vPos.x += 0.1f;
						//m_pInfo->m_vPos += vPushDir * 0.1f;
						/*m_vColObjMaxPos = GetMaxPos(((CPattern1*)(((CSpinCutter*)(*iter))->m_pPattern3))->GetCubeVertex());
						m_vColObjMinPos = GetMinPos(((CPattern1*)(((CSpinCutter*)(*iter))->m_pPattern3))->GetCubeVertex());
						m_pInfo->m_vPos.y = m_pInfo->m_vPos.y + 0.1f;
						m_bYcol = true;*/
					}
				}
			}
			if(((CSpinCutter*)(*iter))->m_pPattern4 != NULL)
			{
				D3DXVECTOR3 vPushDir = m_pInfo->m_vPos - *((CPattern1*)(((CSpinCutter*)(*iter))->m_pPattern4))->GetPos();
				D3DXVec3Normalize(&vPushDir, &vPushDir);
				TargetMax = GetMaxPos(((CPattern1*)(((CSpinCutter*)(*iter))->m_pPattern4))->GetCubeVertex());
				TargetMin = GetMinPos(((CPattern1*)(((CSpinCutter*)(*iter))->m_pPattern4))->GetCubeVertex());

				const D3DXVECTOR3* fCubePos = ((CPattern1*)(((CSpinCutter*)(*iter))->m_pPattern4))->GetPos();

				float fItvX = fabs(m_pInfo->m_vPos.x - fCubePos->x) / ((CPattern1*)(((CSpinCutter*)(*iter))->m_pPattern4))->GetInfo()->m_vScale.x;
				float fItvY = fabs(m_pInfo->m_vPos.y - fCubePos->y) / ((CPattern1*)(((CSpinCutter*)(*iter))->m_pPattern4))->GetInfo()->m_vScale.y;
				float fItvZ = fabs(m_pInfo->m_vPos.z - fCubePos->z) / ((CPattern1*)(((CSpinCutter*)(*iter))->m_pPattern4))->GetInfo()->m_vScale.z;

				if((m_pObjectCol->SetColCheck2(&SelfMax,&SelfMin,&TargetMax,&TargetMin) == true))
				{	
					if(!bRedBull)
						fHp -= 0.2f;
					if(fItvZ > fItvY && fItvZ > fItvX)
					{
						m_pInfo->m_vPos.x += 0.1f;
						m_bZcol = false;
					}
					else if(fItvX > fItvY && fItvX > fItvZ)
					{
						m_pInfo->m_vPos.x += 0.1f;
						m_bZcol = false;
					}

					else if(fItvY > fItvX && fItvY > fItvZ)
					{	
						m_pInfo->m_vPos.x += 0.1f;
						//m_pInfo->m_vPos += vPushDir * 0.1f;
						/*m_vColObjMaxPos = GetMaxPos(((CPattern1*)(((CSpinCutter*)(*iter))->m_pPattern4))->GetCubeVertex());
						m_vColObjMinPos = GetMinPos(((CPattern1*)(((CSpinCutter*)(*iter))->m_pPattern4))->GetCubeVertex());
						m_pInfo->m_vPos.y = m_pInfo->m_vPos.y + 0.1f;
						m_bYcol = true;*/
					}
				}
			}
			if(((CSpinCutter*)(*iter))->m_pPattern5 != NULL)
			{
				D3DXVECTOR3 vPushDir = m_pInfo->m_vPos - *((CPattern1*)(((CSpinCutter*)(*iter))->m_pPattern5))->GetPos();
				D3DXVec3Normalize(&vPushDir, &vPushDir);
				TargetMax = GetMaxPos(((CPattern1*)(((CSpinCutter*)(*iter))->m_pPattern5))->GetCubeVertex());
				TargetMin = GetMinPos(((CPattern1*)(((CSpinCutter*)(*iter))->m_pPattern5))->GetCubeVertex());

				const D3DXVECTOR3* fCubePos = ((CPattern1*)(((CSpinCutter*)(*iter))->m_pPattern5))->GetPos();

				float fItvX = fabs(m_pInfo->m_vPos.x - fCubePos->x) / ((CPattern1*)(((CSpinCutter*)(*iter))->m_pPattern5))->GetInfo()->m_vScale.x;
				float fItvY = fabs(m_pInfo->m_vPos.y - fCubePos->y) / ((CPattern1*)(((CSpinCutter*)(*iter))->m_pPattern5))->GetInfo()->m_vScale.y;
				float fItvZ = fabs(m_pInfo->m_vPos.z - fCubePos->z) / ((CPattern1*)(((CSpinCutter*)(*iter))->m_pPattern5))->GetInfo()->m_vScale.z;

				if((m_pObjectCol->SetColCheck2(&SelfMax,&SelfMin,&TargetMax,&TargetMin) == true))
				{	
					if(!bRedBull)
						fHp -= 0.2f;
					if(fItvZ > fItvY && fItvZ > fItvX)
					{
						m_pInfo->m_vPos.x += 0.1f;
						m_bZcol = false;
					}
					else if(fItvX > fItvY && fItvX > fItvZ)
					{
						m_pInfo->m_vPos.x += 0.1f;
						m_bZcol = false;
					}

					else if(fItvY > fItvX && fItvY > fItvZ)
					{		
						m_pInfo->m_vPos.x += 0.1f;
						//m_pInfo->m_vPos += vPushDir * 0.1f;
						/*m_vColObjMaxPos = GetMaxPos(((CPattern1*)(((CSpinCutter*)(*iter))->m_pPattern5))->GetCubeVertex());
						m_vColObjMinPos = GetMinPos(((CPattern1*)(((CSpinCutter*)(*iter))->m_pPattern5))->GetCubeVertex());
						m_pInfo->m_vPos.y = m_pInfo->m_vPos.y + 0.1f;
						m_bYcol = true;*/
					}
				}
			}
		}
	}
	list<ENGINE::CGameObject*>* pMonsterList = m_pManagement->GetList(CStage::LAYER_GAMELOGIC,L"Monster");
	if(pMonsterList != NULL)
	{
		list<ENGINE::CGameObject*>::iterator iter = pMonsterList->begin();
		list<ENGINE::CGameObject*>::iterator iter_end = pMonsterList->end();

		for(; iter != iter_end; ++iter)
		{
			D3DXVECTOR3 vPushDir = m_pInfo->m_vPos - *(((CMonster*)(*iter))->GetPos());
			D3DXVec3Normalize(&vPushDir, &vPushDir);
			const ENGINE::VTXCUBE* temp = (*iter)->GetCubeVertex();
			TargetMax = GetMaxPos((*iter)->GetCubeVertex());
			TargetMin = GetMinPos((*iter)->GetCubeVertex());

			const D3DXVECTOR3* fCubePos = (*iter)->GetPos();

			float fItvX = fabs(m_pInfo->m_vPos.x - fCubePos->x);//GetInfo 없음
			float fItvY = fabs(m_pInfo->m_vPos.y - fCubePos->y);
			float fItvZ = fabs(m_pInfo->m_vPos.z - fCubePos->z);
			

			if((m_pObjectCol->SetColCheck2(&SelfMax,&SelfMin,&TargetMax,&TargetMin) == true))
			{			
				if(!bRedBull)
					fHp -= 0.2f;
				if(fItvZ > fItvY && fItvZ > fItvX)
				{
					m_pInfo->m_vPos += vPushDir * 0.1f;
					m_bZcol = false;
				}
				else if(fItvX > fItvY && fItvX > fItvZ)
				{
					m_pInfo->m_vPos += vPushDir * 0.1f;
					m_bZcol = false;
				}

				else if(fItvY > fItvX && fItvY > fItvZ)
				{				
					m_vColObjMaxPos = GetMaxPos((*iter)->GetCubeVertex());
					m_vColObjMinPos = GetMinPos((*iter)->GetCubeVertex());
					m_pInfo->m_vPos.y = m_pInfo->m_vPos.y + 0.1f;
					m_bYcol = true;
				}
			}
		}
	}
	list<ENGINE::CGameObject*>* pSpinList = m_pManagement->GetList(CStage::LAYER_GAMELOGIC,L"SpinObstacle");
	if(pSpinList != NULL)
	{
		list<ENGINE::CGameObject*>::iterator iter = pSpinList->begin();
		list<ENGINE::CGameObject*>::iterator iter_end = pSpinList->end();

		for(; iter != iter_end; ++iter)
		{
			D3DXVECTOR3 vPushDir = m_pInfo->m_vPos - ((CSpinObstacle*)(*iter))->GetInfo()->m_vPos;
			D3DXVec3Normalize(&vPushDir, &vPushDir);
			const ENGINE::VTXCUBE* temp = (*iter)->GetCubeVertex();
			TargetMax = GetMaxPos((*iter)->GetCubeVertex());
			TargetMin = GetMinPos((*iter)->GetCubeVertex());

			const D3DXVECTOR3* fCubePos = (*iter)->GetPos();

			float fItvX = fabs(m_pInfo->m_vPos.x - fCubePos->x) / ((CSpinObstacle*)(*iter))->GetInfo()->m_vScale.x;
			float fItvY = fabs(m_pInfo->m_vPos.y - fCubePos->y) / ((CSpinObstacle*)(*iter))->GetInfo()->m_vScale.y;
			float fItvZ = fabs(m_pInfo->m_vPos.z - fCubePos->z) / ((CSpinObstacle*)(*iter))->GetInfo()->m_vScale.z;
			

			if((m_pObjectCol->SetColCheck2(&SelfMax,&SelfMin,&TargetMax,&TargetMin) == true))
			{		
				m_fSpeed = -5.f;
				if(fItvZ > fItvY && fItvZ > fItvX)
				{
					m_pInfo->m_vPos += vPushDir * 0.1f;
					m_bZcol = false;
				}
				else if(fItvX > fItvY && fItvX > fItvZ)
				{
					m_pInfo->m_vPos += vPushDir * 0.1f;
					m_bZcol = false;
				}

				else if(fItvY > fItvX && fItvY > fItvZ)
				{				
					m_vColObjMaxPos = GetMaxPos((*iter)->GetCubeVertex());
					m_vColObjMinPos = GetMinPos((*iter)->GetCubeVertex());
					m_pInfo->m_vPos.y = m_pInfo->m_vPos.y + 0.1f;
					m_bYcol = true;
				}
			}
			else
				m_fSpeed = 5.f;
			if(((CSpinObstacle*)(*iter))->m_pMonster1 != NULL)
			{
				const ENGINE::VTXCUBE* temp2 = ((CSpinObstacle*)(*iter))->m_pMonster1->GetCubeVertex();//(*iter)->GetCubeVertex();
				TargetMax = GetMaxPos(((CSpinObstacle*)(*iter))->m_pMonster1->GetCubeVertex());
				TargetMin = GetMinPos(((CSpinObstacle*)(*iter))->m_pMonster1->GetCubeVertex());

				const D3DXVECTOR3* fCubePos2 = ((CSpinObstacle*)(*iter))->m_pMonster1->GetPos();

				float fItvX2 = fabs(m_pInfo->m_vPos.x - fCubePos2->x) / ((CSpinblade*)((CSpinObstacle*)(*iter))->m_pMonster1)->GetInfo()->m_vScale.x;
				float fItvY2 = fabs(m_pInfo->m_vPos.y - fCubePos2->y) / ((CSpinblade*)((CSpinObstacle*)(*iter))->m_pMonster1)->GetInfo()->m_vScale.y;
				float fItvZ2 = fabs(m_pInfo->m_vPos.z - fCubePos2->z) / ((CSpinblade*)((CSpinObstacle*)(*iter))->m_pMonster1)->GetInfo()->m_vScale.z;
				if((m_pObjectCol->SetColCheck2(&SelfMax,&SelfMin,&TargetMax,&TargetMin) == true))
				{	
					if(!bRedBull)
						fHp -= 0.2f;
					m_fSpeed = -5.f;
					if(fItvZ2 > fItvY2 && fItvZ2 > fItvX2)
					{
						m_pInfo->m_vPos += vPushDir * 0.1f;
						m_bZcol = false;
					}
					else if(fItvX2 > fItvY2 && fItvX2 > fItvZ2)
					{
						m_pInfo->m_vPos += vPushDir * 0.1f;
						m_bZcol = false;
					}

					else if(fItvY2 > fItvX2 && fItvY2 > fItvZ2)
					{				
						m_vColObjMaxPos = GetMaxPos((*iter)->GetCubeVertex());
						m_vColObjMinPos = GetMinPos((*iter)->GetCubeVertex());
						m_pInfo->m_vPos.y = m_pInfo->m_vPos.y + 0.1f;
						m_bYcol = true;
					}
				}
				else
					m_fSpeed = 5.f;
			}
			if(((CSpinObstacle*)(*iter))->m_pMonster2 != NULL)
			{
				const ENGINE::VTXCUBE* temp2 = ((CSpinObstacle*)(*iter))->m_pMonster2->GetCubeVertex();//(*iter)->GetCubeVertex();
				TargetMax = GetMaxPos(((CSpinObstacle*)(*iter))->m_pMonster2->GetCubeVertex());
				TargetMin = GetMinPos(((CSpinObstacle*)(*iter))->m_pMonster2->GetCubeVertex());

				const D3DXVECTOR3* fCubePos2 = ((CSpinObstacle*)(*iter))->m_pMonster2->GetPos();

				float fItvX2 = fabs(m_pInfo->m_vPos.x - fCubePos2->x) / ((CSpinblade*)((CSpinObstacle*)(*iter))->m_pMonster2)->GetInfo()->m_vScale.x;
				float fItvY2 = fabs(m_pInfo->m_vPos.y - fCubePos2->y) / ((CSpinblade*)((CSpinObstacle*)(*iter))->m_pMonster2)->GetInfo()->m_vScale.y;
				float fItvZ2 = fabs(m_pInfo->m_vPos.z - fCubePos2->z) / ((CSpinblade*)((CSpinObstacle*)(*iter))->m_pMonster2)->GetInfo()->m_vScale.z;
				if((m_pObjectCol->SetColCheck2(&SelfMax,&SelfMin,&TargetMax,&TargetMin) == true))
				{	
					if(!bRedBull)
						fHp -= 0.2f;
					m_fSpeed = -5.f;
					if(fItvZ2 > fItvY2 && fItvZ2 > fItvX2)
					{
						m_pInfo->m_vPos += vPushDir * 0.1f;
						m_bZcol = false;
					}
					else if(fItvX2 > fItvY2 && fItvX2 > fItvZ2)
					{
						m_pInfo->m_vPos += vPushDir * 0.1f;
						m_bZcol = false;
					}

					else if(fItvY2 > fItvX2 && fItvY2 > fItvZ2)
					{				
						m_vColObjMaxPos = GetMaxPos((*iter)->GetCubeVertex());
						m_vColObjMinPos = GetMinPos((*iter)->GetCubeVertex());
						m_pInfo->m_vPos.y = m_pInfo->m_vPos.y + 0.1f;
						m_bYcol = true;
					}
				}
				else
					m_fSpeed = 5.f;
			}
		}
	}
	list<ENGINE::CGameObject*>* pSpinSnowList = m_pManagement->GetList(CStage::LAYER_GAMELOGIC,L"SpinSnow");
	if(pSpinSnowList != NULL)
	{
		list<ENGINE::CGameObject*>::iterator iter = pSpinSnowList->begin();
		list<ENGINE::CGameObject*>::iterator iter_end = pSpinSnowList->end();

		for(; iter != iter_end; ++iter)
		{
			D3DXVECTOR3 vPushDir = m_pInfo->m_vPos - *(((CSpinSnow*)(*iter))->m_pSnowMan[2]->GetPos());
			D3DXVec3Normalize(&vPushDir, &vPushDir);
			for(int i = 0; i < 5; ++i)
			{
				if(((CSpinSnow*)(*iter))->m_pSnowMan[i] != NULL)
				{
					/*D3DXVECTOR3 vPushDir = m_pInfo->m_vPos - *(((CSpinSnow*)(*iter))->m_pSnowMan[i]->GetPos());
					D3DXVec3Normalize(&vPushDir, &vPushDir);*/
					const ENGINE::VTXCUBE* temp2 = ((CSpinSnow*)(*iter))->m_pSnowMan[i]->GetCubeVertex();
					TargetMax = GetMaxPos(((CSpinSnow*)(*iter))->m_pSnowMan[i]->GetCubeVertex());
					TargetMin = GetMinPos(((CSpinSnow*)(*iter))->m_pSnowMan[i]->GetCubeVertex());

					const D3DXVECTOR3* fCubePos2 = ((CSpinSnow*)(*iter))->m_pSnowMan[i]->GetPos();

					float fItvX2 = fabs(m_pInfo->m_vPos.x - fCubePos2->x) / ((CSpinSnowMan*)((CSpinSnow*)(*iter))->m_pSnowMan[i])->GetInfo()->m_vScale.x;
					float fItvY2 = fabs(m_pInfo->m_vPos.y - fCubePos2->y) / ((CSpinSnowMan*)((CSpinSnow*)(*iter))->m_pSnowMan[i])->GetInfo()->m_vScale.y;
					float fItvZ2 = fabs(m_pInfo->m_vPos.z - fCubePos2->z) / ((CSpinSnowMan*)((CSpinSnow*)(*iter))->m_pSnowMan[i])->GetInfo()->m_vScale.z;
					if((m_pObjectCol->SetColCheck2(&SelfMax,&SelfMin,&TargetMax,&TargetMin) == true))
					{	
						if(!bRedBull)
							fHp -= 0.2f;
						m_fSpeed = -5.f;
						if(fItvZ2 > fItvY2 && fItvZ2 > fItvX2)
						{
							m_pInfo->m_vPos += vPushDir * 0.1f;
							m_bZcol = false;
						}
						else if(fItvX2 > fItvY2 && fItvX2 > fItvZ2)
						{
							m_pInfo->m_vPos += vPushDir * 0.1f;
							m_bZcol = false;
						}

						else if(fItvY2 > fItvX2 && fItvY2 > fItvZ2)
						{				
							m_vColObjMaxPos = GetMaxPos((*iter)->GetCubeVertex());
							m_vColObjMinPos = GetMinPos((*iter)->GetCubeVertex());
							m_pInfo->m_vPos.y = m_pInfo->m_vPos.y + 0.1f;
							m_bYcol = true;
						}
					}
					else
						m_fSpeed = 5.f;
				}
				if(((CSpinSnow*)(*iter))->m_pSnowMan_Body[i] != NULL)
				{
					/*D3DXVECTOR3 vPushDir = m_pInfo->m_vPos - *(((CSpinSnow*)(*iter))->m_pSnowMan_Body[i]->GetPos());
					D3DXVec3Normalize(&vPushDir, &vPushDir);*/
					const ENGINE::VTXCUBE* temp2 = ((CSpinSnow*)(*iter))->m_pSnowMan_Body[i]->GetCubeVertex();
					TargetMax = GetMaxPos(((CSpinSnow*)(*iter))->m_pSnowMan_Body[i]->GetCubeVertex());
					TargetMin = GetMinPos(((CSpinSnow*)(*iter))->m_pSnowMan_Body[i]->GetCubeVertex());

					const D3DXVECTOR3* fCubePos2 = ((CSpinSnow*)(*iter))->m_pSnowMan_Body[i]->GetPos();

					float fItvX2 = fabs(m_pInfo->m_vPos.x - fCubePos2->x) / ((CSpinSnowMan*)((CSpinSnow*)(*iter))->m_pSnowMan_Body[i])->GetInfo()->m_vScale.x;
					float fItvY2 = fabs(m_pInfo->m_vPos.y - fCubePos2->y) / ((CSpinSnowMan*)((CSpinSnow*)(*iter))->m_pSnowMan_Body[i])->GetInfo()->m_vScale.y;
					float fItvZ2 = fabs(m_pInfo->m_vPos.z - fCubePos2->z) / ((CSpinSnowMan*)((CSpinSnow*)(*iter))->m_pSnowMan_Body[i])->GetInfo()->m_vScale.z;
					if((m_pObjectCol->SetColCheck2(&SelfMax,&SelfMin,&TargetMax,&TargetMin) == true))
					{	
						if(!bRedBull)
							fHp -= 0.2f;
						m_fSpeed = -5.f;
						if(fItvZ2 > fItvY2 && fItvZ2 > fItvX2)
						{
							m_pInfo->m_vPos += vPushDir * 0.1f;
							m_bZcol = false;
						}
						else if(fItvX2 > fItvY2 && fItvX2 > fItvZ2)
						{
							m_pInfo->m_vPos += vPushDir * 0.1f;
							m_bZcol = false;
						}

						else if(fItvY2 > fItvX2 && fItvY2 > fItvZ2)
						{				
							m_vColObjMaxPos = GetMaxPos((*iter)->GetCubeVertex());
							m_vColObjMinPos = GetMinPos((*iter)->GetCubeVertex());
							m_pInfo->m_vPos.y = m_pInfo->m_vPos.y + 0.1f;
							m_bYcol = true;
						}
					}
					else
						m_fSpeed = 5.f;
				}
			}
		}
	}

	list<ENGINE::CGameObject*>* pDoorList = m_pManagement->GetList(CStage::LAYER_GAMELOGIC, L"Door");

	if(pDoorList != NULL)
	{
		list<ENGINE::CGameObject*>::iterator iter = pDoorList->begin();
		list<ENGINE::CGameObject*>::iterator iter_end = pDoorList->end();

		for(; iter != iter_end;)
		{
			D3DXVECTOR3 vPushDir = m_pInfo->m_vPos - *(((CDoor*)(*iter))->GetPos());
			D3DXVec3Normalize(&vPushDir, &vPushDir);
			TargetMax = GetMaxPos((*iter)->GetCubeVertex());
			TargetMin = GetMinPos((*iter)->GetCubeVertex());

			const D3DXVECTOR3* fCubePos = (*iter)->GetPos();

			float fItvX = fabs(m_pInfo->m_vPos.x - fCubePos->x) / ((CDoor*)(*iter))->GetInfo()->m_vScale.x;
			float fItvY = fabs(m_pInfo->m_vPos.y - fCubePos->y) / ((CDoor*)(*iter))->GetInfo()->m_vScale.y;
			float fItvZ = fabs(m_pInfo->m_vPos.z - fCubePos->z) / ((CDoor*)(*iter))->GetInfo()->m_vScale.z;


			if((m_pObjectCol->SetColCheck2(&SelfMax,&SelfMin,&TargetMax,&TargetMin) == true))
			{	
				if(eStageState == ST_STAGE)
					bLevel = true;
				if(eStageState == ST_STAGE1)
					bLevel1 = true;
				if(eStageState == ST_STAGE2)
					bLevel2 = true;
				if(eStageState == ST_SHOT)
					bLevelShot = true;
				return;
			}
			else
				++iter;
		}
	}
	list<ENGINE::CGameObject*>* pRespawnList = m_pManagement->GetList(CStage::LAYER_GAMELOGIC, L"Respawn");

	if(pRespawnList != NULL)
	{
		list<ENGINE::CGameObject*>::iterator iter = pRespawnList->begin();
		list<ENGINE::CGameObject*>::iterator iter_end = pRespawnList->end();
		int i = 0;

		for(; iter != iter_end; ++ iter)
		{
			D3DXVECTOR3 vPushDir = m_pInfo->m_vPos - *(((CRespawn*)(*iter))->GetPos());
			D3DXVec3Normalize(&vPushDir, &vPushDir);
			TargetMax = GetMaxPos((*iter)->GetCubeVertex());
			TargetMin = GetMinPos((*iter)->GetCubeVertex());

			const D3DXVECTOR3* fCubePos = (*iter)->GetPos();

			float fItvX = fabs(m_pInfo->m_vPos.x - fCubePos->x) / ((CRespawn*)(*iter))->GetInfo()->m_vScale.x;
			float fItvY = fabs(m_pInfo->m_vPos.y - fCubePos->y) / ((CRespawn*)(*iter))->GetInfo()->m_vScale.y;
			float fItvZ = fabs(m_pInfo->m_vPos.z - fCubePos->z) / ((CRespawn*)(*iter))->GetInfo()->m_vScale.z;

			++i;


			if((m_pObjectCol->SetColCheck2(&SelfMax,&SelfMin,&TargetMax,&TargetMin) == true))
			{	
				//cout << i << "'s Respawn Check!" << endl;
				m_RespawnPos = ((CRespawn*)(*iter))->GetInfo()->m_vPos;

				//cout << "Respawn pos is : " << m_RespawnPos.x << ", " << m_RespawnPos.y << ", " << m_RespawnPos.z << endl;
			}
		}
	}

	list<ENGINE::CGameObject*>* pCentryGunList = m_pManagement->GetList(CStage::LAYER_GAMELOGIC, L"CentryGun");
	if(pCentryGunList != NULL)
	{
		list<ENGINE::CGameObject*>::iterator iter = pCentryGunList->begin();
		list<ENGINE::CGameObject*>::iterator iter_end = pCentryGunList->end();

		for(; iter != iter_end; ++iter)
		{
			D3DXVECTOR3 vPushDir = m_pInfo->m_vPos - *(((CCentryGun*)(*iter))->GetPos());
			D3DXVec3Normalize(&vPushDir, &vPushDir);
			TargetMax = GetMaxPos((*iter)->GetCubeVertex());
			TargetMin = GetMinPos((*iter)->GetCubeVertex());

			const D3DXVECTOR3* fCubePos = (*iter)->GetPos();

			float fItvX = fabs(m_pInfo->m_vPos.x - fCubePos->x) / ((CCentryGun*)(*iter))->GetInfo()->m_vScale.x;
			float fItvY = fabs(m_pInfo->m_vPos.y - fCubePos->y) / ((CCentryGun*)(*iter))->GetInfo()->m_vScale.y;
			float fItvZ = fabs(m_pInfo->m_vPos.z - fCubePos->z) / ((CCentryGun*)(*iter))->GetInfo()->m_vScale.z;
			

			if((m_pObjectCol->SetColCheck2(&SelfMax,&SelfMin,&TargetMax,&TargetMin) == true))
			{			
				if(fItvZ > fItvY && fItvZ > fItvX)
				{
					m_pInfo->m_vPos += vPushDir * 0.4f;
					m_bZcol = false;
				}
				else if(fItvX > fItvY && fItvX > fItvZ)
				{
					m_pInfo->m_vPos += vPushDir * 0.4f;
					m_bZcol = false;
				}

				else if(fItvY > fItvX && fItvY > fItvZ)
				{				
					/*m_vColObjMaxPos = GetMaxPos((*iter)->GetCubeVertex());
					m_vColObjMinPos = GetMinPos((*iter)->GetCubeVertex());
					m_pInfo->m_vPos.y = m_pInfo->m_vPos.y + 0.1f;
					m_bYcol = true;*/
				}
			}

			D3DXVECTOR3 TargetMax2, TargetMin2;

			list<CBullet*>::iterator iter2 = ((CCentryGun*)(*iter))->GetBulletList()->begin();
			list<CBullet*>::iterator iter2_end = ((CCentryGun*)(*iter))->GetBulletList()->end();
			for(; iter2 != iter2_end; )
			{
				TargetMax2 = GetMaxPos((*iter2)->GetCubeVertex());
				TargetMin2 = GetMinPos((*iter2)->GetCubeVertex());
				if((m_pObjectCol->SetColCheck2(&SelfMax,&SelfMin,&TargetMax2,&TargetMin2) == true))
				{
					fHp -= 5.f;
					if(fHp <= 0.f)
						fHp = 0.f;
					ENGINE::Safe_Delete((*iter2));
					iter2 = ((CCentryGun*)(*iter))->GetBulletList()->erase(iter2);
				}
				else
					++iter2;
			}
		}
	}
	if(fHp <= 0)
	{
		fHp = 100.f;
		m_pInfo->m_vPos = m_RespawnPos;
	}
}
D3DXVECTOR3 CPlayer::GetMaxPos(const ENGINE::VTXCUBE* pCube)
{
	D3DXVECTOR3	vMax;

	float x = pCube[0].vPos.x;
	float y = pCube[0].vPos.y;
	float z = pCube[0].vPos.z;

	for(int i = 0; i != 8; ++i)
	{
		if(pCube[i].vPos.x > x)
			x = pCube[i].vPos.x;

		if(pCube[i].vPos.y > y)
			y = pCube[i].vPos.y;

		if(pCube[i].vPos.z > z)
			z = pCube[i].vPos.z;
	}
	vMax.x = x;
	vMax.y = y;
	vMax.z = z;

	return vMax;
}

D3DXVECTOR3 CPlayer::GetMinPos(const ENGINE::VTXCUBE* pCube)
{

	D3DXVECTOR3	vMin;

	float x = pCube[0].vPos.x;
	float y = pCube[0].vPos.y;
	float z = pCube[0].vPos.z;

	for(int i = 0; i != 8; ++i)
	{
		if(pCube[i].vPos.x < x)
			x = pCube[i].vPos.x;

		if(pCube[i].vPos.y < y)
			y = pCube[i].vPos.y;

		if(pCube[i].vPos.z < z)
			z = pCube[i].vPos.z;
	}

	vMin.x = x;
	vMin.y = y;
	vMin.z = z;
	

	return vMin;
}
void CPlayer::FireBullet(void)
{
	
	if(bStage1)
	{
		list<ENGINE::CGameObject*>* pMonsterList = m_pManagement->GetList(CStage::LAYER_GAMELOGIC, L"CentryGun");
		
		if(pMonsterList != NULL)
		{
			list<ENGINE::CGameObject*>::iterator iter2 = pMonsterList->begin();
			list<ENGINE::CGameObject*>::iterator iter2_end = pMonsterList->end();
		
			for(; iter2 != iter2_end; )
			{
				if(m_pMouseCol->PickObject(&m_vDestPos, ((CCentryGun*)(*iter2))->GetMonsterTex()->GetpVertex(), &((CCentryGun*)(*iter2))->GetMonsterTex()->GetpInfo()->m_matWorld))
				{
					if(*(((CCentryGun*)(*iter2))->GetCentryGunHp()) <= 0)
					{
						((CCentryGun*)(*iter2))->GetMonsterTex()->SetDead(true);
						return;
					}
					else
					{
						*(((CCentryGun*)(*iter2))->GetCentryGunHp()) -= 1;
						++iter2;
						return;
					}
				}
				else
					++iter2;
			}
		}
	}
	

	if(bStage2Start)
	{
		// c++책
		if(bStage2Start)
		{
			list<ENGINE::CGameObject*>* pTargetList = m_pManagement->GetList(CShot::LAYER_TARGET,L"Book");
			list<ENGINE::CGameObject*>::iterator iter3 = pTargetList->begin();
			list<ENGINE::CGameObject*>::iterator iter3_end = pTargetList->end();

			for(; iter3 != iter3_end; )
			{
				if(m_pMouseCol->PickObject(&m_vDestPos, ((CTarget*)(*iter3))->GetpVertex(), &((CTarget*)(*iter3))->GetpInfo()->m_matWorld))
				{
					((CTarget*)(*iter3))->SetLife(false);
					iScore += 20;
					return;
				}
				else
					++iter3;
			}
		}

		//용책
		if(bStage2Start2)
		{
			list<ENGINE::CGameObject*>* pTargetList = m_pManagement->GetList(CShot::LAYER_TARGET2,L"Book");
			list<ENGINE::CGameObject*>::iterator iter3 = pTargetList->begin();
			list<ENGINE::CGameObject*>::iterator iter3_end = pTargetList->end();

			for(; iter3 != iter3_end; )
			{
				if(m_pMouseCol->PickObject(&m_vDestPos, ((CTarget*)(*iter3))->GetpVertex(), &((CTarget*)(*iter3))->GetpInfo()->m_matWorld))
				{
					((CTarget*)(*iter3))->SetLife(false);
					iScore += 40;
					return;
				}
				else
					++iter3;
			}
		}

		// 해골책
		if(bStage2Start3)
		{
			list<ENGINE::CGameObject*>* pTargetList = m_pManagement->GetList(CShot::LAYER_TARGET3,L"Book");
			list<ENGINE::CGameObject*>::iterator iter3 = pTargetList->begin();
			list<ENGINE::CGameObject*>::iterator iter3_end = pTargetList->end();

			for(; iter3 != iter3_end; )
			{
				if(m_pMouseCol->PickObject(&m_vDestPos, ((CTarget*)(*iter3))->GetpVertex(), &((CTarget*)(*iter3))->GetpInfo()->m_matWorld))
				{
					((CTarget*)(*iter3))->SetLife(false);
					iScore += 60;
					return;
				}
				else
					++iter3;
			}
		}
	
	}
	
}

ENGINE::CTransform* CPlayer::GetInfo(void)
{
	return m_pInfo;
}

void CPlayer::BookCheck(void)
{
	if(bStage2Start)
	{
		list<ENGINE::CGameObject*>* pTargetList2 = m_pManagement->GetList(CShot::LAYER_TARGET,L"Book");
		if(pTargetList2 != NULL)
		{
			list<ENGINE::CGameObject*>::iterator iter4 = pTargetList2->begin();
			list<ENGINE::CGameObject*>::iterator iter4_end = pTargetList2->end();

			if(iter4 == iter4_end)
				return;

			for(; iter4 != iter4_end;)
			{
				if(((CTarget*)(*iter4))->GetLife() == true)
				{
					m_pInfoSubject->UnSubscribe(((CTarget*)(*iter4))->GetCamObserver());
					ENGINE::Safe_Delete((*iter4));
					iter4 = pTargetList2->erase(iter4);

				}
				else
					++iter4;
			}
		}
	}
	if(bStage2Start2)
	{
		list<ENGINE::CGameObject*>* pTargetList2 = m_pManagement->GetList(CShot::LAYER_TARGET2,L"Book");
		if(pTargetList2 != NULL)
		{
			list<ENGINE::CGameObject*>::iterator iter4 = pTargetList2->begin();
			list<ENGINE::CGameObject*>::iterator iter4_end = pTargetList2->end();

			if(iter4 == iter4_end)
				return;

			for(; iter4 != iter4_end;)
			{
				if(((CTarget*)(*iter4))->GetLife() == true)
				{
					m_pInfoSubject->UnSubscribe(((CTarget*)(*iter4))->GetCamObserver());
					ENGINE::Safe_Delete((*iter4));
					iter4 = pTargetList2->erase(iter4);

				}
				else
					++iter4;
			}
		}
	}

	if(bStage2Start3)
	{
		list<ENGINE::CGameObject*>* pTargetList2 = m_pManagement->GetList(CShot::LAYER_TARGET3,L"Book");
		if(pTargetList2 != NULL)
		{
			list<ENGINE::CGameObject*>::iterator iter4 = pTargetList2->begin();
			list<ENGINE::CGameObject*>::iterator iter4_end = pTargetList2->end();

			if(iter4 == iter4_end)
				return;

			for(; iter4 != iter4_end;)
			{
				if(((CTarget*)(*iter4))->GetLife() == true)
				{
					m_pInfoSubject->UnSubscribe(((CTarget*)(*iter4))->GetCamObserver());
					ENGINE::Safe_Delete((*iter4));
					iter4 = pTargetList2->erase(iter4);

				}
				else
					++iter4;
			}
		}	
	}
}

void CPlayer::SetStageState(STAGESTATE eState)
{
	eStageState = eState;
}

void CPlayer::CentryCheck(void)
{
	list<ENGINE::CGameObject*>* pMonsterList = m_pManagement->GetList(CStage::LAYER_GAMELOGIC, L"CentryGun");
	if(pMonsterList != NULL)
	{
		list<ENGINE::CGameObject*>::iterator iter2 = pMonsterList->begin();
		list<ENGINE::CGameObject*>::iterator iter2_end = pMonsterList->end();

		if(iter2 == iter2_end)
			return;

		for(; iter2 != iter2_end; )
		{
			if(((CCentryGun*)(*iter2))->GetMonsterTex()->GetDelete() == true)
			{
				ENGINE::Safe_Delete((*iter2));
				iter2 = pMonsterList->erase(iter2);
			}
			else
				++iter2;
		}
	}
}
