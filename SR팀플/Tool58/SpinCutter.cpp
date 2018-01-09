#include "stdafx.h"
#include "SpinCutter.h"

#include "Include.h"
#include "../Reference/Headers/Texture.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Pipeline.h"
#include "CameraObserver.h"
#include "CollisionMgr.h"
#include "TerrainCol.h"
#include "Stage.h"
#include "CutterPattern.h"

CSpinCutter::CSpinCutter(LPDIRECT3DDEVICE9 pDevice)
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
, m_pPattern3(NULL)
, m_pPattern4(NULL)
, m_pPattern5(NULL)
{

}

CSpinCutter::~CSpinCutter()
{
	Release();
}


HRESULT CSpinCutter::Initialize(void)
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

	m_pPattern1 = CPattern1::Create(m_pDevice);
	m_pPattern1->SetPos(D3DXVECTOR3(4.f, 3.f, 0.f));
	m_pPattern1->SetScale(D3DXVECTOR3(1.f, 1.f, 1.f));
	m_pPattern1->SetTrans(D3DXVECTOR3(0.f, 0.f, 0.f));
	m_pPattern1->SetRotState(CPattern1::ROT_Z);
	m_pPattern1->SetRotAngle(70.f);

	m_pPattern2 = CPattern1::Create(m_pDevice);
	m_pPattern2->SetPos(D3DXVECTOR3(4.f, 3.f, 0.f));
	m_pPattern2->SetScale(D3DXVECTOR3(1.f, 1.f, 1.f));
	m_pPattern2->SetTrans(D3DXVECTOR3(0.f, 0.f, 0.f));
	m_pPattern2->SetRotState(CPattern1::ROT_Z);
	m_pPattern2->SetRotAngle(70.f);

	m_pPattern3 = CPattern1::Create(m_pDevice);
	m_pPattern3->SetPos(D3DXVECTOR3(4.f, 3.f, 0.f));
	m_pPattern3->SetScale(D3DXVECTOR3(1.f, 1.f, 1.f));
	m_pPattern3->SetTrans(D3DXVECTOR3(0.f, 0.f, 0.f));
	m_pPattern3->SetRotState(CPattern1::ROT_Z);
	m_pPattern3->SetRotAngle(70.f);

	m_pPattern4 = CPattern1::Create(m_pDevice);
	m_pPattern4->SetPos(D3DXVECTOR3(4.f, 3.f, 0.f));
	m_pPattern4->SetScale(D3DXVECTOR3(1.f, 1.f, 1.f));
	m_pPattern4->SetTrans(D3DXVECTOR3(0.f, 0.f, 0.f));
	m_pPattern4->SetRotState(CPattern1::ROT_Z);
	m_pPattern4->SetRotAngle(70.f);

	m_pPattern5 = CPattern1::Create(m_pDevice);
	m_pPattern5->SetPos(D3DXVECTOR3(4.f, 3.f, 0.f));
	m_pPattern5->SetScale(D3DXVECTOR3(1.f, 1.f, 1.f));
	m_pPattern5->SetTrans(D3DXVECTOR3(0.f, 0.f, 0.f));
	m_pPattern5->SetRotState(CPattern1::ROT_Z);
	m_pPattern5->SetRotAngle(70.f);


	ChangeTexture(m_pPattern1, L"CutterPattern2");
	ChangeTexture(m_pPattern2, L"CutterPattern");
	ChangeTexture(m_pPattern3, L"CutterPattern2");
	ChangeTexture(m_pPattern4, L"CutterPattern");
	ChangeTexture(m_pPattern5, L"SpinCutter");

	FAILED_CHECK(AddComponent());

	TexToPos();

	m_iflag = 4;

	return S_OK;
}

