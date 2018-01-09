// CreateCube.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool58.h"
#include "CreateCube.h"
#include "GraphicDev.h"
#include "Back.h"

#include "MainFrm.h"
#include "MyForm.h"
#include "Tool58View.h"
#include "Transform.h"
#include "..\Engine\Utility\Code\GameObject.h"

#include "SpinObstacle.h"
#include "CubeObj.h"
#include "UpDownCube.h"
#include "CentryGun.h"
#include "SpinCutter.h"
#include "CrashMaze.h"
#include "ShrinkingPlatform.h"
#include "LeftToRight.h"
#include "RightToLeft.h"
#include "SpinSnow.h"
#include "CubeStair.h"


// CCreateCube 대화 상자입니다.

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CCreateCube, CDialog)

CCreateCube::CCreateCube(CWnd* pParent /*=NULL*/)
	: CDialog(CCreateCube::IDD, pParent)
	, m_PosX(_T("0.000"))
	, m_PosY(_T("0.000"))
	, m_PosZ(_T("0.000"))
	, m_bStart(true)
	, m_ScaleX(_T("1.000"))
	, m_ScaleY(_T("1.000"))
	, m_ScaleZ(_T("1.000"))
	, m_RotX(_T("0.000"))
	, m_RotY(_T("0.000"))
	, m_RotZ(_T("0.000"))
	, m_fScaleVal(0.5f)
	, m_fRotVal(10.f)
	, m_fPosVal(1.f)
{

}

CCreateCube::~CCreateCube()
{
}

void CCreateCube::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_PosX);
	DDX_Text(pDX, IDC_EDIT2, m_PosY);
	DDX_Text(pDX, IDC_EDIT3, m_PosZ);

	DDX_Control(pDX, IDC_RADIO4, m_Radio[0]);
	DDX_Control(pDX, IDC_RADIO5, m_Radio[1]);

	DDX_Control(pDX, IDC_RADIO6, m_RadioPoint[0]);
	DDX_Control(pDX, IDC_RADIO7, m_RadioPoint[1]);

	DDX_Control(pDX, IDC_RADIO8, m_CubeType[0]);
	DDX_Control(pDX, IDC_RADIO9, m_CubeType[1]);
	DDX_Control(pDX, IDC_RADIO10, m_CubeType[2]);
	DDX_Control(pDX, IDC_RADIO11, m_CubeType[3]);
	DDX_Control(pDX, IDC_RADIO12, m_CubeType[4]);
	DDX_Control(pDX, IDC_RADIO13, m_CubeType[5]);
	DDX_Control(pDX, IDC_RADIO14, m_CubeType[6]);
	DDX_Control(pDX, IDC_RADIO15, m_CubeType[7]);
	DDX_Control(pDX, IDC_RADIO16, m_CubeType[8]);
	DDX_Control(pDX, IDC_RADIO17, m_CubeType[9]);
	DDX_Control(pDX, IDC_RADIO18, m_CubeType[10]);
	
	


	if(m_bStart)
	{
		m_Radio->SetCheck(1);
		m_RadioPoint->SetCheck(1);
		m_CubeType[0].SetCheck(1);

		((CMainFrame*)AfxGetMainWnd())->m_pMainView->m_bCreateCube = true;
		m_pBack = (CBack*)(((CMainFrame*)AfxGetMainWnd())->m_pMainView->m_pBack);

		m_bStart = false;
	}

	/*((CMainFrame*)AfxGetMainWnd())->Invalidate(FALSE);
	((CMainFrame*)AfxGetMainWnd())->m_pMainView->Invalidate(TRUE);
	Invalidate(FALSE);*/

	DDX_Text(pDX, IDC_EDIT9, m_ScaleX);
	DDX_Text(pDX, IDC_EDIT10, m_ScaleY);
	DDX_Text(pDX, IDC_EDIT11, m_ScaleZ);
	DDX_Text(pDX, IDC_EDIT4, m_RotX);
	DDX_Text(pDX, IDC_EDIT5, m_RotY);
	DDX_Text(pDX, IDC_EDIT8, m_RotZ);
	
}


