#include "stdafx.h"
#include "CrashPattern.h"

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

CPattern2::CPattern2(LPDIRECT3DDEVICE9 pDevice)
: CObstacle(pDevice)
, m_pResourceMgr(ENGINE::Get_ResourcesMgr())
, m_pManagement(ENGINE::Get_Management())
, m_pTimeMgr(ENGINE::Get_Time())
, m_pInfoSubject(ENGINE::Get_InfoSubject())
, m_pCollisionMgr(CCollisionMgr::GetInstance())
, m_pTerrainCol(NULL)
, m_pTerrainVertex(NULL)
, m_bMove(false)
, m_bPeak(false)
{

}

CPattern2::~CPattern2()
{
	Release();
}


HRESULT CPattern2::Initialize(void)
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

	m_dwTime = GetTickCount();

	TexToPos();

	return S_OK;
}

void CPattern2::Update(void)
{
	D3DXVec3TransformNormal(&m_pInfo->m_vDir, &g_vLook, &m_pInfo->m_matWorld);
	D3DXVec3Normalize(&m_pInfo->m_vDir, &m_pInfo->m_vDir);

	ENGINE::CGameObject::Update();


	D3DXMATRIX matTran,matScale;
	D3DXMatrixIdentity(&matScale);
	matScale.m[0][0] = memScale.x;
	matScale.m[1][1] = memScale.y;
	matScale.m[2][2] = memScale.z;
	D3DXMatrixIdentity(&matRot);
	

	D3DXMatrixIdentity(&matTran);
	D3DXMatrixTranslation(&matTran, memTrans.x, memTrans.y, memTrans.z);
	m_pInfo->m_matWorld = matScale * matTran  * m_pInfo->m_matWorld;


	if(eMoveState == MW_X)
	{
		if(m_bPeak == false)
		{
			m_pInfo->m_vPos.x += m_pTimeMgr->GetInstance()->GetTime() * m_fSpeed;
			m_pInfo->m_vPos.y = memOriginPos.y;
			m_pInfo->m_vPos.z = memOriginPos.z;

			if(memOriginPos.x + 20 < m_pInfo->m_vPos.x)
		 {
			 m_bPeak = true;
		 }
		}
		if(m_bPeak == true)
		{
			m_pInfo->m_vPos.x -= m_pTimeMgr->GetInstance()->GetTime() * m_fSpeed;
			m_pInfo->m_vPos.y = memOriginPos.y;
			m_pInfo->m_vPos.z = memOriginPos.z;

			if(memOriginPos.x > m_pInfo->m_vPos.x)
				m_bPeak = false;
		}
	}

	if(eMoveState == MW_Y)
	{
		if(m_bPeak == false)
		{
			m_pInfo->m_vPos.y += m_pTimeMgr->GetInstance()->GetTime() * m_fSpeed;
			m_pInfo->m_vPos.x = memOriginPos.x;
			m_pInfo->m_vPos.z = memOriginPos.z;

			if(memOriginPos.y + 20 < m_pInfo->m_vPos.y)
		 {
			 m_bPeak = true;
		 }
		}
		if(m_bPeak == true)
		{
			m_pInfo->m_vPos.y -= m_pTimeMgr->GetInstance()->GetTime() * m_fSpeed;
			m_pInfo->m_vPos.x = memOriginPos.x;
			m_pInfo->m_vPos.z = memOriginPos.z;

			if(memOriginPos.y > m_pInfo->m_vPos.y)
				m_bPeak = false;
		}
	}

	if(eMoveState == MW_Z)
	{
		if(m_bPeak == false)
		{
			m_pInfo->m_vPos.z += m_pTimeMgr->GetInstance()->GetTime() * m_fSpeed;
			m_pInfo->m_vPos.y = memOriginPos.y;
			m_pInfo->m_vPos.x = memOriginPos.x;

			if(memOriginPos.z + 20 < m_pInfo->m_vPos.z)
		 {
			 m_bPeak = true;
		 }
		}
		if(m_bPeak == true)
		{
			m_pInfo->m_vPos.z -= m_pTimeMgr->GetInstance()->GetTime() * m_fSpeed;
			m_pInfo->m_vPos.y = memOriginPos.y;
			m_pInfo->m_vPos.x = memOriginPos.x;

			if(memOriginPos.z > m_pInfo->m_vPos.z)
				m_bPeak = false;
		}
	}

	if(eMoveState == MW_RevX)
	{
		if(m_bPeak == false)
		{
			m_pInfo->m_vPos.x -= m_pTimeMgr->GetInstance()->GetTime() * m_fSpeed;
			m_pInfo->m_vPos.y = memOriginPos.y;
			m_pInfo->m_vPos.z = memOriginPos.z;

			if(memOriginPos.x - 20 > m_pInfo->m_vPos.x)
		 {
			 m_bPeak = true;
		 }
		}
		if(m_bPeak == true)
		{
			m_pInfo->m_vPos.x += m_pTimeMgr->GetInstance()->GetTime() * m_fSpeed;
			m_pInfo->m_vPos.y = memOriginPos.y;
			m_pInfo->m_vPos.z = memOriginPos.z;

			if(memOriginPos.x < m_pInfo->m_vPos.x)
				m_bPeak = false;
		}
	}

	if(eMoveState == MW_RevY)
	{
		if(m_bPeak == false)
		{
			m_pInfo->m_vPos.y -= m_pTimeMgr->GetInstance()->GetTime() * m_fSpeed;
			m_pInfo->m_vPos.x = memOriginPos.x;
			m_pInfo->m_vPos.z = memOriginPos.z;

			if(memOriginPos.y - 20 > m_pInfo->m_vPos.y)
		 {
			 m_bPeak = true;
		 }
		}
		if(m_bPeak == true)
		{
			m_pInfo->m_vPos.y += m_pTimeMgr->GetInstance()->GetTime() * m_fSpeed;
			m_pInfo->m_vPos.x = memOriginPos.x;
			m_pInfo->m_vPos.z = memOriginPos.z;

			if(memOriginPos.y < m_pInfo->m_vPos.y)
				m_bPeak = false;
		}
	}

	if(eMoveState == MW_RevZ)
	{
		if(m_bPeak == false)
		{
			m_pInfo->m_vPos.z -= m_pTimeMgr->GetInstance()->GetTime() * m_fSpeed;
			m_pInfo->m_vPos.x = memOriginPos.x;
			m_pInfo->m_vPos.y = memOriginPos.y;

			if(memOriginPos.z - 20 > m_pInfo->m_vPos.z)
		 {
			 m_bPeak = true;
		 }
		}
		if(m_bPeak == true)
		{
			m_pInfo->m_vPos.z += m_pTimeMgr->GetInstance()->GetTime() * m_fSpeed;
			m_pInfo->m_vPos.x = memOriginPos.x;
			m_pInfo->m_vPos.y = memOriginPos.y;

			if(memOriginPos.z < m_pInfo->m_vPos.z)
				m_bPeak = false;
		}
	}

	SetTransForm();

}

