#include "stdafx.h"
#include "StaticCamera.h"
#include "Export_Function.h"
#include "Include.h"
#include "Transform.h" 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CStaticCamera::CStaticCamera(LPDIRECT3DDEVICE9 pDevice)
: ENGINE::CCamera(pDevice)
, m_pTimeMgr(ENGINE::Get_Time())
, m_pInfoSubject(ENGINE::Get_InfoSubject())
, m_pTargetInfo(NULL)
, m_fAngle(0.f)
, m_fAngle2(0.f)
, m_fCameraSpeed(0.f)
, m_fTargetDistance(0.f)
{
	
}

CStaticCamera::~CStaticCamera()
{

}

CStaticCamera* CStaticCamera::Create(LPDIRECT3DDEVICE9 pDevice, 
									 ENGINE::CTransform* pTargetInfo)
{
		CStaticCamera*		pCamera = new CStaticCamera(pDevice);
		if(FAILED(pCamera->Initialize()))
			ENGINE::Safe_Delete(pCamera);

		pCamera->SetCameraTarget(pTargetInfo);
		pCamera->SetProjectionMatrix(D3DXToRadian(45.f), float(WINCX) / WINCY, 1.f, 1000.f);
		
		return pCamera;
}

void CStaticCamera::SetCameraTarget(ENGINE::CTransform* pTargetInfo)
{
	m_pTargetInfo = pTargetInfo;
}

HRESULT CStaticCamera::Initialize(void)
{
	m_fCameraSpeed = 10.f;
	m_fTargetDistance = 20.f;
	m_fAngle = D3DXToRadian(30.f);

	m_pInfoSubject->AddData(D3DTS_VIEW, &m_matView);
	m_pInfoSubject->AddData(D3DTS_PROJECTION, &m_matProj);

	//m_pTimeMgr->InitTime();

	return S_OK;
}

void CStaticCamera::Update(void)
{
	KeyCheck();
	TargetRenewal();

	m_pInfoSubject->Notify(D3DTS_VIEW);
	m_pInfoSubject->Notify(D3DTS_PROJECTION);

}

void CStaticCamera::KeyCheck(void)
{
	//m_pTimeMgr->InitTime();
	float fTime = m_pTimeMgr->GetTime();
	fTime = 0.005f;

	if(GetAsyncKeyState('O'))
	{
		m_fTargetDistance += 10.f * fTime;
	}

	if(GetAsyncKeyState('P'))
	{
		m_fTargetDistance -= 10.f * fTime;
	}

	if(GetAsyncKeyState('1'))
	{
		m_fAngle += D3DXToRadian(40.f) * fTime;
	}
	if(GetAsyncKeyState('2'))
	{
		m_fAngle -= D3DXToRadian(40.f) * fTime;
	}
	if(GetAsyncKeyState(VK_LEFT))
	{
		m_fAngle2 -= D3DXToRadian(40.f) * fTime;
	}
	if(GetAsyncKeyState(VK_RIGHT))
	{
		m_fAngle2 += D3DXToRadian(40.f) * fTime;
	}
	if(GetAsyncKeyState(VK_UP))
	{
		D3DXVECTOR3 vTemp = m_vAt - m_vEye;
		vTemp.y = 0.f;
		m_pTargetInfo->m_vPos += vTemp * fTime;
	}
	if(GetAsyncKeyState(VK_DOWN))
	{
		D3DXVECTOR3 vTemp = m_vAt - m_vEye;
		vTemp.y = 0.f;
		m_pTargetInfo->m_vPos -= vTemp * fTime;
	}
}

void CStaticCamera::TargetRenewal(void)
{
	m_vEye = m_pTargetInfo->m_vDir * (-1);
	D3DXVec3Normalize(&m_vEye, &m_vEye);

	m_vEye *= m_fTargetDistance;

	D3DXVECTOR3		vRight;
	memcpy(&vRight, &m_pTargetInfo->m_matWorld.m[0][0], sizeof(float) * 3);

	D3DXMATRIX		matRotAxis;

	D3DXMatrixRotationAxis(&matRotAxis, &vRight, m_fAngle);
	D3DXVec3TransformNormal(&m_vEye, &m_vEye, &matRotAxis);

	D3DXVECTOR3		vUp;
	memcpy(&vUp, &m_pTargetInfo->m_matWorld.m[1][0], sizeof(float) * 3);

	D3DXMatrixRotationAxis(&matRotAxis, &vUp, m_fAngle2);
	D3DXVec3TransformNormal(&m_vEye, &m_vEye, &matRotAxis);


	m_vEye += m_pTargetInfo->m_vPos;
	
	m_vAt = m_pTargetInfo->m_vPos;

	D3DXMATRIX	matView;
	
	CCamera::SetViewSpaceMatrix(&m_vEye, &m_vAt, &D3DXVECTOR3(0.f, 1.f, 0.f));
}