BEGIN_MESSAGE_MAP(CCreateCube, CDialog)
	
	ON_BN_CLICKED(IDC_BUTTON1, &CCreateCube::OnAddCube)
	ON_EN_CHANGE(IDC_EDIT9, &CCreateCube::OnScaleX)
	ON_EN_CHANGE(IDC_EDIT1, &CCreateCube::OnPosX)
	ON_BN_CLICKED(IDC_RADIO5, &CCreateCube::OnDel)
	ON_BN_CLICKED(IDC_RADIO4, &CCreateCube::OnSel)
	ON_EN_CHANGE(IDC_EDIT2, &CCreateCube::OnPosY)
	ON_EN_CHANGE(IDC_EDIT3, &CCreateCube::OnPosZ)
	ON_EN_CHANGE(IDC_EDIT10, &CCreateCube::OnScaleY)
	ON_EN_CHANGE(IDC_EDIT11, &CCreateCube::OnScaleZ)
	ON_EN_CHANGE(IDC_EDIT4, &CCreateCube::OnRotX)
	ON_EN_CHANGE(IDC_EDIT5, &CCreateCube::OnRotY)
	ON_EN_CHANGE(IDC_EDIT8, &CCreateCube::OnRotZ)

	ON_BN_CLICKED(IDC_BUTTON4, &CCreateCube::OnScaleXAdd)
	ON_BN_CLICKED(IDC_BUTTON11, &CCreateCube::OnScaleXDe)
	ON_BN_CLICKED(IDC_BUTTON10, &CCreateCube::OnScaleXEu)
	ON_BN_CLICKED(IDC_BUTTON12, &CCreateCube::OnScaleYIn)
	ON_BN_CLICKED(IDC_BUTTON13, &CCreateCube::OnScaleYDe)
	ON_BN_CLICKED(IDC_BUTTON28, &CCreateCube::OnScaleYEu)
	ON_BN_CLICKED(IDC_BUTTON14, &CCreateCube::OnScaleZIn)
	ON_BN_CLICKED(IDC_BUTTON15, &CCreateCube::OnScaleZDe)
	ON_BN_CLICKED(IDC_BUTTON29, &CCreateCube::OnScaleZEu)
	ON_BN_CLICKED(IDC_BUTTON16, &CCreateCube::OnRotXIn)
	ON_BN_CLICKED(IDC_BUTTON17, &CCreateCube::OnRotXDe)
	ON_BN_CLICKED(IDC_BUTTON30, &CCreateCube::OnRotXEu)
	ON_BN_CLICKED(IDC_BUTTON18, &CCreateCube::OnRotYIn)
	ON_BN_CLICKED(IDC_BUTTON19, &CCreateCube::OnRotYDe)
	ON_BN_CLICKED(IDC_BUTTON31, &CCreateCube::OnRotYEu)
	ON_BN_CLICKED(IDC_BUTTON20, &CCreateCube::OnRotZIn)
	ON_BN_CLICKED(IDC_BUTTON21, &CCreateCube::OnRotZDe)
	ON_BN_CLICKED(IDC_BUTTON32, &CCreateCube::OnRotZEu)
	ON_BN_CLICKED(IDC_BUTTON22, &CCreateCube::OnPosXIn)
	ON_BN_CLICKED(IDC_BUTTON23, &CCreateCube::OnPosXDe)
	ON_BN_CLICKED(IDC_BUTTON33, &CCreateCube::OnPosXEu)
	ON_BN_CLICKED(IDC_BUTTON24, &CCreateCube::OnPosYIn)
	ON_BN_CLICKED(IDC_BUTTON25, &CCreateCube::OnPosYDe)
	ON_BN_CLICKED(IDC_BUTTON34, &CCreateCube::OnPosYEu)
	ON_BN_CLICKED(IDC_BUTTON26, &CCreateCube::OnPosZIn)
	ON_BN_CLICKED(IDC_BUTTON27, &CCreateCube::OnPosZDe)
	ON_BN_CLICKED(IDC_BUTTON35, &CCreateCube::OnPosZEu)
	ON_BN_CLICKED(IDC_BUTTON37, &CCreateCube::OnSelectCube)
END_MESSAGE_MAP()


