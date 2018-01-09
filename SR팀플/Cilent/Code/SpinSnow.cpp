#include "stdafx.h"
#include "SpinSnow.h"

#include "Include.h"
#include "Texture.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Pipeline.h"
#include "CameraObserver.h"
#include "CollisionMgr.h"
#include "TerrainCol.h"
#include "Stage.h"
#include "Spinblade.h"
#include "SpinSnowMan.h"

CSpinSnow::CSpinSnow(LPDIRECT3DDEVICE9 pDevice)
: CObstacle(pDevice)
, m_pResourceMgr(ENGINE::Get_ResourcesMgr())
, m_pManagement(ENGINE::Get_Management())
, m_pTimeMgr(ENGINE::Get_Time())
, m_pInfoSubject(ENGINE::Get_InfoSubject())
, m_pCollisionMgr(CCollisionMgr::GetInstance())
, m_pTerrainCol(NULL)
, m_pTerrainVertex(NULL)
{
	for(int i = 0; i < 5; ++i)
	{
		m_pSnowMan[i] = NULL;
		m_pSnowMan_Body[i] = NULL;
	}

}

CSpinSnow::~CSpinSnow()
{
	Release();
}


HRESULT CSpinSnow::Initialize(void)
{
	SetTransForm();
	m_pCameraObserver = CCameraObserver::Create();
	m_pInfoSubject->Subscribe(m_pCameraObserver);

	m_fDistance = 10.f;
	
	m_dwVtxCnt = 8;

	m_pVertex 			= new ENGINE::VTXCUBE[m_dwVtxCnt];
	m_pConvertVertex	= new ENGINE::VTXCUBE[m_dwVtxCnt];
	m_pColVtx			= new ENGINE::VTXCUBE[m_dwVtxCnt];

	m_pResourceMgr->GetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer_CubeTex", m_pVertex);

	FAILED_CHECK(AddComponent());

	m_pInfo->m_vScale = D3DXVECTOR3(13.f, 0.2f, 0.5f);

	SnowManCreate();

	return S_OK;
}

void CSpinSnow::Update(void)
{

	m_pTerrainVertex = m_pManagement->GetTerrainVertex(CStage::LAYER_GAMELOGIC, L"Terrain");

	m_pTerrainCol->SetColInfo(&m_pInfo->m_vPos, m_pTerrainVertex);

	D3DXVec3TransformNormal(&m_pInfo->m_vDir, &g_vLook, &m_pInfo->m_matWorld);
	D3DXVec3Normalize(&m_pInfo->m_vDir, &m_pInfo->m_vDir);

	m_pInfo->m_fAngle[ENGINE::ANGLE_Y] -= D3DXToRadian(100.f) * ENGINE::CTimeMgr::GetInstance()->GetTime();
	
	ENGINE::CGameObject::Update();

	for(int i = 0; i < 5; ++i)
	{
		if(m_pSnowMan[i] != NULL)
		{
			m_pSnowMan[i]->Update();
			m_pSnowMan[i]->SetPos(m_pInfo->m_vPos);
		}
		if(m_pSnowMan_Body[i] != NULL)
		{
			m_pSnowMan_Body[i]->Update();
			m_pSnowMan_Body[i]->SetPos(m_pSnowMan[i]->m_pInfo->m_vPos);
		}
	}
	
	/*D3DXMATRIX matScale;
	D3DXMatrixIdentity(&matScale);
	matScale.m[1][1] = 4.f;

	m_pInfo->m_matWorld = matScale * m_pInfo->m_matWorld;*/

	CObstacle::Update();
	SetTransForm();

	
}

