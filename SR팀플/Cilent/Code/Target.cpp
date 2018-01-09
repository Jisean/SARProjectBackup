#include "stdafx.h"
#include "Target.h"

#include "Include.h"
#include "Texture.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Pipeline.h"
#include "CameraObserver.h"

CTarget::CTarget(LPDIRECT3DDEVICE9 pDevice)
: CGameObject(pDevice)
, m_pResourceMgr(ENGINE::Get_ResourcesMgr())
, m_pManagement(ENGINE::Get_Management())
, m_pTimeMgr(ENGINE::Get_Time())
, m_pInfoSubject(ENGINE::Get_InfoSubject())
, m_pCameraObserver(NULL)
, m_pConvertVertex(NULL)
, m_pVertex(NULL)
, m_dwVtxCnt(0)
, m_fAngle(2.f)
, m_fDeadTime(0.f)
, m_bLife(true)
, m_bup(true)
, m_bisDead(false)
, m_bStart(true)
, m_bDown(false)
, m_bStay(false)
, m_bMiss(false)
, m_fStay(0.f)
{

}

CTarget::~CTarget()
{
	Release();
}
CTarget* CTarget::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CTarget*	pBook = new CTarget(pDevice);

	if(FAILED(pBook->Initialize()))
		ENGINE::Safe_Delete(pBook);


	return pBook;

}
HRESULT CTarget::Initialize(void)
{
	m_pCameraObserver = CCameraObserver::Create();
	m_pInfoSubject->Subscribe(m_pCameraObserver);

	m_dwVtxCnt = 4;

	m_pVertex			= new ENGINE::VTXTEX[m_dwVtxCnt];
	m_pConvertVertex	= new ENGINE::VTXTEX[m_dwVtxCnt];
	m_pVertexTemp		= new ENGINE::VTXTEX[m_dwVtxCnt];

	m_pResourceMgr->GetVtxInfo(ENGINE::RESOURCE_STATIC, L"Buffer_RcTarget", m_pVertex);

	for(int i = 0;  i != m_dwVtxCnt; ++i)
		m_pVertexTemp[i] = m_pVertex[i];

	m_pVertexTemp[0].vPos = D3DXVECTOR3(-1.f, 1.f, 0.f);
	m_pVertexTemp[1].vPos = D3DXVECTOR3(1.f, 1.f, 0.f);
	m_pVertexTemp[2].vPos = D3DXVECTOR3(1.f, -1.f, 0.f);
	m_pVertexTemp[3].vPos = D3DXVECTOR3(-1.f, -1.f, 0.f);

	FAILED_CHECK(AddComponet());

	return S_OK;
}

HRESULT CTarget::AddComponet(void)
{
	ENGINE::CComponent*			pComponent = NULL;

	// Transform
	pComponent = m_pInfo = ENGINE::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	// Buffer
	pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_STATIC, L"Buffer_RcTarget");
	m_pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	// Texture
	pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_STATIC, L"Texture_CBook");
	m_pTexture[0] = dynamic_cast<ENGINE::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_STATIC, L"Texture_Bang");
	m_pTexture[1] = dynamic_cast<ENGINE::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_STATIC, L"Texture_Miss");
	m_pTexture[2] = dynamic_cast<ENGINE::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	//¿ë

	pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_STATIC, L"Texture_DragonBook");
	m_pTexture[3] = dynamic_cast<ENGINE::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	//ÇØ°ñ
	pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_STATIC, L"Texture_SkullBook");
	m_pTexture[4] = dynamic_cast<ENGINE::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	return S_OK;
}
void CTarget::Update(void)
{
	D3DXVec3TransformNormal(&m_pInfo->m_vDir, &g_vLook, &m_pInfo->m_matWorld);
	D3DXVec3Normalize(&m_pInfo->m_vDir, &m_pInfo->m_vDir);

	ENGINE::CGameObject::Update();

	if(GetAsyncKeyState('5'))
		m_bStart = true;

	if(GetAsyncKeyState('6'))
	{
		m_bStart = false;
		m_fAngle = 2.f;
		m_bup = true;
		m_bDown = false;
		m_bStay = false;
		m_fStay = 0.f;
	}

	if(m_bStart)
		RotateX();

	if(!m_bLife)
	{
		m_fAngle = 0.f;
		m_fDeadTime +=  ENGINE::CTimeMgr::GetInstance()->GetTime();
		if(m_fDeadTime > 1.f)
			m_bisDead = true;
	}

	SetTransForm();

}