// CCreateCube 메시지 처리기입니다.
void CCreateCube::OnAddCube()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CBack::MakeACube();
	UpdateData(TRUE);

	ENGINE::CGameObject* m_pGameObj = NULL;

	float fPosX = (float)_wtof(m_PosX);
	float fPosY = (float)_wtof(m_PosY);
	float fPosZ = (float)_wtof(m_PosZ);

	float fScaleX = (float)_wtof(m_ScaleX);
	float fScaleY = (float)_wtof(m_ScaleY);
	float fScaleZ = (float)_wtof(m_ScaleZ);

	float fRotX = (float)_wtof(m_RotX);
	float fRotY = (float)_wtof(m_RotY);
	float fRotZ = (float)_wtof(m_RotZ);

	const D3DXVECTOR3 vPos(fPosX, fPosY, fPosZ);

	if(m_CubeType[0].GetCheck())
	{
		m_pGameObj = CCubeObj::Create(ENGINE::CGraphicDev::GetInstance()->GetDevice());
		CCubeObj* pCubeObj = (CCubeObj*)m_pGameObj;

		pCubeObj->SetPos(vPos);
		pCubeObj->GetInfo()->m_vScale = D3DXVECTOR3(fScaleX, fScaleY, fScaleZ);
		pCubeObj->GetInfo()->m_fAngle[ENGINE::ANGLE_X] = fRotX;
		pCubeObj->GetInfo()->m_fAngle[ENGINE::ANGLE_Y] = fRotY;
		pCubeObj->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = fRotZ;

		CBack::m_vecCube.push_back(pCubeObj);
	}
	else if(m_CubeType[1].GetCheck())
	{
		m_pGameObj = CSpinObstacle::Create(ENGINE::CGraphicDev::GetInstance()->GetDevice());

		CSpinObstacle* pSpinObstacle = (CSpinObstacle*)m_pGameObj;

		pSpinObstacle->SetPos(vPos);
		pSpinObstacle->GetInfo()->m_vScale = D3DXVECTOR3(fScaleX, fScaleY, fScaleZ);
		pSpinObstacle->GetInfo()->m_fAngle[ENGINE::ANGLE_X] = fRotX;
		pSpinObstacle->GetInfo()->m_fAngle[ENGINE::ANGLE_Y] = fRotY;
		pSpinObstacle->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = fRotZ;

		CBack::m_vecCube.push_back(pSpinObstacle);
	}
	else if(m_CubeType[2].GetCheck())
	{
		m_pGameObj = CUpDownCube::Create(ENGINE::CGraphicDev::GetInstance()->GetDevice());
		
		CUpDownCube* pUpDownCube = (CUpDownCube*)m_pGameObj;

		pUpDownCube->SetPos(vPos);
		pUpDownCube->GetInfo()->m_vScale = D3DXVECTOR3(fScaleX, fScaleY, fScaleZ);
		pUpDownCube->GetInfo()->m_fAngle[ENGINE::ANGLE_X] = fRotX;
		pUpDownCube->GetInfo()->m_fAngle[ENGINE::ANGLE_Y] = fRotY;
		pUpDownCube->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = fRotZ;

		CBack::m_vecCube.push_back(pUpDownCube);
	}
	else if(m_CubeType[3].GetCheck())
	{
		m_pGameObj = CCentryGun::Create(ENGINE::CGraphicDev::GetInstance()->GetDevice());

		CCentryGun* pGun = (CCentryGun*)m_pGameObj;

		pGun->SetPos(vPos);
		pGun->GetInfo()->m_vScale = D3DXVECTOR3(fScaleX, fScaleY, fScaleZ);
		pGun->GetInfo()->m_fAngle[ENGINE::ANGLE_X] = fRotX;
		pGun->GetInfo()->m_fAngle[ENGINE::ANGLE_Y] = fRotY;
		pGun->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = fRotZ;

		CBack::m_vecCube.push_back(pGun);
	}
	else if(m_CubeType[4].GetCheck())
	{
		m_pGameObj = CSpinCutter::Create(ENGINE::CGraphicDev::GetInstance()->GetDevice());

		CSpinCutter* pSpinCutter = (CSpinCutter*)m_pGameObj;

		pSpinCutter->SetPos(vPos);
		pSpinCutter->GetInfo()->m_vScale = D3DXVECTOR3(fScaleX, fScaleY, fScaleZ);
		pSpinCutter->GetInfo()->m_fAngle[ENGINE::ANGLE_X] = fRotX;
		pSpinCutter->GetInfo()->m_fAngle[ENGINE::ANGLE_Y] = fRotY;
		pSpinCutter->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = fRotZ;

		CBack::m_vecCube.push_back(pSpinCutter);
	}
	else if(m_CubeType[5].GetCheck())
	{
		m_pGameObj = CCrashMaze::Create(ENGINE::CGraphicDev::GetInstance()->GetDevice());

		CCrashMaze* pCrashMaze = (CCrashMaze*)m_pGameObj;

		pCrashMaze->SetPos(vPos);
		pCrashMaze->GetInfo()->m_vScale = D3DXVECTOR3(fScaleX, fScaleY, fScaleZ);
		pCrashMaze->GetInfo()->m_fAngle[ENGINE::ANGLE_X] = fRotX;
		pCrashMaze->GetInfo()->m_fAngle[ENGINE::ANGLE_Y] = fRotY;
		pCrashMaze->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = fRotZ;

		CBack::m_vecCube.push_back(pCrashMaze);

	}
	else if(m_CubeType[6].GetCheck())
	{
		m_pGameObj = CShrinkingPlatform::Create(ENGINE::CGraphicDev::GetInstance()->GetDevice());

		CShrinkingPlatform* pShrinking = (CShrinkingPlatform*)m_pGameObj;

		pShrinking->SetPos(vPos);
		pShrinking->GetInfo()->m_vScale = D3DXVECTOR3(fScaleX, fScaleY, fScaleZ);
		pShrinking->GetInfo()->m_fAngle[ENGINE::ANGLE_X] = fRotX;
		pShrinking->GetInfo()->m_fAngle[ENGINE::ANGLE_Y] = fRotY;
		pShrinking->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = fRotZ;

		CBack::m_vecCube.push_back(pShrinking);
	}
	else if(m_CubeType[7].GetCheck())
	{
		m_pGameObj = CLeftToRight::Create(ENGINE::CGraphicDev::GetInstance()->GetDevice());

		CLeftToRight* pLeftToRight = (CLeftToRight*)m_pGameObj;

		pLeftToRight->SetPos(vPos);
		pLeftToRight->GetInfo()->m_vScale = D3DXVECTOR3(fScaleX, fScaleY, fScaleZ);
		pLeftToRight->GetInfo()->m_fAngle[ENGINE::ANGLE_X] = fRotX;
		pLeftToRight->GetInfo()->m_fAngle[ENGINE::ANGLE_Y] = fRotY;
		pLeftToRight->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = fRotZ;

		CBack::m_vecCube.push_back(pLeftToRight);
	}
	else if(m_CubeType[8].GetCheck())
	{
		m_pGameObj = CRightToLeft::Create(ENGINE::CGraphicDev::GetInstance()->GetDevice());

		CRightToLeft* pRightToLeft = (CRightToLeft*)m_pGameObj;

		pRightToLeft->SetPos(vPos);
		pRightToLeft->GetInfo()->m_vScale = D3DXVECTOR3(fScaleX, fScaleY, fScaleZ);
		pRightToLeft->GetInfo()->m_fAngle[ENGINE::ANGLE_X] = fRotX;
		pRightToLeft->GetInfo()->m_fAngle[ENGINE::ANGLE_Y] = fRotY;
		pRightToLeft->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = fRotZ;

		CBack::m_vecCube.push_back(pRightToLeft);
	}
	else if(m_CubeType[9].GetCheck())
	{
		m_pGameObj = CSpinSnow::Create(ENGINE::CGraphicDev::GetInstance()->GetDevice());

		CSpinSnow* pSpinSnow = (CSpinSnow*)m_pGameObj;

		pSpinSnow->SetPos(vPos);
		pSpinSnow->GetInfo()->m_vScale = D3DXVECTOR3(fScaleX, fScaleY, fScaleZ);
		pSpinSnow->GetInfo()->m_fAngle[ENGINE::ANGLE_X] = fRotX;
		pSpinSnow->GetInfo()->m_fAngle[ENGINE::ANGLE_Y] = fRotY;
		pSpinSnow->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = fRotZ;

		CBack::m_vecCube.push_back(pSpinSnow);
	}
	else if(m_CubeType[10].GetCheck())
	{
		m_pGameObj = CCubeStair::Create(ENGINE::CGraphicDev::GetInstance()->GetDevice());

		CCubeStair* pCubeStair = (CCubeStair*)m_pGameObj;

		pCubeStair->SetPos(vPos);
		pCubeStair->GetInfo()->m_vScale = D3DXVECTOR3(fScaleX, fScaleY, fScaleZ);
		pCubeStair->GetInfo()->m_fAngle[ENGINE::ANGLE_X] = fRotX;
		pCubeStair->GetInfo()->m_fAngle[ENGINE::ANGLE_Y] = fRotY;
		pCubeStair->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = fRotZ;

		CBack::m_vecCube.push_back(pCubeStair);
	}


	
	m_pBack->Render();

	

	UpdateData(FALSE);

	((CMainFrame*)AfxGetMainWnd())->m_pMainView->Invalidate(FALSE);
	//((CMainFrame*)AfxGetMainWnd())->Invalidate(FALSE);
	//((CMainFrame*)AfxGetMainWnd())->m_pMainView->Invalidate(FALSE);
	
	//((CMainFrame*)AfxGetMainWnd())->m_pMainView->m_bInvalidate = true;
}


