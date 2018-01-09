#include "stdafx.h"
#include "ShrinkingPlatform.h"

#include "Include.h"
#include "../Reference/Headers/Texture.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Pipeline.h"
#include "CameraObserver.h"
#include "CollisionMgr.h"
#include "TerrainCol.h"
#include "Stage.h"
#include "CrashPattern.h"

CShrinkingPlatform::CShrinkingPlatform(LPDIRECT3DDEVICE9 pDevice)
: CObstacle(pDevice)
, m_pResourceMgr(ENGINE::Get_ResourcesMgr())
, m_pManagement(ENGINE::Get_Management())
, m_pTimeMgr(ENGINE::Get_Time())
, m_pInfoSubject(ENGINE::Get_InfoSubject())
, m_pCollisionMgr(CCollisionMgr::GetInstance())
, m_pTerrainCol(NULL)
, m_pTerrainVertex(NULL)
, m_pPattern1(NULL)
, m_pPattern2(NULL)
, m_bPosUpdate1(false)
, m_bPosUpdate2(false)
{

}

CShrinkingPlatform::~CShrinkingPlatform()
{
	Release();
}


HRESULT CShrinkingPlatform::Initialize(void)
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

	m_pPattern1 = CPattern2::Create(m_pDevice);
	m_pPattern1->SetPos(D3DXVECTOR3(50.f, 10.f, 50.f));
	m_pPattern1->SetScale(D3DXVECTOR3(10.f, 0.5f, 5.f));
	m_pPattern1->SetTrans(D3DXVECTOR3(2.f, 0.f, 0.f));
	m_pPattern1->SetSpeed(0.f);
	m_pPattern1->SetOriginPos(D3DXVECTOR3(50.f, 1.f, 50.f));
	m_pPattern1->SetMoveWay(CPattern2::MW_X);

	m_pPattern2 = CPattern2::Create(m_pDevice);
	m_pPattern2->SetPos(D3DXVECTOR3(59.f, 10.f, 59.f));
	m_pPattern2->SetScale(D3DXVECTOR3(10.f, 0.5f, 5.f));
	m_pPattern2->SetTrans(D3DXVECTOR3(2.f, 0.f, 0.f));
	m_pPattern2->SetSpeed(0.f);
	m_pPattern2->SetOriginPos(D3DXVECTOR3(50.f, 1.f, 50.f));
	m_pPattern2->SetMoveWay(CPattern2::MW_RevX);

	FAILED_CHECK(AddComponent());

	m_iflag = 6;

	TexToPos();

	ChangeTexture(m_pPattern1, L"Platform");
	ChangeTexture(m_pPattern2, L"Platform");

	return S_OK;
}

void CShrinkingPlatform::Update(void)
{

	//m_pTerrainVertex = m_pManagement->GetTerrainVertex(CStage::LAYER_GAMELOGIC, L"Terrain");

	//m_pTerrainCol->SetColInfo(&m_pInfo->m_vPos, m_pTerrainVertex);

	D3DXVec3TransformNormal(&m_pInfo->m_vDir, &g_vLook, &m_pInfo->m_matWorld);
	D3DXVec3Normalize(&m_pInfo->m_vDir, &m_pInfo->m_vDir);

	ENGINE::CGameObject::Update();
	if(m_pPattern1 != NULL)
	{
		m_pPattern1->Update();
		if(m_bPosUpdate1 == false)
		{
			m_pPattern1->SetPos(D3DXVECTOR3(m_pInfo->m_vPos.x, m_pInfo->m_vPos.y, m_pInfo->m_vPos.z));
			m_bPosUpdate1 = true;
		}
		m_pPattern1->SetScale(D3DXVECTOR3(5.f, 0.5f, 10.f));
		m_pPattern1->SetTrans(D3DXVECTOR3(m_pInfo->m_vPos.x - 25.f, m_pInfo->m_vPos.y, 0.f));
		m_pPattern1->SetSpeed(5.f);
		m_pPattern1->SetOriginPos(D3DXVECTOR3(m_pInfo->m_vPos.x, m_pInfo->m_vPos.y, m_pInfo->m_vPos.z));
		m_pPattern1->SetMoveWay(CPattern2::MW_X);
	}
	if(m_pPattern2 != NULL)
	{
		m_pPattern2->Update();
		if(m_bPosUpdate2 == false)
		{
			m_pPattern2->SetPos(D3DXVECTOR3(m_pInfo->m_vPos.x + 50.f , m_pInfo->m_vPos.y, m_pInfo->m_vPos.z));
			m_bPosUpdate2 = true;
		}
		m_pPattern2->SetScale(D3DXVECTOR3(5.f, 0.5f, 10.f));
		m_pPattern2->SetTrans(D3DXVECTOR3(m_pInfo->m_vPos.x - 25.f, m_pInfo->m_vPos.y, 0.f));
		m_pPattern2->SetSpeed(5.f);
		m_pPattern2->SetOriginPos(D3DXVECTOR3(m_pInfo->m_vPos.x + 50.f , m_pInfo->m_vPos.y, m_pInfo->m_vPos.z));
		m_pPattern1->SetMoveWay(CPattern2::MW_X);
	}
	D3DXMATRIX matScale;
	D3DXMatrixIdentity(&matScale);
	matScale.m[1][1] = 4.f;

	m_pInfo->m_matWorld = matScale * m_pInfo->m_matWorld;

	SetTransForm();

	
}