void CSpinSnow::Render(void)
{
	//m_pDevice->SetTransform(D3DTS_WORLD, &m_pInfo->m_matWorld);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00000088);
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pResourceMgr->SetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer_CubeTex", m_pConvertVertex);

	if(m_bBehind == false)
	{
		m_pTexture->Render(0);

		m_pBuffer->Render();
		
	}
	for(int i = 0; i < 5; ++i)
	{
		if(m_pSnowMan[i] != NULL)
			m_pSnowMan[i]->Render();
		if(m_pSnowMan_Body[i] != NULL)
			m_pSnowMan_Body[i]->Render();
	}
	
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//

	int k = m_pTexture->m_iImgCnt;
	int j = g_iImgCnt;

	int iTotal = k + j;
	
}

void CSpinSnow::Release(void)
{
	m_pInfoSubject->UnSubscribe(m_pCameraObserver);
	ENGINE::Safe_Delete(m_pCameraObserver);

	if(m_pVertex)
		ENGINE::Safe_Delete_Array(m_pVertex);

	if(m_pConvertVertex)
		ENGINE::Safe_Delete_Array(m_pConvertVertex);
	
	if(m_pColVtx)
		ENGINE::Safe_Delete_Array(m_pColVtx);

	for(int i = 0; i < 5; ++i)
	{
		if(m_pSnowMan[i])
		{
			ENGINE::Safe_Delete(m_pSnowMan[i]);
		}

		if(m_pSnowMan_Body[i])
		{
			ENGINE::Safe_Delete(m_pSnowMan_Body[i]);	
		}
	}
	
}

HRESULT CSpinSnow::AddComponent(void)
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
	pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_STATIC, L"SnowFloor");
	m_pTexture = dynamic_cast<ENGINE::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));


	return S_OK;
}

CSpinSnow* CSpinSnow::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CSpinSnow* pCubeObj = new CSpinSnow(pDevice);

	if(FAILED(pCubeObj->Initialize()))
		ENGINE::Safe_Delete(pCubeObj);

	return pCubeObj;
}


void CSpinSnow::SetTransForm(void)
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

void CSpinSnow::SnowManCreate(void)
{

	for(int i = 0; i < 5; ++i)
	{
		float X = (m_pInfo->m_vPos.x - 10.f) + (i * 5);
		m_pSnowMan[i] = CSpinSnowMan::Create(m_pDevice);
		m_pSnowMan[i]->setvTrans(D3DXVECTOR3(X, 3.6f, 0.f));
		m_pSnowMan[i]->SetvScale(D3DXVECTOR3(1.5f, 1.5f, 1.5f));

		m_pSnowMan_Body[i] = CSpinSnowMan::Create(m_pDevice);
		m_pSnowMan_Body[i]->setvTrans(D3DXVECTOR3(X, 0.6f, 0.f));
		m_pSnowMan_Body[i]->SetvScale(D3DXVECTOR3(1.8f, 1.5f, 1.5f));
	}

	ChangeTexture(m_pSnowMan[4], L"SnowHead1");
	ChangeTexture(m_pSnowMan_Body[4], L"SnowBody1");

	ChangeTexture(m_pSnowMan[3], L"SnowHead2");
	ChangeTexture(m_pSnowMan_Body[3], L"SnowBody2");

	ChangeTexture(m_pSnowMan[2], L"SnowHead3");
	ChangeTexture(m_pSnowMan_Body[2], L"SnowBody3");

	ChangeTexture(m_pSnowMan[1], L"SnowHead4");
	ChangeTexture(m_pSnowMan_Body[1], L"SnowBody4");

	ChangeTexture(m_pSnowMan[0], L"SnowHead5");
	ChangeTexture(m_pSnowMan_Body[0], L"SnowBody5");
	
}
void CSpinSnow::SetPos(const D3DXVECTOR3& vPos)
{
	m_pInfo->m_vPos = vPos;
	m_vCopyPos = vPos;
}

const	ENGINE::VTXCUBE* CSpinSnow::GetCubeVertex()
{
	return m_pColVtx;
}

D3DXVECTOR3* CSpinSnow::GetPos(void)
{
	return &m_vCopyPos;
}