void CCreateCube::OnDel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	m_pBack->m_bSelRadio = false;

	m_pBack->m_pCubeForDel = NULL;
}

void CCreateCube::OnSel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pBack->m_bSelRadio = true;
}
void CCreateCube::OnPosX()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if(m_Radio[0].GetCheck() && m_pBack->m_pCubeForDel != NULL)
	{
		UpdateData(TRUE);

		float X = (float)_wtof(m_PosX);

		ENGINE::CTransform* pTransform = m_pBack->m_pCubeForDel->GetInfo();

		if(m_RadioPoint[0].GetCheck())
			pTransform->m_vPos.x = X;
		else
			pTransform->m_vPos.x += X;

		UpdateData(FALSE);

		((CMainFrame*)AfxGetMainWnd())->m_pMainView->Invalidate(FALSE);
	}
}

void CCreateCube::OnPosY()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if(m_Radio[0].GetCheck() && m_pBack->m_pCubeForDel != NULL)
	{
		UpdateData(TRUE);

		float Y = (float)_wtof(m_PosY);

		ENGINE::CTransform* pTransform = m_pBack->m_pCubeForDel->GetInfo();

		if(m_RadioPoint[0].GetCheck())
			pTransform->m_vPos.y = Y;
		else
			pTransform->m_vPos.y += Y;

		UpdateData(FALSE);

		((CMainFrame*)AfxGetMainWnd())->m_pMainView->Invalidate(FALSE);
	}
}

void CCreateCube::OnPosZ()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if(m_Radio[0].GetCheck() && m_pBack->m_pCubeForDel != NULL)
	{
		UpdateData(TRUE);

		float Z = (float)_wtof(m_PosZ);

		ENGINE::CTransform* pTransform = m_pBack->m_pCubeForDel->GetInfo();

		if(m_RadioPoint[0].GetCheck())
			pTransform->m_vPos.z = Z;
		else
			pTransform->m_vPos.z += Z;

		UpdateData(FALSE);

		((CMainFrame*)AfxGetMainWnd())->m_pMainView->Invalidate(FALSE);
	}
}

void CCreateCube::OnScaleX()
{

	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if(m_Radio[0].GetCheck() && m_pBack->m_pCubeForDel != NULL)
	{
		UpdateData(TRUE);

		float X = (float)_wtof(m_ScaleX);

		ENGINE::CTransform* pTransform = m_pBack->m_pCubeForDel->GetInfo();

		if(m_RadioPoint[0].GetCheck())
			pTransform->m_vScale.x = X;
		else
			pTransform->m_vScale.x += X;

		UpdateData(FALSE);

		((CMainFrame*)AfxGetMainWnd())->m_pMainView->Invalidate(FALSE);
	}
}

void CCreateCube::OnScaleY()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if(m_Radio[0].GetCheck() && m_pBack->m_pCubeForDel != NULL)
	{
		UpdateData(TRUE);

		float Y = (float)_wtof(m_ScaleY);

		ENGINE::CTransform* pTransform = m_pBack->m_pCubeForDel->GetInfo();

		if(m_RadioPoint[0].GetCheck())
			pTransform->m_vScale.y = Y;
		else
			pTransform->m_vScale.y += Y;

		UpdateData(FALSE);

		((CMainFrame*)AfxGetMainWnd())->m_pMainView->Invalidate(FALSE);
	}
}

void CCreateCube::OnScaleZ()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if(m_Radio[0].GetCheck() && m_pBack->m_pCubeForDel != NULL)
	{
		UpdateData(TRUE);

		float Z = (float)_wtof(m_ScaleZ);

		ENGINE::CTransform* pTransform = m_pBack->m_pCubeForDel->GetInfo();

		if(m_RadioPoint[0].GetCheck())
			pTransform->m_vScale.z = Z;
		else
			pTransform->m_vScale.z += Z;

		UpdateData(FALSE);

		((CMainFrame*)AfxGetMainWnd())->m_pMainView->Invalidate(FALSE);
	}
}

void CCreateCube::OnRotX()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if(m_Radio[0].GetCheck() && m_pBack->m_pCubeForDel != NULL)
	{
		UpdateData(TRUE);

		float X = (float)_wtof(m_RotX);

		ENGINE::CTransform* pTransform = m_pBack->m_pCubeForDel->GetInfo();

		if(m_RadioPoint[0].GetCheck())
			pTransform->m_fAngle[ENGINE::ANGLE_X] = D3DXToRadian(X);
		else
			pTransform->m_fAngle[ENGINE::ANGLE_X] += D3DXToRadian(X);

		UpdateData(FALSE);

		((CMainFrame*)AfxGetMainWnd())->m_pMainView->Invalidate(FALSE);
	}
}