void CShrinkingPlatform::Render(void)
{
	//m_pDevice->SetTransform(D3DTS_WORLD, &m_pInfo->m_matWorld);

	/*m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00000088);
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);*/

	//m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pResourceMgr->SetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer_CubeTex", m_pConvertVertex);

	m_pTexture->Render(0);

	m_pBuffer->Render();

	if(m_pPattern1 != NULL)
		m_pPattern1->Render();
	if(m_pPattern2 != NULL)
		m_pPattern2->Render();
	

	/*m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);*/
	//
	
}

void CShrinkingPlatform::Release(void)
{
	ENGINE::Safe_Delete(m_pCameraObserver);

	if(m_pVertex)
		ENGINE::Safe_Delete_Array(m_pVertex);

	if(m_pConvertVertex)
		ENGINE::Safe_Delete_Array(m_pConvertVertex);
	
	if(m_pColVtx)
		ENGINE::Safe_Delete_Array(m_pColVtx);

	if(m_pPattern1)
	{
		m_pInfoSubject->UnSubscribe(m_pPattern1->m_pCameraObserver);
		ENGINE::Safe_Delete(m_pPattern1);
	}

	if(m_pPattern2)
	{
		m_pInfoSubject->UnSubscribe(m_pPattern2->m_pCameraObserver);
		ENGINE::Safe_Delete(m_pPattern2);
	}

}

HRESULT CShrinkingPlatform::AddComponent(void)
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

	//// Collision Terrain
	//pComponent = m_pCollisionMgr->CloneColObject(CCollisionMgr::COL_TERRAIN);
	//m_pTerrainCol = dynamic_cast<CTerrainCol*>(pComponent);
	//NULL_CHECK_RETURN(m_pTerrainCol, E_FAIL);
	//m_mapComponent.insert(MAPCOMPONENT::value_type(L"TerrainCol", pComponent));
	
	// Texture
	pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, m_wstrObjKey);
	m_pTexture = dynamic_cast<ENGINE::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));


	return S_OK;
}

CShrinkingPlatform* CShrinkingPlatform::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CShrinkingPlatform* pCubeObj = new CShrinkingPlatform(pDevice);

	if(FAILED(pCubeObj->Initialize()))
		ENGINE::Safe_Delete(pCubeObj);

	return pCubeObj;
}


void CShrinkingPlatform::SetTransForm(void)
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

void CShrinkingPlatform::SetPos(const D3DXVECTOR3& vPos)
{
	m_pInfo->m_vPos = vPos;
	m_vCopyPos = vPos;
	memOriginPos = vPos;
}

const	ENGINE::VTXCUBE* CShrinkingPlatform::GetCubeVertex()
{
	return m_pColVtx;
}

D3DXVECTOR3* CShrinkingPlatform::GetPos(void)
{
	return &m_vCopyPos;
}

void CShrinkingPlatform::SetScale(const D3DXVECTOR3& fScale)
{
	m_vMazeScale = fScale;
}

void CShrinkingPlatform::SetTrans(const float& fTrans)
{
	m_fMazeTrans = fTrans;
}
void CShrinkingPlatform::SetOriginPos(const D3DXVECTOR3& vOriginPos)
{
	memOriginPos = vOriginPos;
}

void CShrinkingPlatform::SetSpeed(const float& fSpeed)
{
	m_fSpeed = fSpeed;
}
