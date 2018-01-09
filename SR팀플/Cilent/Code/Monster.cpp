#include "stdafx.h"
#include "Monster.h"

#include "Include.h"
#include "Texture.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Pipeline.h"
#include "CameraObserver.h"
#include "CollisionMgr.h"
#include "TerrainCol.h"
#include "Stage.h"
#include "ObjectCol.h"
#include "KeyMgr.h"
#include "MonsterTex.h"

CMonster::CMonster(LPDIRECT3DDEVICE9 pDevice)
: CGameObject(pDevice)
, m_pResourceMgr(ENGINE::Get_ResourcesMgr())
, m_pManagement(ENGINE::Get_Management())
, m_pTimeMgr(ENGINE::Get_Time())
, m_pInfoSubject(ENGINE::Get_InfoSubject())
, m_pCameraObserver(NULL)
, m_pVertex(NULL)
, m_pConvertVertex(NULL)
, m_dwVtxCnt(0)
, m_pCollisionMgr(CCollisionMgr::GetInstance())
, m_pTerrainCol(NULL)
, m_pTerrainVertex(NULL)
, m_bMove(false)
, m_pMonsterTex(NULL)
{

}

CMonster::~CMonster()
{
	Release();
}

HRESULT CMonster::Initialize(void)
{
	m_pCameraObserver = CCameraObserver::Create();
	m_pInfoSubject->Subscribe(m_pCameraObserver);


	m_fSpeed = 10.f;
	m_fDistance = 10.f;
	m_fCameraAngle = D3DXToRadian(45.f);

	m_dwVtxCnt = 8;


	m_pVertex			= new ENGINE::VTXCUBE[m_dwVtxCnt];
	m_pConvertVertex	= new ENGINE::VTXCUBE[m_dwVtxCnt];
	m_pColVtx			 = new ENGINE::VTXCUBE[m_dwVtxCnt];
	m_pResourceMgr->GetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer_CubeTex", m_pVertex);

	FAILED_CHECK(AddComponent());

	m_pMonsterTex = CMonsterTex::Create(m_pDevice);
	NULL_CHECK_RETURN(m_pMonsterTex, E_FAIL);

	return S_OK;
}

void CMonster::Update(void)
{
	//KeyInput();
	
	m_pTerrainVertex = m_pManagement->GetTerrainVertex(CStage::LAYER_GAMELOGIC, L"Terrain");

	m_pTerrainCol->SetColInfo(&m_pInfo->m_vPos, m_pTerrainVertex);

	D3DXVec3TransformNormal(&m_pInfo->m_vDir, &g_vLook, &m_pInfo->m_matWorld);
	D3DXVec3Normalize(&m_pInfo->m_vDir, &m_pInfo->m_vDir);

	//if(m_bMove)
	//	Move();

	ENGINE::CGameObject::Update();

	//D3DXVECTOR3 SelfMax, SelfMin;
	//D3DXVECTOR3 TargetMax, TargetMin;

	//SelfMax = GetMaxPos(m_pConvertVertex);
	//SelfMin = GetMinPos(m_pConvertVertex);

	//TargetMax = GetMaxPos(m_pManagement->GetCubeVertex(CStage::LAYER_GAMELOGIC,L"CubeObj"));
	//TargetMin = GetMinPos(m_pManagement->GetCubeVertex(CStage::LAYER_GAMELOGIC,L"CubeObj"));

	//if((m_pObjectCol->SetColCheck(m_pConvertVertex,m_pManagement->GetCubeVertex(CStage::LAYER_GAMELOGIC,L"CubeObj")) == true))
	//{
	//	int a = 10;
	//}

	/*if((m_pObjectCol->SetColCheck2(&SelfMax,&SelfMin,&TargetMax,&TargetMin) == true))
	{
		if (SelfMax.z > TargetMin.z)
		{
			m_pInfo->m_vPos.z =  m_pInfo->m_vPos.z - 0.1f;
		}

	}*/
	

	SetTransForm();

	if(m_pMonsterTex != NULL)
	{
		m_pMonsterTex->SetMonsterTex(&m_pInfo->m_vPos);
		m_pMonsterTex->Update();
	}
}

