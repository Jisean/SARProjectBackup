#include "stdafx.h"
#include "Terrain.h"

#include "Export_Function.h"
#include "Transform.h"
#include "../Engine/Resources/Code/Texture.h"
#include "Include.h"
#include "Pipeline.h"
#include "CameraObserver.h"
#include "CollisionMgr.h"
#include "MouseCol.h"
#include "CubeObj.h"
#include "Back.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pDevice)
:CGameObject(pDevice)
, m_pResourceMgr(ENGINE::Get_ResourcesMgr())
, m_pManagement(ENGINE::Get_Management())
, m_pTimeMgr(ENGINE::Get_Time())
, m_pInfoSubject(ENGINE::Get_InfoSubject())
, m_pCameraObserver(NULL)
, m_pVertex(NULL)
, m_pConvertVertex(NULL)
, m_pCollisionMgr(CCollisionMgr::GetInstance())
,m_dwVtxCnt(0)
{
	

}

CTerrain::~CTerrain()
{
	Release();
}
HRESULT CTerrain::Initialize(void)
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE); // ����
	m_pCameraObserver = CCameraObserver::Create();
	m_pInfoSubject->Subscribe(m_pCameraObserver);

	m_fCameraAngle = D3DXToRadian(45.f);

	m_dwVtxCnt = VTXCNTX * VTXCNTZ;

	m_pVertex = new ENGINE::VTXTEX[m_dwVtxCnt];
	m_pConvertVertex = new ENGINE::VTXTEX[m_dwVtxCnt];

	m_pResourceMgr->GetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer_Terrain", m_pVertex);

	FAILED_CHECK(AddComponent());

	return S_OK;
}

CTerrain* CTerrain::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CTerrain*		pTerrain = new CTerrain(pDevice);
	
	if(FAILED(pTerrain->Initialize()))
		ENGINE::Safe_Delete(pTerrain);

	return pTerrain;
}

void CTerrain::Update(void)
{
	D3DXVec3TransformNormal(&m_pInfo->m_vDir, &g_vLook, &m_pInfo->m_matWorld);
	D3DXVec3Normalize(&m_pInfo->m_vDir, &m_pInfo->m_vDir);

	ENGINE::CGameObject::Update();

	SetTransForm();

	/*if(GetAsyncKeyState(VK_LBUTTON))
	{
		if(m_pMouseCol->PickTerrain(&m_vDestPos, m_pVertex))
		{
			CCubeObj* m_pCubeObj = CCubeObj::Create(ENGINE::CGraphicDev::GetInstance()->GetDevice());
			const D3DXVECTOR3 vPos(m_vDestPos.x, m_vDestPos.y + 1.f, m_vDestPos.z);
			m_pCubeObj->SetPos(vPos);
			CBack::m_vecCube.push_back(m_pCubeObj);
		}
	}*/

}

void CTerrain::Render(void)
{
	//m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pResourceMgr->SetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer_Terrain", m_pConvertVertex);
	
	m_pTexture->Render(0);

	m_pBuffer->Render();

}

void CTerrain::Release(void)
{
	ENGINE::Safe_Delete(m_pCameraObserver);

	if(m_pVertex)
		ENGINE::Safe_Delete_Array(m_pVertex);

	if(m_pConvertVertex)
		ENGINE::Safe_Delete_Array(m_pConvertVertex);
}

HRESULT CTerrain::AddComponent(void)
{
	ENGINE::CComponent*			pComponent = NULL;

	// Transform
	pComponent = m_pInfo = ENGINE::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	// Buffer
	pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, L"Buffer_Terrain");
	m_pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	// Texture
	pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, L"Terrain_Terrain");
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

void CTerrain::SetTransForm(void)
{
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

const ENGINE::VTXTEX* CTerrain::GetTerrainVertex()
{
	return m_pVertex;
}