void CSpinCutter::Update(void)
{

	//m_pTerrainVertex = m_pManagement->GetTerrainVertex(CStage::LAYER_GAMELOGIC, L"Terrain");

	//m_pTerrainCol->SetColInfo(&m_pInfo->m_vPos, m_pTerrainVertex);

	D3DXVec3TransformNormal(&m_pInfo->m_vDir, &g_vLook, &m_pInfo->m_matWorld);
	D3DXVec3Normalize(&m_pInfo->m_vDir, &m_pInfo->m_vDir);

	ENGINE::CGameObject::Update();
	if(m_pPattern1 != NULL)
	{
		m_pPattern1->Update();
		m_pPattern1->SetPos(D3DXVECTOR3(m_pInfo->m_vPos.x, 1.f, m_pInfo->m_vPos.z + 5.f));
		m_pPattern1->SetScale(D3DXVECTOR3(1.f, 5.f, 1.f));
		m_pPattern1->SetTrans(D3DXVECTOR3(0.f, 6.f, 0.f));
	}
	if(m_pPattern2 != NULL)
	{
		m_pPattern2->Update();
		m_pPattern2->SetPos(D3DXVECTOR3(m_pInfo->m_vPos.x, 1.f, m_pInfo->m_vPos.z));
		m_pPattern2->SetScale(D3DXVECTOR3(5.f, 1.f, 1.f));
		m_pPattern2->SetTrans(D3DXVECTOR3(-6.f, 0.f, 0.f));
	}
	if(m_pPattern3 != NULL)
	{
		m_pPattern3->Update();
		m_pPattern3->SetPos(D3DXVECTOR3(m_pInfo->m_vPos.x, 1.f, m_pInfo->m_vPos.z - 5.f));
		m_pPattern3->SetScale(D3DXVECTOR3(1.f, 5.f, 1.f));
		m_pPattern3->SetTrans(D3DXVECTOR3(0.f, -6.f, 0.f));
	}
	if(m_pPattern4 != NULL)
	{
		m_pPattern4->Update();
		m_pPattern4->SetPos(D3DXVECTOR3(m_pInfo->m_vPos.x, 1.f, m_pInfo->m_vPos.z - 10.f));
		m_pPattern4->SetScale(D3DXVECTOR3(5.f, 1.f, 1.f));
		m_pPattern4->SetTrans(D3DXVECTOR3(6.f, 0.f, 0.f));
	}
	if(m_pPattern5 != NULL)
	{
		m_pPattern5->Update();
		m_pPattern5->SetPos(D3DXVECTOR3(m_pInfo->m_vPos.x, 1.f, m_pInfo->m_vPos.z - 2.5f));
		m_pPattern5->SetScale(D3DXVECTOR3(1.f, 1.f, 10.f));
		m_pPattern5->SetTrans(D3DXVECTOR3(0.f, 0.f, 0.f));
	}
	D3DXMATRIX matScale;
	D3DXMatrixIdentity(&matScale);
	matScale.m[1][1] = 4.f;

	m_pInfo->m_matWorld = matScale * m_pInfo->m_matWorld;

	SetTransForm();

	
}

void CSpinCutter::Render(void)
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
	if(m_pPattern3 != NULL)
		m_pPattern3->Render();
	if(m_pPattern4 != NULL)
		m_pPattern4->Render();
	if(m_pPattern5 != NULL)
		m_pPattern5->Render();

	/*m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);*/
	//
	
}

void CSpinCutter::Release(void)
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

	if(m_pPattern3)
	{
		m_pInfoSubject->UnSubscribe(m_pPattern3->m_pCameraObserver);
		ENGINE::Safe_Delete(m_pPattern3);
	}

	if(m_pPattern4)
	{
		m_pInfoSubject->UnSubscribe(m_pPattern4->m_pCameraObserver);
		ENGINE::Safe_Delete(m_pPattern4);
	}

	if(m_pPattern5)
	{
		m_pInfoSubject->UnSubscribe(m_pPattern5->m_pCameraObserver);
		ENGINE::Safe_Delete(m_pPattern5);
	}

}

HRESULT CSpinCutter::AddComponent(void)
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

CSpinCutter* CSpinCutter::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CSpinCutter* pCubeObj = new CSpinCutter(pDevice);

	if(FAILED(pCubeObj->Initialize()))
		ENGINE::Safe_Delete(pCubeObj);

	return pCubeObj;
}


void CSpinCutter::SetTransForm(void)
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

void CSpinCutter::SetPos(const D3DXVECTOR3& vPos)
{
	m_pInfo->m_vPos = vPos;
	m_vCopyPos = vPos;
}

const	ENGINE::VTXCUBE* CSpinCutter::GetCubeVertex()
{
	return m_pColVtx;
}

D3DXVECTOR3* CSpinCutter::GetPos(void)
{
	return &m_vCopyPos;
}

void CSpinCutter::SetScale(const D3DXVECTOR3& fScale)
{
	m_vCutScale = fScale;
}

void CSpinCutter::SetTrans(const D3DXVECTOR3& fTrans)
{
	m_vCutTrans = fTrans;
}