void CMonster::Render(void)
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

	m_pTexture->Render(0);

	m_pBuffer->Render();

	if(m_pMonsterTex != NULL)
		m_pMonsterTex->Render();

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	
}

void CMonster::Release(void)
{
	m_pInfoSubject->UnSubscribe(m_pCameraObserver);
	ENGINE::Safe_Delete(m_pCameraObserver);

	if(m_pVertex)
		ENGINE::Safe_Delete_Array(m_pVertex);

	if(m_pConvertVertex)
		ENGINE::Safe_Delete_Array(m_pConvertVertex);

	if(m_pMonsterTex)
		ENGINE::Safe_Delete(m_pMonsterTex);
}



HRESULT CMonster::AddComponent(void)
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

	// Collision Terrain
	pComponent = m_pCollisionMgr->CloneColObject(CCollisionMgr::COL_TERRAIN);
	m_pTerrainCol = dynamic_cast<CTerrainCol*>(pComponent);
	NULL_CHECK_RETURN(m_pTerrainCol, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"TerrainCol", pComponent));

	// Texture
	pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_STATIC, L"Monster0");
	m_pTexture = dynamic_cast<ENGINE::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));


	return S_OK;
}

CMonster* CMonster::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMonster* pPlayer = new CMonster(pDevice);

	if(FAILED(pPlayer->Initialize()))
		ENGINE::Safe_Delete(pPlayer);

	return pPlayer;
}


void CMonster::SetTransForm(void)
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

void CMonster::SetPos(const D3DXVECTOR3& vPos)
{
	m_pInfo->m_vPos = vPos;
}

void CMonster::KeyInput(void)
{
	float fTime = m_pTimeMgr->GetTime();

	D3DXVECTOR3 vSide = D3DXVECTOR3(1.f, 0.f, 0.f);
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
		m_pInfo->m_vPos += vSide * fTime * m_fSpeed;
	}

	if(GetAsyncKeyState('A'))
	{
		m_pInfo->m_vPos -= vSide * fTime * m_fSpeed;
	}
	/*if(ptMouse.x <= WINCX - 800)
	{
		m_pInfo->m_fAngle[ENGINE::ANGLE_Y] += D3DXToRadian(40.f) * fTime;
	}

	if(ptMouse.x >= WINCX)
	{
		m_pInfo->m_fAngle[ENGINE::ANGLE_Y] -= D3DXToRadian(40.f) * fTime;
	}


	if(ptMouse.y <= WINCY - 600)
	{
		m_pInfo->m_fAngle[ENGINE::ANGLE_X] += D3DXToRadian(40.f) * fTime;
	}
	if(ptMouse.y >= WINCY)
	{
		m_pInfo->m_fAngle[ENGINE::ANGLE_X] -= D3DXToRadian(40.f) * fTime;
	}*/
}

void CMonster::Move(void)
{
	D3DXVECTOR3		vDir;

	vDir = m_vDestPos - m_pInfo->m_vPos;

	float fDistance = D3DXVec3Length(&vDir);
	D3DXVec3Normalize(&vDir, &vDir);

	m_pInfo->m_vPos += vDir * m_fSpeed * m_pTimeMgr->GetTime();

	if(fDistance < 1.f)
		m_bMove = false;
}

D3DXVECTOR3 CMonster::GetMaxPos(const ENGINE::VTXCUBE* pCube)
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

D3DXVECTOR3 CMonster::GetMinPos(const ENGINE::VTXCUBE* pCube)
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
const	ENGINE::VTXCUBE* CMonster::GetCubeVertex()
{
	return m_pColVtx;
}
D3DXVECTOR3* CMonster::GetPos(void)
{
	return &m_pInfo->m_vPos;
}