void CCreateCube::OnRotY()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if(m_Radio[0].GetCheck() && m_pBack->m_pCubeForDel != NULL)
	{
		UpdateData(TRUE);

		float Y = (float)_wtof(m_RotY);

		ENGINE::CTransform* pTransform = m_pBack->m_pCubeForDel->GetInfo();

		if(m_RadioPoint[0].GetCheck())
			pTransform->m_fAngle[ENGINE::ANGLE_Y] = D3DXToRadian(Y);
		else
			pTransform->m_fAngle[ENGINE::ANGLE_Y] += D3DXToRadian(Y);

		UpdateData(FALSE);

		((CMainFrame*)AfxGetMainWnd())->m_pMainView->Invalidate(FALSE);
	}
}

void CCreateCube::OnRotZ()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if(m_Radio[0].GetCheck() && m_pBack->m_pCubeForDel != NULL)
	{
		UpdateData(TRUE);

		float Z = (float)_wtof(m_RotZ);

		ENGINE::CTransform* pTransform = m_pBack->m_pCubeForDel->GetInfo();

		if(m_RadioPoint[0].GetCheck())
			pTransform->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(Z);
		else
			pTransform->m_fAngle[ENGINE::ANGLE_Z] += D3DXToRadian(Z);

		UpdateData(FALSE);

		((CMainFrame*)AfxGetMainWnd())->m_pMainView->Invalidate(FALSE);
	}
}


void CCreateCube::OnScaleXAdd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if(m_Radio[0].GetCheck() && m_pBack->m_pCubeForDel != NULL)
	{
		UpdateData(TRUE);

		float X = (float)_wtof(m_ScaleX);

		X += m_fScaleVal;

		ENGINE::CTransform* pTransform = m_pBack->m_pCubeForDel->GetInfo();

		if(m_RadioPoint[0].GetCheck())
			pTransform->m_vScale.x = X;
		else
			pTransform->m_vScale.x += X;

		m_ScaleX.Format(_T("%.3f"), X);

		UpdateData(FALSE);

		((CMainFrame*)AfxGetMainWnd())->m_pMainView->Invalidate(FALSE);
	}
}

void CCreateCube::OnScaleXDe()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if(m_Radio[0].GetCheck() && m_pBack->m_pCubeForDel != NULL)
	{
		UpdateData(TRUE);

		float X = (float)_wtof(m_ScaleX);

		X -= m_fScaleVal;

		ENGINE::CTransform* pTransform = m_pBack->m_pCubeForDel->GetInfo();

		if(m_RadioPoint[0].GetCheck())
			pTransform->m_vScale.x = X;
		else
			pTransform->m_vScale.x += X;

		m_ScaleX.Format(_T("%.3f"), X);

		UpdateData(FALSE);

		((CMainFrame*)AfxGetMainWnd())->m_pMainView->Invalidate(FALSE);
	}
}

void CCreateCube::OnScaleXEu()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if(m_Radio[0].GetCheck() && m_pBack->m_pCubeForDel != NULL)
	{
		UpdateData(TRUE);

		float X = (float)_wtof(m_ScaleX);

		ENGINE::CTransform* pTransform = m_pBack->m_pCubeForDel->GetInfo();

		if(m_RadioPoint[0].GetCheck())
			pTransform->m_vScale.x = X;
		else
			pTransform->m_vScale.x += X;

		m_ScaleX.Format(_T("%.3f"), X);

		UpdateData(FALSE);

		((CMainFrame*)AfxGetMainWnd())->m_pMainView->Invalidate(FALSE);
	}
}

void CCreateCube::OnScaleYIn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if(m_Radio[0].GetCheck() && m_pBack->m_pCubeForDel != NULL)
	{
		UpdateData(TRUE);

		float Y = (float)_wtof(m_ScaleY);

		Y += m_fScaleVal;

		ENGINE::CTransform* pTransform = m_pBack->m_pCubeForDel->GetInfo();

		if(m_RadioPoint[0].GetCheck())
			pTransform->m_vScale.y = Y;
		else
			pTransform->m_vScale.y += Y;

		m_ScaleY.Format(_T("%.3f"), Y);

		UpdateData(FALSE);

		((CMainFrame*)AfxGetMainWnd())->m_pMainView->Invalidate(FALSE);
	}
}

void CCreateCube::OnScaleYDe()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if(m_Radio[0].GetCheck() && m_pBack->m_pCubeForDel != NULL)
	{
		UpdateData(TRUE);

		float Y = (float)_wtof(m_ScaleY);

		Y -= m_fScaleVal;

		ENGINE::CTransform* pTransform = m_pBack->m_pCubeForDel->GetInfo();

		if(m_RadioPoint[0].GetCheck())
			pTransform->m_vScale.y = Y;
		else
			pTransform->m_vScale.y += Y;

		m_ScaleY.Format(_T("%.3f"), Y);

		UpdateData(FALSE);

		((CMainFrame*)AfxGetMainWnd())->m_pMainView->Invalidate(FALSE);
	}
}

void CCreateCube::OnScaleYEu()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if(m_Radio[0].GetCheck() && m_pBack->m_pCubeForDel != NULL)
	{
		UpdateData(TRUE);

		float Y = (float)_wtof(m_ScaleY);

		ENGINE::CTransform* pTransform = m_pBack->m_pCubeForDel->GetInfo();

		if(m_RadioPoint[0].GetCheck())
			pTransform->m_vScale.y = Y;
		else
			pTransform->m_vScale.y += Y;

		UpdateData(FALSE);

		((CMainFrame*)AfxGetMainWnd())->m_pMainView->Invalidate(FALSE);
	}
}

