#include "stdafx.h"
#include "HotSixTex.h"

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
#include "Player.h"

CHotSixTex::CHotSixTex(LPDIRECT3DDEVICE9 pDevice)
:CGameObject(pDevice)
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
, m_pPos(NULL)
, m_dwVtxCnt(0)
, m_bMove(false)
{

}

CHotSixTex::~CHotSixTex()
{
	Release();
}
HRESULT CHotSixTex::Initialize(void)
{
	m_pCameraObserver = CCameraObserver::Create();
	m_pInfoSubject->Subscribe(m_pCameraObserver);

	m_fSpeed = 10.f;
	m_fDistance = 10.f;
	m_fCameraAngle = D3DXToRadian(45.f);

	m_dwVtxCnt = 4;
	m_pPos = NULL;

	m_pVertex = new ENGINE::VTXTEX[m_dwVtxCnt];
	m_pConvertVertex = new ENGINE::VTXTEX[m_dwVtxCnt];

	m_pResourceMgr->GetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer_RcTex", m_pVertex);

	FAILED_CHECK(AddComponent());

	return S_OK;
}
void CHotSixTex::Update(void)
{
	//m_pTerrainVertex = m_pManagement->GetTerrainVertex(CStage::LAYER_GAMELOGIC, L"Terrain");
	//m_pTerrainCol->SetColInfo(&m_pInfo->m_vPos, m_pTerrainVertex);

	D3DXVec3TransformNormal(&m_pInfo->m_vDir, &g_vLook, &m_pInfo->m_matWorld);
	D3DXVec3Normalize(&m_pInfo->m_vDir, &m_pInfo->m_vDir);

	m_pInfo->m_vPos = *m_pPos;

	ENGINE::CGameObject::Update();

	D3DXMATRIX		matBill;
	D3DXMatrixIdentity(&matBill);

	D3DXMATRIX		matView = *m_pCameraObserver->GetView();

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, 0, &matBill);

	m_pInfo->m_matWorld = matBill * m_pInfo->m_matWorld;

	D3DXMATRIX matScale;
	D3DXMatrixIdentity(&matScale);
	matScale.m[0][0] = 0.3f;
	matScale.m[1][1] = 0.6f;

	m_pInfo->m_matWorld = matScale * m_pInfo->m_matWorld;

	SetTransForm();

}
CHotSixTex* CHotSixTex::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CHotSixTex* pMonster = new CHotSixTex(pDevice);

	if(FAILED(pMonster->Initialize()))
		ENGINE::Safe_Delete(pMonster);

	return pMonster;
}
HRESULT CHotSixTex::AddComponent(void)
{
	ENGINE::CComponent*			pComponent = NULL;

	// Transform
	pComponent = m_pInfo = ENGINE::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	// Buffer
	pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, L"Buffer_RcTex");
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

	// Texture
	pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_STATIC, L"Texture_HotSix");
	m_pTexture = dynamic_cast<ENGINE::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	return S_OK;

}

void CHotSixTex::Render(void)
{
	//와이어
	//m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	//텍스처
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00000088);
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pResourceMgr->SetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer_RcTex", m_pConvertVertex);

	bool bBehind = false;
	CGameObject* pPlayer = ENGINE::CManagement::GetInstance()->GetList(CStage::LAYER_GAMELOGIC, L"Player")->front();
	D3DXVECTOR3 vLook(((CPlayer*)pPlayer)->GetInfo()->m_vDir);
	D3DXVec3Normalize(&vLook, &vLook);
	D3DXVECTOR3 vTemp;
	memcpy(&vTemp, &m_pInfo->m_matWorld.m[3][0], sizeof(float) * 3);
	D3DXVECTOR3 vPlayerToObstacle = vTemp - ((CPlayer*)pPlayer)->GetInfo()->m_vPos;
	D3DXVec3Normalize(&vPlayerToObstacle, &vPlayerToObstacle);
	float fCos = D3DXVec3Dot(&vLook, &vPlayerToObstacle);
	float fAngle = acosf(fCos);
	if(D3DXToRadian(120.f) <= fAngle)
	{
		bBehind = true;
	}
	else
	{
		bBehind = false;
	}
	if(bBehind == false)
	{
		m_pTexture->Render(0);

		m_pBuffer->Render();
	}

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}
void CHotSixTex::Release(void)
{
	m_pInfoSubject->UnSubscribe(m_pCameraObserver);
	ENGINE::Safe_Delete(m_pCameraObserver);

	if(m_pVertex)
		ENGINE::Safe_Delete_Array(m_pVertex);

	if(m_pConvertVertex)
		ENGINE::Safe_Delete_Array(m_pConvertVertex);
}

void CHotSixTex::SetTransForm(void)
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
void CHotSixTex::SetMonsterTex(D3DXVECTOR3* vPos)
{
	m_pPos = vPos;
}


