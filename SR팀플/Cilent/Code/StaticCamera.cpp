#include "stdafx.h"
#include "StaticCamera.h"
#include "Export_Function.h"
#include "Include.h"
#include "Transform.h" 

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
	Release();
}

CStaticCamera* CStaticCamera::Create(LPDIRECT3DDEVICE9 pDevice, 
									 const ENGINE::CTransform* pTargetInfo)
{
	CStaticCamera*		pCamera = new CStaticCamera(pDevice);
	if(FAILED(pCamera->Initialize()))
		ENGINE::Safe_Delete(pCamera);

	pCamera->SetCameraTarget(pTargetInfo);
	pCamera->SetProjectionMatrix(D3DXToRadian(50.f), float(WINCX) / WINCY, 1.f, 1000.f);
	
	return pCamera;
}

void CStaticCamera::SetCameraTarget(const ENGINE::CTransform* pTargetInfo)
{
	m_pTargetInfo = pTargetInfo;
}

HRESULT CStaticCamera::Initialize(void)
{
	m_fCameraSpeed = 20.f;
	m_fTargetDistance = 1.f;
	m_fAngle = D3DXToRadian(30.f);
	m_fAngle2 = D3DXToRadian(0.f);

	m_pInfoSubject->AddData(D3DTS_VIEW, &m_matView);
	m_pInfoSubject->AddData(D3DTS_PROJECTION, &m_matProj);

	m_dwTime = GetTickCount();
	m_iOriginY = WINCY / 2;

	ShowCursor(FALSE);

	m_Height = 1.f;
	m_bMaxHeight = true;
	m_bMinHeight = false;

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
	float fTime = m_pTimeMgr->GetTime();

	POINT		ptMouse;

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	ptMouse.x;
	ptMouse.y;
	//if(ptMouse.x <= WINCX - 800)
	//{
	//	m_fAngle2 -= D3DXToRadian(40.f) * fTime;
	//}

	//if(ptMouse.x >= WINCX)
	//{
	//	m_fAngle2 += D3DXToRadian(40.f) * fTime;
	//}
	if(m_dwTime + 50 < GetTickCount())
	{
		m_dwTime = GetTickCount();
		m_iOriginY = ptMouse.y;
	}
	if(ptMouse.y < m_iOriginY)
	{
		m_fAngle -= D3DXToRadian(50.f) * fTime;
	}
	if(ptMouse.y > m_iOriginY)
	{
		m_fAngle += D3DXToRadian(50.f) * fTime;
	}
	/*if(ptMouse.y >= WINCY || ptMouse.y <= 0)
	{*/
		POINT pt = { 0, 0};
		ClientToScreen(g_hWnd, &pt);
		SetCursorPos(pt.x + ptMouse.x, pt.y + WINCY / 2);
		m_iOriginY = WINCY / 2;
	//}
	//if(ptMouse.x <= WINCX - 800)
	//{
	//	m_fAngle -= D3DXToRadian(40.f) * fTime;
	//}

	//if(ptMouse.x >= WINCX)
	//{
	//	m_fAngle += D3DXToRadian(40.f) * fTime;
	//}
}

void CStaticCamera::TargetRenewal(void)
{
	if(m_bMaxHeight == true && m_bMinHeight == false)
	{
		m_Height -= ENGINE::CTimeMgr::GetInstance()->GetTime();
		if(m_Height <= 0.f)
		{
			m_bMaxHeight = false;
			m_bMinHeight = true;
		}
	}
	if(m_bMaxHeight == false && m_bMinHeight == true)
	{
		m_Height += ENGINE::CTimeMgr::GetInstance()->GetTime();
		if(m_Height >= 0.2f)
		{
			m_bMaxHeight = true;
			m_bMinHeight = false;
		}
	}

	m_vEye = m_pTargetInfo->m_vDir * (-1);
	D3DXVec3Normalize(&m_vEye, &m_vEye);

	m_vEye *= m_fTargetDistance;

	D3DXVECTOR3		vRight;
	memcpy(&vRight, &m_pTargetInfo->m_matWorld.m[0][0], sizeof(float) * 3);

	D3DXVECTOR3		vUp;
	memcpy(&vUp, &m_pTargetInfo->m_matWorld.m[1][0], sizeof(float) * 3);

	D3DXMATRIX		matRotAxis;

	D3DXMatrixRotationAxis(&matRotAxis, &vRight, m_fAngle);
	D3DXVec3TransformNormal(&m_vEye, &m_vEye, &matRotAxis);
	D3DXMatrixRotationAxis(&matRotAxis, &vUp, m_fAngle2);
	D3DXVec3TransformNormal(&m_vEye, &m_vEye, &matRotAxis);
	
	D3DXVECTOR3		vHeight;
	if(GetAsyncKeyState('W') || GetAsyncKeyState('A')
		|| GetAsyncKeyState('S') || GetAsyncKeyState('D'))
	{
		vHeight = D3DXVECTOR3(0.f, m_Height, 0.f);
	}
	else
	{
		vHeight = D3DXVECTOR3(0.f, 0.f, 0.f);
	}
	
	m_vEye += m_pTargetInfo->m_vPos + vHeight + D3DXVECTOR3(0.f, 1.f, 0.f);
	m_vAt = m_pTargetInfo->m_vPos + vHeight + D3DXVECTOR3(0.f, 1.f, 0.f);


	D3DXMATRIX	matView;
	
	CCamera::SetViewSpaceMatrix(&m_vEye, &m_vAt, &D3DXVECTOR3(0.f, 1.f, 0.f));
}

void CStaticCamera::Release(void)
{
	m_pInfoSubject->RemoveData(D3DTS_VIEW, &m_matView);
	m_pInfoSubject->RemoveData(D3DTS_PROJECTION, &m_matProj);
}

