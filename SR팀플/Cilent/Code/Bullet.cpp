#include "stdafx.h"
#include "Bullet.h"

#include "Include.h"
#include "Texture.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Pipeline.h"
#include "CameraObserver.h"
#include "CollisionMgr.h"
#include "TerrainCol.h"
#include "Stage.h"

CBullet::CBullet(LPDIRECT3DDEVICE9 pDevice)
: CObstacle(pDevice)
, m_pResourceMgr(ENGINE::Get_ResourcesMgr())
, m_pManagement(ENGINE::Get_Management())
, m_pTimeMgr(ENGINE::Get_Time())
, m_pInfoSubject(ENGINE::Get_InfoSubject())
, m_pCollisionMgr(CCollisionMgr::GetInstance())
, m_pTerrainCol(NULL)
, m_pTerrainVertex(NULL)
{

}

CBullet::~CBullet()
{
	Release();
}


HRESULT CBullet::Initialize(void)
{
	SetTransForm();
	m_pCameraObserver = CCameraObserver::Create();
	m_pInfoSubject->Subscribe(m_pCameraObserver);

	m_fDistance = 10.f;
	m_fSpeed = 5.f;
	
	m_dwVtxCnt = 8;
	m_fCameraAngle = D3DXToRadian(45.f);

	m_pVertex 			= new ENGINE::VTXCUBE[m_dwVtxCnt];
	m_pConvertVertex	= new ENGINE::VTXCUBE[m_dwVtxCnt];
	m_pColVtx			= new ENGINE::VTXCUBE[m_dwVtxCnt];

	m_pResourceMgr->GetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer_CubeTex", m_pVertex);

	m_pTimeMgr->InitTime();

	FAILED_CHECK(AddComponent());

	TexToPos();


	m_iflag = 3;

	return S_OK;
}

void CBullet::Update(void)
{
	/*m_pTimeMgr->SetTime();*/

	//m_pTerrainVertex = m_pManagement->GetTerrainVertex(CStage::LAYER_GAMELOGIC, L"Terrain");

	//m_pTerrainCol->SetColInfo(&m_pInfo->m_vPos, m_pTerrainVertex);


	//D3DXVec3Normalize(&m_pInfo->m_vDir, &m_pInfo->m_vDir);
	//D3DXVec3TransformNormal(&m_pInfo->m_vDir, &g_vLook, &m_pInfo->m_matWorld);
	//D3DXVec3Normalize(&m_pInfo->m_vDir, &m_pInfo->m_vDir);

	ENGINE::CGameObject::Update();

	D3DXMATRIX	matScale;
	D3DXMatrixIdentity(&matScale);
	matScale.m[0][0] = 0.5f;
	matScale.m[1][1] = 0.5f;
	matScale.m[2][2] = 0.5f;

	m_pInfo->m_matWorld = matScale * m_pInfo->m_matWorld;

	m_pInfo->m_vPos += m_pInfo->m_vDir * m_fSpeed * ENGINE::CTimeMgr::GetInstance()->GetTime();
	m_vCopyPos = m_pInfo->m_vPos;

	CObstacle::Update();
	SetTransForm();

	
}

void CBullet::Render(void)
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
	
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//
	
}

void CBullet::Release(void)
{
	m_pInfoSubject->UnSubscribe(m_pCameraObserver);
	ENGINE::Safe_Delete(m_pCameraObserver);

	if(m_pVertex)
		ENGINE::Safe_Delete_Array(m_pVertex);

	if(m_pConvertVertex)
		ENGINE::Safe_Delete_Array(m_pConvertVertex);
	
	if(m_pColVtx)
		ENGINE::Safe_Delete_Array(m_pColVtx);
}

HRESULT CBullet::AddComponent(void)
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
	pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_STATIC, L"Fireball");
	m_pTexture = dynamic_cast<ENGINE::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));


	return S_OK;
}

CBullet* CBullet::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CBullet* pCubeObj = new CBullet(pDevice);

	if(FAILED(pCubeObj->Initialize()))
		ENGINE::Safe_Delete(pCubeObj);

	return pCubeObj;
}


void CBullet::SetTransForm(void)
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

void CBullet::SetPos(const D3DXVECTOR3& vPos)
{
	m_pInfo->m_vPos = vPos;
	m_vCopyPos = vPos;
}

const	ENGINE::VTXCUBE* CBullet::GetCubeVertex()
{
	return m_pColVtx;
}

D3DXVECTOR3* CBullet::GetPos(void)
{
	return &m_vCopyPos;
}
void	CBullet::SetBulletDir(D3DXVECTOR3 vDir)
{
	m_pInfo->m_vDir = vDir;
}