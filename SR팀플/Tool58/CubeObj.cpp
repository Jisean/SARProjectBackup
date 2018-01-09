#include "stdafx.h"
#include "CubeObj.h"

#include "Include.h"
#include "Texture.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Pipeline.h"
#include "CameraObserver.h"
#include "TerrainCol.h"
#include "../Reference/Headers/Texture.h"
#include "CubeTex.h"
#include "VIBuffer.h"
#include "TriCol.h"
#include "RcTex.h"
#include "CollisionMgr.h"
#include "MouseCol.h"
#include "Back.h"
#include "MainFrm.h"
#include "Tool58View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CCubeObj::CCubeObj(LPDIRECT3DDEVICE9 pDevice)
: CObstacle(pDevice)
, m_pResourceMgr(ENGINE::Get_ResourcesMgr())
, m_pManagement(ENGINE::Get_Management())
, m_pTimeMgr(ENGINE::Get_Time())
, m_pInfoSubject(ENGINE::Get_InfoSubject())
, m_pCollisionMgr(CCollisionMgr::GetInstance())
, m_pTerrainVertex(NULL)
{
	m_wstrObjKey = L"Monster1";
}

CCubeObj::~CCubeObj()
{
	Release();
}


HRESULT CCubeObj::Initialize(void)
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE); // Α¶Έν

	m_pCameraObserver = CCameraObserver::Create();
	m_pInfoSubject->Subscribe(m_pCameraObserver);

	m_fDistance = 10.f;
	
	m_dwVtxCnt = 8;

	m_pVertex			= new ENGINE::VTXCUBE[m_dwVtxCnt];
	m_pConvertVertex	 = new ENGINE::VTXCUBE[m_dwVtxCnt];

	/*if(wstrObjKey != L"")
		m_wstrObjKey = wstrObjKey;*/

	m_pResourceMgr->GetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer_CubeTex", m_pVertex);

	FAILED_CHECK(AddComponent());

	TexToPos();

	m_iflag = 0;

	return S_OK;
}

void CCubeObj::Update(void)
{
	/*m_pTerrainVertex = m_pManagement->GetTerrainVertex(CStage::LAYER_GAMELOGIC, L"Terrain");

	m_pTerrainCol->SetColInfo(&m_pInfo->m_vPos, m_pTerrainVertex);*/


	D3DXVec3TransformNormal(&m_pInfo->m_vDir, &g_vLook, &m_pInfo->m_matWorld);
	D3DXVec3Normalize(&m_pInfo->m_vDir, &m_pInfo->m_vDir);

	ENGINE::CGameObject::Update();
	//m_pMouseCol->Update();

	SetTransForm();

}

void CCubeObj::Render(void)
{
	//m_pDevice->SetTransform(D3DTS_WORLD, &m_pInfo->m_matWorld);

	/*m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00000088);
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);*/

	//((ENGINE::CVIBuffer*)m_pResources)->SetVtxInfo(m_pConvertVertex);
	m_pResourceMgr->SetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer_CubeTex", m_pConvertVertex);
	
	m_pTexture->Render(0);

	m_pBuffer->Render();

	//m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	//m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	
	
}

void CCubeObj::Release(void)
{
	if(m_pVertex)
		ENGINE::Safe_Delete_Array(m_pVertex);

	if(m_pConvertVertex)
		ENGINE::Safe_Delete_Array(m_pConvertVertex);

	m_pInfoSubject->UnSubscribe(m_pCameraObserver);
	ENGINE::Safe_Delete(m_pCameraObserver);

	for_each(m_mapComponent.begin(), m_mapComponent.end(), DELETEMAP());
}

HRESULT CCubeObj::AddComponent(void)
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
	
	// Texture
	pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, m_wstrObjKey);
	m_pTexture = dynamic_cast<ENGINE::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	//// Collision Mouse
	//pComponent = m_pCollisionMgr->CloneColObject(CCollisionMgr::COL_MOUSE);
	//m_pMouseCol = dynamic_cast<CMouseCol*>(pComponent);
	//NULL_CHECK_RETURN(m_pMouseCol, E_FAIL);
	//m_mapComponent.insert(MAPCOMPONENT::value_type(L"MouseCol", pComponent));


	return S_OK;
}

CCubeObj* CCubeObj::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CCubeObj* pCubeObj = new CCubeObj(pDevice);

	if(FAILED(pCubeObj->Initialize()))
		ENGINE::Safe_Delete(pCubeObj);

	return pCubeObj;
}


void CCubeObj::SetTransForm(void)
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

void CCubeObj::SetPos(const D3DXVECTOR3& vPos)
{
	m_pInfo->m_vPos = vPos;
}
void CCubeObj::TexToPos(void)
{
	for(int i = 0; i < 8; ++i)
	{
		m_pVertex[i].vPos = m_pVertex[i].vTex;
	}
}