void CCreateCube::OnScaleZIn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if(m_Radio[0].GetCheck() && m_pBack->m_pCubeForDel != NULL)
	{
		UpdateData(TRUE);

		float Z = (float)_wtof(m_ScaleZ);

		Z += m_fScaleVal;

		ENGINE::CTransform* pTransform = m_pBack->m_pCubeForDel->GetInfo();

		if(m_RadioPoint[0].GetCheck())
			pTransform->m_vScale.z = Z;
		else
			pTransform->m_vScale.z += Z;

		m_ScaleZ.Format(_T("%.3f"), Z);

		UpdateData(FALSE);

		((CMainFrame*)AfxGetMainWnd())->m_pMainView->Invalidate(FALSE);
	}
}

void CCreateCube::OnScaleZDe()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if(m_Radio[0].GetCheck() && m_pBack->m_pCubeForDel != NULL)
	{
		UpdateData(TRUE);

		float Z = (float)_wtof(m_ScaleZ);

		Z -= m_fScaleVal;

		ENGINE::CTransform* pTransform = m_pBack->m_pCubeForDel->GetInfo();

		if(m_RadioPoint[0].GetCheck())
			pTransform->m_vScale.z = Z;
		else
			pTransform->m_vScale.z += Z;

		m_ScaleZ.Format(_T("%.3f"), Z);

		UpdateData(FALSE);

		((CMainFrame*)AfxGetMainWnd())->m_pMainView->Invalidate(FALSE);
	}
}

void CCreateCube::OnScaleZEu()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if(m_Radio[0].GetCheck() && m_pBack->m_pCubeForDel != NULL)
	{
		UpdateData(TRUE);

		float Z = (float)_wtof(m_ScaleZ);

		ENGINE::CTransform* pTransform = m_pBack->m_pCubeForDel->GetInfo();

		if(m_RadioPoint[0].GetCheck())
			pTransform->m_vScale.z = Z;
		else
			pTransform->m_vScale.z += Z;

		UpdateData(FALSE);

		((CMainFrame*)AfxGetMainWnd())->m_pMainView->Invalidate(FALSE);
	}
}

void CCreateCube::OnRotXIn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	
	if(m_Radio[0].GetCheck() && m_pBack->m_pCubeForDel != NULL)
	{
		UpdateData(TRUE);

		float X = (float)_wtof(m_RotX);

		X += m_fRotVal;

		ENGINE::CTransform* pTransform = m_pBack->m_pCubeForDel->GetInfo();

		if(m_RadioPoint[0].GetCheck())
			pTransform->m_fAngle[ENGINE::ANGLE_X] = D3DXToRadian(X);
		else
			pTransform->m_fAngle[ENGINE::ANGLE_X] += D3DXToRadian(X);

		m_RotX.Format(_T("%.3f"), X);

		UpdateData(FALSE);

		((CMainFrame*)AfxGetMainWnd())->m_pMainView->Invalidate(FALSE);
	}
}

void CCreateCube::OnRotXDe()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if(m_Radio[0].GetCheck() && m_pBack->m_pCubeForDel != NULL)
	{
		UpdateData(TRUE);

		float X = (float)_wtof(m_RotX);

		X -= m_fRotVal;

		ENGINE::CTransform* pTransform = m_pBack->m_pCubeForDel->GetInfo();

		if(m_RadioPoint[0].GetCheck())
			pTransform->m_fAngle[ENGINE::ANGLE_X] = D3DXToRadian(X);
		else
			pTransform->m_fAngle[ENGINE::ANGLE_X] += D3DXToRadian(X);

		m_RotX.Format(_T("%.3f"), X);

		UpdateData(FALSE);

		((CMainFrame*)AfxGetMainWnd())->m_pMainView->Invalidate(FALSE);
	}
}

void CCreateCube::OnRotXEu()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if(m_Radio[0].GetCheck() && m_pBack->m_pCubeForDel != NULL)
	{
		UpdateData(TRUE);

		float X = (float)_wtof(m_RotX);

		ENGINE::CTransform* pTransform = m_pBack->m_pCubeForDel->GetInfo();

		if(m_RadioPoint[0].GetCheck())
			pTransform->m_fAngle[ENGINE::ANGLE_X] = D3DXToRadian(X);
		else
			pTransform->m_fAngle[ENGINE::ANGLE_X] += D3DXToRadian(X);

		UpdateData(FALSE);

		((CMainFrame*)AfxGetMainWnd())->m_pMainView->Invalidate(FALSE);
	}
}

void CCreateCube::OnRotYIn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if(m_Radio[0].GetCheck() && m_pBack->m_pCubeForDel != NULL)
	{
		UpdateData(TRUE);

		float Y = (float)_wtof(m_RotY);

		Y += m_fRotVal;

		ENGINE::CTransform* pTransform = m_pBack->m_pCubeForDel->GetInfo();

		if(m_RadioPoint[0].GetCheck())
			pTransform->m_fAngle[ENGINE::ANGLE_Y] = D3DXToRadian(Y);
		else
			pTransform->m_fAngle[ENGINE::ANGLE_Y] += D3DXToRadian(Y);

		m_RotY.Format(_T("%.3f"), Y);

		UpdateData(FALSE);

		((CMainFrame*)AfxGetMainWnd())->m_pMainView->Invalidate(FALSE);
	}
}

void CCreateCube::OnRotYDe()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_Radio[0].GetCheck() && m_pBack->m_pCubeForDel != NULL)
	{
		UpdateData(TRUE);

		float Y = (float)_wtof(m_RotY);

		Y -= m_fRotVal;

		ENGINE::CTransform* pTransform = m_pBack->m_pCubeForDel->GetInfo();

		if(m_RadioPoint[0].GetCheck())
			pTransform->m_fAngle[ENGINE::ANGLE_Y] = D3DXToRadian(Y);
		else
			pTransform->m_fAngle[ENGINE::ANGLE_Y] += D3DXToRadian(Y);

		m_RotY.Format(_T("%.3f"), Y);

		UpdateData(FALSE);

		((CMainFrame*)AfxGetMainWnd())->m_pMainView->Invalidate(FALSE);
	}
}

