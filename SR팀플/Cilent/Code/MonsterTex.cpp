#include "stdafx.h"
#include "MonsterTex.h"

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

CMonsterTex::CMonsterTex(LPDIRECT3DDEVICE9 pDevice)
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
, m_bDead(false)
, m_bDelete(false)
, m_iFrame(0)
, m_fFrame2(0.f)
{

}

CMonsterTex::~CMonsterTex()
{
	Release();
}
HRESULT CMonsterTex::Initialize(void)
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
void CMonsterTex::Update(void)
{
	//m_pTerrainVertex = m_pManagement->GetTerrainVertex(CStage::LAYER_GAMELOGIC, L"Terrain");
	//m_pTerrainCol->SetColInfo(&m_pInfo->m_vPos, m_pTerrainVertex);

	D3DXVec3TransformNormal(&m_pInfo->m_vDir, &g_vLook, &m_pInfo->m_matWorld);
	D3DXVec3Normalize(&m_pInfo->m_vDir, &m_pInfo->m_vDir);

	m_pInfo->m_vPos = *m_pPos;

	SetFrame();
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

	if(m_bDead)
	{
		m_fFrame2 +=  m_pTimeMgr->GetTime() * 15;

		if(m_fFrame2 > 19)
		{
			m_fFrame2  = 0.f;
			m_bDelete = true;			
		}		
	}

	SetTransForm();

}
CMonsterTex* CMonsterTex::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMonsterTex* pMonster = new CMonsterTex(pDevice);

	if(FAILED(pMonster->Initialize()))
		ENGINE::Safe_Delete(pMonster);

	return pMonster;
}
HRESULT CMonsterTex::AddComponent(void)
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
	pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_STATIC, L"Sentry_Sentry");
	m_pTexture[0] = dynamic_cast<ENGINE::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	// Texture
	pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_STATIC, L"Sentry_Blast");
	m_pTexture[1] = dynamic_cast<ENGINE::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	return S_OK;

}

void CMonsterTex::Render(void)
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

	if(!m_bDead && !m_bDelete)
		m_pTexture[0]->Render(m_iFrame);
	else if(m_bDead && !m_bDelete)
		m_pTexture[1]->Render(int(m_fFrame2));

	m_pBuffer->Render();

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}
void CMonsterTex::Release(void)
{
	m_pInfoSubject->UnSubscribe(m_pCameraObserver);
	ENGINE::Safe_Delete(m_pCameraObserver);

	if(m_pVertex)
		ENGINE::Safe_Delete_Array(m_pVertex);

	if(m_pConvertVertex)
		ENGINE::Safe_Delete_Array(m_pConvertVertex);
}

void CMonsterTex::SetTransForm(void)
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
void CMonsterTex::SetMonsterTex(D3DXVECTOR3* vPos)
{
	m_pPos = vPos;
}

void CMonsterTex::SetFrame(void)
{
	list<ENGINE::CGameObject*>* pPlayerList = m_pManagement->GetList(CStage::LAYER_GAMELOGIC,L"Player");
	m_pTarget = pPlayerList->front();

	ENGINE::CTransform* TargetInfo = ((CPlayer*)m_pTarget)->GetInfo();

	if(TargetInfo->m_vPos.x > m_pInfo->m_vPos.x - (m_pInfo->m_vScale.x / 2.f))
		m_iFrame = 1;
	if(TargetInfo->m_vPos.x < m_pInfo->m_vPos.x + (m_pInfo->m_vScale.x / 2.f))
		m_iFrame = 3;
	if(TargetInfo->m_vPos.x > m_pInfo->m_vPos.x - (m_pInfo->m_vScale.x / 2.f) &&
		TargetInfo->m_vPos.x < m_pInfo->m_vPos.x + (m_pInfo->m_vScale.x / 2.f))
	{
		m_iFrame = 0;
	}
}