void CTarget::Render(void)
{
	//m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00000088);
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pResourceMgr->SetVtxInfo(ENGINE::RESOURCE_STATIC, L"Buffer_RcTarget", m_pConvertVertex);

	if(m_bLife)
	{	
		if(m_pInfo->m_vPos.z <= 30.f)
			m_pTexture[0]->Render(0);

		if(m_pInfo->m_vPos.z > 30.f && m_pInfo->m_vPos.z <= 40.f)
			m_pTexture[3]->Render(0);

		if(m_pInfo->m_vPos.z > 40.f && m_pInfo->m_vPos.z <= 50.f)
			m_pTexture[4]->Render(0);
	}

	if(!m_bLife && m_bMiss == false)
		m_pTexture[1]->Render(0);

	if(!m_bLife && m_bMiss == true)
		m_pTexture[2]->Render(0);

	m_pBuffer->Render();

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

void CTarget::SetPos(const D3DXVECTOR3& vPos)
{
	m_pInfo->m_vPos = vPos;
}

ENGINE::VTXTEX* CTarget::GetpVertex(void)
{
	return m_pVertexTemp;
}
void CTarget::Release(void)
{
	m_pInfoSubject->UnSubscribe(m_pCameraObserver);
	ENGINE::Safe_Delete(m_pCameraObserver);

	if(m_pVertex)
		ENGINE::Safe_Delete_Array(m_pVertex);

	if(m_pConvertVertex)
		ENGINE::Safe_Delete_Array(m_pConvertVertex);

	if(m_pVertexTemp)
		ENGINE::Safe_Delete_Array(m_pVertexTemp);
}
void CTarget::SetTransForm(void)
{
	const D3DXMATRIX*		pMatView = m_pCameraObserver->GetView();
	const D3DXMATRIX*		pMatProj = m_pCameraObserver->GetProj();

	for(size_t i = 0; i < m_dwVtxCnt; ++i)
	{
		m_pConvertVertex[i] = m_pVertexTemp[i];

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

void CTarget::RotateX(void)
{
	D3DXVECTOR3 vPos; 
	vPos = m_pVertexTemp[2].vPos - m_pVertexTemp[3].vPos;	

	D3DXMATRIX matTrans;

	D3DXMatrixIdentity(&matTrans);

	D3DXMatrixIdentity(&matRot);

	D3DXMatrixRotationAxis(&matRot,&vPos,m_fAngle);

	matTrans.m[3][1] = 1.f;

	m_pInfo->m_matWorld = matTrans * matRot * m_pInfo->m_matWorld;

	if(m_bLife)
	{
		if(m_bup)
		{
			m_fAngle -= ENGINE::CTimeMgr::GetInstance()->GetTime();
		}
		if(m_bStay)
		{
			m_fStay += ENGINE::CTimeMgr::GetInstance()->GetTime();
		}
		if(m_bDown)
		{
			m_fAngle += ENGINE::CTimeMgr::GetInstance()->GetTime();
		}
	}	

	if(m_fAngle < 0 && m_bDown == false)
	{
		m_bup = false;
		m_bStay = true;
	}
	if(m_fStay >= 1)
	{
		m_fStay = 0.f;
		m_fAngle = 0.f;
		m_bDown = true;
		m_bStay = false;
	}
	if(m_fAngle > 2)
	{
		m_bLife = false;
		m_bMiss = true;
	}			

}