void CCreateCube::OnRotYEu()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_Radio[0].GetCheck() && m_pBack->m_pCubeForDel != NULL)
	{
		UpdateData(TRUE);

		float Y = (float)_wtof(m_RotY);

		ENGINE::CTransform* pTransform = m_pBack->m_pCubeForDel->GetInfo();

		if(m_RadioPoint[0].GetCheck())
			pTransform->m_fAngle[ENGINE::ANGLE_Y] = D3DXToRadian(Y);
		else
			pTransform->m_fAngle[ENGINE::ANGLE_Y] += D3DXToRadian(Y);

		UpdateData(FALSE);

		((CMainFrame*)AfxGetMainWnd())->m_pMainView->Invalidate(FALSE);
	}
}

void CCreateCube::OnRotZIn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_Radio[0].GetCheck() && m_pBack->m_pCubeForDel != NULL)
	{
		UpdateData(TRUE);

		float Z = (float)_wtof(m_RotZ);

		Z += m_fRotVal;

		ENGINE::CTransform* pTransform = m_pBack->m_pCubeForDel->GetInfo();

		if(m_RadioPoint[0].GetCheck())
			pTransform->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(Z);
		else
			pTransform->m_fAngle[ENGINE::ANGLE_Z] += D3DXToRadian(Z);

		m_RotZ.Format(_T("%.3f"), Z);

		UpdateData(FALSE);

		((CMainFrame*)AfxGetMainWnd())->m_pMainView->Invalidate(FALSE);
	}
}

void CCreateCube::OnRotZDe()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_Radio[0].GetCheck() && m_pBack->m_pCubeForDel != NULL)
	{
		UpdateData(TRUE);

		float Z = (float)_wtof(m_RotZ);

		Z -= m_fRotVal;

		ENGINE::CTransform* pTransform = m_pBack->m_pCubeForDel->GetInfo();

		if(m_RadioPoint[0].GetCheck())
			pTransform->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(Z);
		else
			pTransform->m_fAngle[ENGINE::ANGLE_Z] += D3DXToRadian(Z);

		m_RotZ.Format(_T("%.3f"), Z);

		UpdateData(FALSE);

		((CMainFrame*)AfxGetMainWnd())->m_pMainView->Invalidate(FALSE);
	}
}

void CCreateCube::OnRotZEu()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if(m_Radio[0].GetCheck() && m_pBack->m_pCubeForDel != NULL)
	{
		UpdateData(TRUE);

		float Z = (float)_wtof(m_RotZ);

		ENGINE::CTransform* pTransform = m_pBack->m_pCubeForDel->GetInfo();

		if(m_RadioPoint[0].GetCheck())
			pTransform->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(Z);
		else
			pTransform->m_fAngle[ENGINE::ANGLE_Z] += D3DXToRadian(Z);

		UpdateData(FALSE);

		((CMainFrame*)AfxGetMainWnd())->m_pMainView->Invalidate(FALSE);
	}
}

void CCreateCube::OnPosXIn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if(m_Radio[0].GetCheck() && m_pBack->m_pCubeForDel != NULL)
	{
		UpdateData(TRUE);

		float X = (float)_wtof(m_PosX);

		X += m_fPosVal;

		ENGINE::CTransform* pTransform = m_pBack->m_pCubeForDel->GetInfo();

		if(m_RadioPoint[0].GetCheck())
			pTransform->m_vPos.x = X;
		else
			pTransform->m_vPos.x += X;

		m_PosX.Format(_T("%.3f"), X);

		UpdateData(FALSE);

		((CMainFrame*)AfxGetMainWnd())->m_pMainView->Invalidate(FALSE);
	}
}

void CCreateCube::OnPosXDe()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if(m_Radio[0].GetCheck() && m_pBack->m_pCubeForDel != NULL)
	{
		UpdateData(TRUE);

		float X = (float)_wtof(m_PosX);

		X -= m_fPosVal;

		ENGINE::CTransform* pTransform = m_pBack->m_pCubeForDel->GetInfo();

		if(m_RadioPoint[0].GetCheck())
			pTransform->m_vPos.x = X;
		else
			pTransform->m_vPos.x += X;

		m_PosX.Format(_T("%.3f"), X);

		UpdateData(FALSE);

		((CMainFrame*)AfxGetMainWnd())->m_pMainView->Invalidate(FALSE);
	}
}

void CCreateCube::OnPosXEu()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다/
	if(m_Radio[0].GetCheck() && m_pBack->m_pCubeForDel != NULL)
	{
		UpdateData(TRUE);

		float X = (float)_wtof(m_PosX);

		ENGINE::CTransform* pTransform = m_pBack->m_pCubeForDel->GetInfo();

		if(m_RadioPoint[0].GetCheck())
			pTransform->m_vPos.x = X;
		else
			pTransform->m_vPos.x += X;

		UpdateData(FALSE);

		((CMainFrame*)AfxGetMainWnd())->m_pMainView->Invalidate(FALSE);
	}
}

void CCreateCube::OnPosYIn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if(m_Radio[0].GetCheck() && m_pBack->m_pCubeForDel != NULL)
	{
		UpdateData(TRUE);

		float Y = (float)_wtof(m_PosY);

		Y += m_fPosVal;

		ENGINE::CTransform* pTransform = m_pBack->m_pCubeForDel->GetInfo();

		if(m_RadioPoint[0].GetCheck())
			pTransform->m_vPos.y = Y;
		else
			pTransform->m_vPos.y += Y;

		m_PosY.Format(_T("%.3f"), Y);

		UpdateData(FALSE);

		((CMainFrame*)AfxGetMainWnd())->m_pMainView->Invalidate(FALSE);
	}
}

