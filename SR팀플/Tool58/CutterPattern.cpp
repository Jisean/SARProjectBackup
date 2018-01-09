#include "stdafx.h"
#include "CutterPattern.h"

#include "Include.h"
#include "../Reference/Headers/Texture.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Pipeline.h"
#include "CameraObserver.h"
#include "CollisionMgr.h"
#include "TerrainCol.h"
#include "Stage.h"
#include "ObjectCol.h"
#include "KeyMgr.h"

CPattern1::CPattern1(LPDIRECT3DDEVICE9 pDevice)
: CObstacle(pDevice)
, m_pResourceMgr(ENGINE::Get_ResourcesMgr())
, m_pManagement(ENGINE::Get_Management())
, m_pTimeMgr(ENGINE::Get_Time())
, m_pInfoSubject(ENGINE::Get_InfoSubject())
, m_pCollisionMgr(CCollisionMgr::GetInstance())
, m_pTerrainCol(NULL)
, m_pTerrainVertex(NULL)
, m_bMove(false)
{

}

CPattern1::~CPattern1()
{
	Release();
}


HRESULT CPattern1::Initialize(void)
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

	TexToPos();

	m_dwTime = GetTickCount();

	m_fAngle = 0.f;

	return S_OK;
}

void CPattern1::Update(void)
{
	D3DXVec3TransformNormal(&m_pInfo->m_vDir, &g_vLook, &m_pInfo->m_matWorld);
	D3DXVec3Normalize(&m_pInfo->m_vDir, &m_pInfo->m_vDir);

	ENGINE::CGameObject::Update();

	m_fAngle += D3DXToRadian(m_fMemAngle) * ENGINE::CTimeMgr::GetInstance()->GetTime();

	D3DXMATRIX matTran,matScale;
	D3DXMatrixIdentity(&matScale);
	matScale.m[0][0] = memScale.x;
	matScale.m[1][1] = memScale.y;
	matScale.m[2][2] = memScale.z;
	D3DXMatrixIdentity(&matRot);
	
	if(eRotState == ROT_X)
		D3DXMatrixRotationX(&matRot, m_fAngle);
	if(eRotState == ROT_Y)
		D3DXMatrixRotationY(&matRot, m_fAngle);
	if(eRotState == ROT_Z)
		D3DXMatrixRotationZ(&matRot, m_fAngle);

	D3DXMatrixIdentity(&matTran);
	D3DXMatrixTranslation(&matTran, memTrans.x, memTrans.y, memTrans.z);
	m_pInfo->m_matWorld = matScale * matTran * matRot * m_pInfo->m_matWorld;

	SetTransForm();

}

void CPattern1::Render(void)
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

	m_pResourceMgr->SetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer_CubeTex", m_pConvertVertex);

	m_pTexture->Render(00);

	m_pBuffer->Render();


	/*m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);*/
}

void CPattern1::Release(void)
{
	ENGINE::Safe_Delete(m_pCameraObserver);

	if(m_pVertex)
		ENGINE::Safe_Delete_Array(m_pVertex);

	if(m_pConvertVertex)
		ENGINE::Safe_Delete_Array(m_pConvertVertex);
}

HRESULT CPattern1::AddComponent(void)
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

CPattern1* CPattern1::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CPattern1* pPattern1 = new CPattern1(pDevice);

	if(FAILED(pPattern1->Initialize()))
		ENGINE::Safe_Delete(pPattern1);

	return pPattern1;
}

void CPattern1::SetTransForm(void)
{

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

void CPattern1::KeyInput(void)
{

}

void CPattern1::Move(void)
{

}

void CPattern1::SetPos(const D3DXVECTOR3& vPos)
{
	m_pInfo->m_vPos = vPos;
}

void CPattern1::SetScale(const D3DXVECTOR3& vScale)
{
	memScale = vScale;
}

D3DXVECTOR3* CPattern1::GetPos(void)
{
	return &m_pInfo->m_vPos;
}

const	ENGINE::VTXCUBE* CPattern1::GetCubeVertex()
{
	return m_pColVtx;
}

D3DXVECTOR3 CPattern1::GetMaxPos(const ENGINE::VTXCUBE* pCube)
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

D3DXVECTOR3 CPattern1::GetMinPos(const ENGINE::VTXCUBE* pCube)
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

void CPattern1::SetTrans(const D3DXVECTOR3& vTrans)
{
	memTrans = vTrans;
}

void CPattern1::SetRotState(const ROTATEAXIS& eState)
{
	eRotState = eState;
}

void CPattern1::SetRotAngle(const float& fAngle)
{
	m_fMemAngle = fAngle;
}