void CPattern2::Render(void)
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

	m_pTexture->Render(0);

	m_pBuffer->Render();


	/*m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);*/
}

void CPattern2::Release(void)
{
	ENGINE::Safe_Delete(m_pCameraObserver);

	if(m_pVertex)
		ENGINE::Safe_Delete_Array(m_pVertex);

	if(m_pConvertVertex)
		ENGINE::Safe_Delete_Array(m_pConvertVertex);
}

HRESULT CPattern2::AddComponent(void)
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

CPattern2* CPattern2::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CPattern2* pPattern2 = new CPattern2(pDevice);

	if(FAILED(pPattern2->Initialize()))
		ENGINE::Safe_Delete(pPattern2);

	return pPattern2;
}

void CPattern2::SetTransForm(void)
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

void CPattern2::KeyInput(void)
{

}

void CPattern2::Move(void)
{

}

void CPattern2::SetPos(const D3DXVECTOR3& vPos)
{
	m_pInfo->m_vPos = vPos;
}

void CPattern2::SetScale(const D3DXVECTOR3& vScale)
{
	memScale = vScale;
}

D3DXVECTOR3* CPattern2::GetPos(void)
{
	return &m_pInfo->m_vPos;
}

const	ENGINE::VTXCUBE* CPattern2::GetCubeVertex()
{
	return m_pColVtx;
}

D3DXVECTOR3 CPattern2::GetMaxPos(const ENGINE::VTXCUBE* pCube)
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

D3DXVECTOR3 CPattern2::GetMinPos(const ENGINE::VTXCUBE* pCube)
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

void CPattern2::SetTrans(const D3DXVECTOR3& vTrans)
{
	memTrans = vTrans;
}


void CPattern2::SetSpeed(const float& fSpeed)
{
	m_fSpeed = fSpeed;
}

void CPattern2::SetOriginPos(const D3DXVECTOR3& vOriginPos)
{
	memOriginPos = vOriginPos;
}

void CPattern2::SetMoveWay(const MOVEWAY& eState)
{
	eMoveState = eState;
}