void CCreateCube::OnPosYDe()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if(m_Radio[0].GetCheck() && m_pBack->m_pCubeForDel != NULL)
	{
		UpdateData(TRUE);

		float Y = (float)_wtof(m_PosY);

		Y -= m_fPosVal;

		ENGINE::CTransform* pTransform = m_pBack->m_pCubeForDel->GetInfo();

		if(m_RadioPoint[0].GetCheck())
			pTransform->m_vPos.y = Y;
		else
			pTransform->m_vPos.y += Y;

		m_PosY.Format(_T("%.3f"), Y);

		UpdateData(FALSE);

		((CMainFrame*)AfxGetMainWnd())->m_pMainView->Invalidate(FALSE);
	}
}

void CCreateCube::OnPosYEu()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if(m_Radio[0].GetCheck() && m_pBack->m_pCubeForDel != NULL)
	{
		UpdateData(TRUE);

		float Y = (float)_wtof(m_PosY);

		ENGINE::CTransform* pTransform = m_pBack->m_pCubeForDel->GetInfo();

		if(m_RadioPoint[0].GetCheck())
			pTransform->m_vPos.y = Y;
		else
			pTransform->m_vPos.y += Y;

		UpdateData(FALSE);

		((CMainFrame*)AfxGetMainWnd())->m_pMainView->Invalidate(FALSE);
	}
}

void CCreateCube::OnPosZIn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_Radio[0].GetCheck() && m_pBack->m_pCubeForDel != NULL)
	{
		UpdateData(TRUE);

		float Z = (float)_wtof(m_PosZ);

		Z += m_fPosVal;

		ENGINE::CTransform* pTransform = m_pBack->m_pCubeForDel->GetInfo();

		if(m_RadioPoint[0].GetCheck())
			pTransform->m_vPos.z = Z;
		else
			pTransform->m_vPos.z += Z;

		m_PosZ.Format(_T("%.3f"), Z);

		UpdateData(FALSE);

		((CMainFrame*)AfxGetMainWnd())->m_pMainView->Invalidate(FALSE);
	}
}

void CCreateCube::OnPosZDe()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_Radio[0].GetCheck() && m_pBack->m_pCubeForDel != NULL)
	{
		UpdateData(TRUE);

		float Z = (float)_wtof(m_PosZ);

		Z -= m_fPosVal;

		ENGINE::CTransform* pTransform = m_pBack->m_pCubeForDel->GetInfo();

		if(m_RadioPoint[0].GetCheck())
			pTransform->m_vPos.z = Z;
		else
			pTransform->m_vPos.z += Z;

		m_PosZ.Format(_T("%.3f"), Z);

		UpdateData(FALSE);

		((CMainFrame*)AfxGetMainWnd())->m_pMainView->Invalidate(FALSE);
	}
}

void CCreateCube::OnPosZEu()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_Radio[0].GetCheck() && m_pBack->m_pCubeForDel != NULL)
	{
		UpdateData(TRUE);

		float Z = (float)_wtof(m_PosZ);

		ENGINE::CTransform* pTransform = m_pBack->m_pCubeForDel->GetInfo();

		if(m_RadioPoint[0].GetCheck())
			pTransform->m_vPos.z = Z;
		else
			pTransform->m_vPos.z += Z;

		UpdateData(FALSE);

		((CMainFrame*)AfxGetMainWnd())->m_pMainView->Invalidate(FALSE);
	}
}

void CCreateCube::OnSelectCube()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	if(m_pBack->m_pCubeForDel != NULL)
	{
		CObstacle* pCubeObj = m_pBack->m_pCubeForDel;

		m_RadioPoint[0].SetCheck(1);
		m_RadioPoint[1].SetCheck(0);

		float fPosX = (float)_wtof(m_PosX);
		float fPosY = (float)_wtof(m_PosY);
		float fPosZ = (float)_wtof(m_PosZ);

		float fScaleX = (float)_wtof(m_ScaleX);
		float fScaleY = (float)_wtof(m_ScaleY);
		float fScaleZ = (float)_wtof(m_ScaleZ);

		float fRotX = (float)_wtof(m_RotX);
		float fRotY = (float)_wtof(m_RotY);
		float fRotZ = (float)_wtof(m_RotZ);


		fScaleX = pCubeObj->GetInfo()->m_vScale.x;
		fScaleY = pCubeObj->GetInfo()->m_vScale.y;
		fScaleZ = pCubeObj->GetInfo()->m_vScale.z;

		
		fRotX = D3DXToDegree(pCubeObj->GetInfo()->m_fAngle[ENGINE::ANGLE_X]);
		fRotY = D3DXToDegree(pCubeObj->GetInfo()->m_fAngle[ENGINE::ANGLE_Y]);
		fRotZ = D3DXToDegree(pCubeObj->GetInfo()->m_fAngle[ENGINE::ANGLE_Z]); 

		fPosX = pCubeObj->GetInfo()->m_vPos.x;
		fPosY = pCubeObj->GetInfo()->m_vPos.y;
		fPosZ = pCubeObj->GetInfo()->m_vPos.z;



		m_ScaleX.Format(_T("%.3f"), fScaleX);
		m_ScaleY.Format(_T("%.3f"), fScaleY);
		m_ScaleZ.Format(_T("%.3f"), fScaleZ);

		m_RotX.Format(_T("%.3f"), fRotX);
		m_RotY.Format(_T("%.3f"), fRotY);
		m_RotZ.Format(_T("%.3f"), fRotZ);

		m_PosX.Format(_T("%.3f"), fPosX);
		m_PosY.Format(_T("%.3f"), fPosY);
		m_PosZ.Format(_T("%.3f"), fPosZ);
	}

	UpdateData(FALSE);
}
