#include "stdafx.h"
#include "PlayerTex.h"

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

CPlayerTex::CPlayerTex(LPDIRECT3DDEVICE9 pDevice)
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
, m_pPos(NULL)
, m_dwVtxCnt(0)
, m_bMove(false)
{

}

CPlayerTex::~CPlayerTex()
{
	Release();
}


HRESULT CPlayerTex::Initialize(void)
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

void CPlayerTex::Update(void)
{
	

	m_pTerrainVertex = m_pManagement->GetTerrainVertex(CStage::LAYER_GAMELOGIC, L"Terrain");
	m_pTerrainCol->SetColInfo(&m_pInfo->m_vPos, m_pTerrainVertex);

	D3DXVec3TransformNormal(&m_pInfo->m_vDir, &g_vLook, &m_pInfo->m_matWorld);
	D3DXVec3Normalize(&m_pInfo->m_vDir, &m_pInfo->m_vDir);

	m_pInfo->m_vPos = *m_pPos;
	

	if(m_bMove)
		Move();

	ENGINE::CGameObject::Update();
	
	/*D3DXMATRIX		matBill;
	D3DXMatrixIdentity(&matBill);

	matBill = *m_pCameraObserver->GetView();
	ZeroMemory(&matBill.m[3][0], sizeof(D3DXVECTOR3));

	D3DXMatrixInverse(&matBill, 0, &matBill);
	m_pInfo->m_matWorld = matBill * m_pInfo->m_matWorld;*/
	D3DXMATRIX		matBill;
	D3DXMatrixIdentity(&matBill);

	D3DXMATRIX		matView = *m_pCameraObserver->GetView();

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, 0, &matBill);
	
	m_pInfo->m_matWorld = matBill * m_pInfo->m_matWorld;

	SetTransForm();

	//KeyInput();
}

void CPlayerTex::Render(void)
{
	//m_pDevice->SetTransform(D3DTS_WORLD, &m_pInfo->m_matWorld);

	//m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	/*m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00000088);
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);*/

	m_pResourceMgr->SetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer_RcTex", m_pConvertVertex);

	//m_pTexture->Render(0);

	//m_pBuffer->Render();

	/*m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);*/
	
}

void CPlayerTex::Release(void)
{
	m_pInfoSubject->UnSubscribe(m_pCameraObserver);
	ENGINE::Safe_Delete(m_pCameraObserver);

	if(m_pVertex)
		ENGINE::Safe_Delete_Array(m_pVertex);

	if(m_pConvertVertex)
		ENGINE::Safe_Delete_Array(m_pConvertVertex);
}

void CPlayerTex::KeyInput(void)
{
	float fTime = m_pTimeMgr->GetTime();
	

	if(GetAsyncKeyState(VK_UP))
	{
		m_pInfo->m_vPos += m_pInfo->m_vDir * fTime * m_fSpeed;
	}

	if(GetAsyncKeyState(VK_DOWN))
	{
		m_pInfo->m_vPos -= m_pInfo->m_vDir * fTime * m_fSpeed;
	}

	if(GetAsyncKeyState(VK_LEFT))
	{
		m_pInfo->m_fAngle[ENGINE::ANGLE_Y] -= D3DXToRadian(40.f) * fTime;
	}

	if(GetAsyncKeyState(VK_RIGHT))
	{
		m_pInfo->m_fAngle[ENGINE::ANGLE_Y] += D3DXToRadian(40.f) * fTime;
	}

	if(GetAsyncKeyState(VK_LBUTTON))
	{
		//m_bMove = true;

		//m_pMouseCol->PickTerrain(&m_vDestPos, m_pTerrainVertex);

		m_pMouseCol->PickObject(&m_vDestPos, m_pVertex, &m_pInfo->m_matWorld);
	}

	
}

HRESULT CPlayerTex::AddComponent(void)
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
	pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_STATIC, L"Player0");
	m_pTexture = dynamic_cast<ENGINE::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));


	return S_OK;
}

CPlayerTex* CPlayerTex::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CPlayerTex* pPlayer = new CPlayerTex(pDevice);

	if(FAILED(pPlayer->Initialize()))
		ENGINE::Safe_Delete(pPlayer);

	return pPlayer;
}


void CPlayerTex::SetTransForm(void)
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

void CPlayerTex::Move(void)
{
	D3DXVECTOR3		vDir;

	vDir = m_vDestPos - m_pInfo->m_vPos;

	float fDistance = D3DXVec3Length(&vDir);
	D3DXVec3Normalize(&vDir, &vDir);

	m_pInfo->m_vPos += vDir * m_fSpeed * m_pTimeMgr->GetTime();

	if(fDistance < 1.f)
		m_bMove = false;
}

void CPlayerTex::SetPlayerTex(D3DXVECTOR3* vPos)
{
	m_pPos = vPos;
}
ENGINE::VTXTEX*	CPlayerTex::GetpVertex(void)
{
	return m_pVertex;
}
ENGINE::CTransform* CPlayerTex::GetpInfo(void)
{
	return m_pInfo;
}