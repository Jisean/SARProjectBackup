// MyForm.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool58.h"
#include "MyForm.h"
#include "MainFrm.h"

#include "Back.h"
#include "Tool58View.h"
#include "CubeObj.h"
#include "Transform.h"
#include "..\Engine\System\Code\GraphicDev.h"
#include "Obstacle.h"
#include "Export_Function.h"
#include "..\Engine\Resources\Code\Texture.h"
#include "SpinObstacle.h"
#include "UpDownCube.h"
#include "CentryGun.h"
#include "SpinCutter.h"
#include "CrashMaze.h"
#include "ShrinkingPlatform.h"
#include "RightToLeft.h"
#include "LeftToRight.h"
#include "SpinSnow.h"
#include "CubeStair.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMyForm

IMPLEMENT_DYNCREATE(CMyForm, CFormView)

CMyForm::CMyForm()
	: CFormView(CMyForm::IDD)
{

}

CMyForm::~CMyForm()
{
}

void CMyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);

	Invalidate(FALSE);
}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &CMyForm::OnSave)
	ON_BN_CLICKED(IDC_BUTTON7, &CMyForm::OnLoad)
	ON_BN_CLICKED(IDC_BUTTON9, &CMyForm::OnPathFind)
ON_BN_CLICKED(IDC_BUTTON2, &CMyForm::OnCreateCube)
ON_BN_CLICKED(IDC_BUTTON36, &CMyForm::OnTextureTool)
END_MESSAGE_MAP()


// CMyForm �����Դϴ�.

#ifdef _DEBUG
void CMyForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyForm �޽��� ó�����Դϴ�.

void CMyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	m_Font.CreatePointFont(100, L"����");
	GetDlgItem(IDC_BUTTON9)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON2)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON36)->SetFont(&m_Font);
}

void CMyForm::OnSave()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	//wofstream		SaveFile;
	//SaveFile.open(L"../Data/MapData.bat", ios::out);	// ���� ����

	//int		iCount = 0;

	//CObj* pObj = ((CMainFrame*)AfxGetMainWnd())->m_pMainView->m_pBack;
	//
	//vector<CCubeObj*>::iterator iter = ((CBack*)pObj)->m_vecCube.begin();
	//vector<CCubeObj*>::iterator iter_end = ((CBack*)pObj)->m_vecCube.end();


	//for(iter; iter != iter_end; ++iter)
	//{
	//	++iCount;

	//	SaveFile << (*iter)->GetInfo()->m_vPos << L"|";
	//	SaveFile << (*iter)->GetInfo()->m_vScale << L"|";
	//	SaveFile << (*iter)->GetInfo()->m_fAngle[ENGINE::ANGLE_X] << L"|";
	//	SaveFile << (*iter)->GetInfo()->m_fAngle[ENGINE::ANGLE_Y] << L"|";
	//	SaveFile << (*iter)->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] << L"|";
	//	SaveFile << (*iter)->GetInfo()->m_fAngle[ENGINE::ANGLE_X] << L"|";

	//	if(iCount == ((CBack*)pObj)->m_vecCube.size())
	//		SaveFile << (*iter)->m_wstrObjKey <<  flush;
	//	else
	//		SaveFile << (*iter)->m_wstrObjKey << endl;

	//}

	//SaveFile.close();	

	UpdateData(TRUE);

	CObj* pObj = ((CMainFrame*)AfxGetMainWnd())->m_pMainView->m_pBack;
	int iCount = ((CBack*)pObj)->m_vecCube.size();
	
	FILE* pFile = NULL;

	fopen_s(&pFile, "../Data/Stage2.dat", "wb");

	fwrite(&iCount, sizeof(int), 1, pFile);

	vector<CObstacle*>::iterator iter = ((CBack*)pObj)->m_vecCube.begin();
	vector<CObstacle*>::iterator iter_end = ((CBack*)pObj)->m_vecCube.end();

	for(iter; iter != iter_end; ++iter)
	{
		fwrite(&((*iter)->m_iflag), sizeof(int), 1, pFile);

		
		if(((*iter)->m_iflag) == 7)
		{
			float fX = 3.f - ((CLeftToRight*)(*iter))->m_fMaxMove;
			float fY = 7.f - ((CLeftToRight*)(*iter))->m_fMaxHeight;

			D3DXVECTOR3 vPos = (*iter)->GetInfo()->m_vPos;
			vPos.x -= fX;
			vPos.y += fY;

			fwrite(&vPos, sizeof(D3DXVECTOR3), 1, pFile);	
		}
		else if(((*iter)->m_iflag) == 8)
		{
			float fX = 3.f - ((CLeftToRight*)(*iter))->m_fMaxMove;
			float fY = 7.f - ((CLeftToRight*)(*iter))->m_fMaxHeight;

			D3DXVECTOR3 vPos = (*iter)->GetInfo()->m_vPos;
			vPos.x += fX;
			vPos.y += fY;

			fwrite(&vPos, sizeof(D3DXVECTOR3), 1, pFile);
		}
		else
			fwrite(&((*iter)->GetInfo()->m_vPos), sizeof(D3DXVECTOR3), 1, pFile);
		fwrite(&((*iter)->GetInfo()->m_vScale), sizeof(D3DXVECTOR3), 1, pFile);
		fwrite((*iter)->GetInfo()->m_fAngle, sizeof(float), 3, pFile);
     
		TCHAR Temp[30];

 		memcpy(Temp, (*iter)->GetObjKey().c_str(), sizeof(TCHAR) * 30);

		fwrite(&Temp, sizeof(wchar_t), 30, pFile);
	}

	fclose(pFile);


	UpdateData(FALSE);
	
	

	
}

void CMyForm::OnLoad()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	
	UpdateData(TRUE);

	CObj* pObj = ((CMainFrame*)AfxGetMainWnd())->m_pMainView->m_pBack;
	int iCount = 0;
	int iFlag = 0;
	D3DXVECTOR3 vPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 vScale = D3DXVECTOR3(0.f, 0.f, 0.f);
	float fAngle[3] = {0};
	TCHAR ObjKey[30];
	wstring wstrObjKey = L"";

	FILE* pFile = NULL;

	fopen_s(&pFile, "../Data/Stage2.dat", "rb");

	fread(&iCount, sizeof(int), 1, pFile);

	for(int i = 0; i != iCount; ++i)
	{
		fread(&iFlag, sizeof(int), 1, pFile);  
		fread(&vPos, sizeof(D3DXVECTOR3), 1, pFile);
		fread(&vScale, sizeof(D3DXVECTOR3), 1, pFile);
		fread(fAngle, sizeof(float), 3, pFile);
		fread(ObjKey, sizeof(TCHAR), 30, pFile);	

		wstrObjKey = ObjKey;

		//wcscpy_s((wchar_t*)wstrObjKey.c_str(), sizeof(wchar_t) * 30, ObjKey);

		CObstacle* pCubeObj = NULL;

		if(iFlag == 0)
			pCubeObj = CCubeObj::Create(ENGINE::CGraphicDev::GetInstance()->GetDevice());
		else if(iFlag == 1)
			pCubeObj = CSpinObstacle::Create(ENGINE::CGraphicDev::GetInstance()->GetDevice());
		else if(iFlag == 2)
			pCubeObj = CUpDownCube::Create(ENGINE::CGraphicDev::GetInstance()->GetDevice());
		else if(iFlag == 3)
			pCubeObj = CCentryGun::Create(ENGINE::CGraphicDev::GetInstance()->GetDevice());
		else if(iFlag == 4)
			pCubeObj = CSpinCutter::Create(ENGINE::CGraphicDev::GetInstance()->GetDevice());
		else if(iFlag == 5)
			pCubeObj = CCrashMaze::Create(ENGINE::CGraphicDev::GetInstance()->GetDevice());
		else if(iFlag == 6)
			pCubeObj = CShrinkingPlatform::Create(ENGINE::CGraphicDev::GetInstance()->GetDevice());
		else if(iFlag == 7)
			pCubeObj = CLeftToRight::Create(ENGINE::CGraphicDev::GetInstance()->GetDevice());
		else if(iFlag == 8)
			pCubeObj = CRightToLeft::Create(ENGINE::CGraphicDev::GetInstance()->GetDevice());
		else if(iFlag == 9)
			pCubeObj = CSpinSnow::Create(ENGINE::CGraphicDev::GetInstance()->GetDevice());
		else if(iFlag == 10)
			pCubeObj = CCubeStair::Create((ENGINE::CGraphicDev::GetInstance()->GetDevice()));

		
		pCubeObj->GetInfo()->m_vPos = vPos;
		pCubeObj->GetInfo()->m_vScale = vScale;

		if(iFlag != 9)
		{
			pCubeObj->GetInfo()->m_fAngle[ENGINE::ANGLE_X] = fAngle[0];
			pCubeObj->GetInfo()->m_fAngle[ENGINE::ANGLE_Y] = fAngle[1];
			pCubeObj->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = fAngle[2];
		}
		
		
	
		const ENGINE::CComponent*	pComponent = pCubeObj->GetComponent(L"Texture");

		// ������Ʈ �� �����̳� ã�ƿ� Texture �����ϱ�
		map<wstring, ENGINE::CComponent*>::iterator iter = pCubeObj->GetmapComponent()->find(L"Texture");
		pCubeObj->GetmapComponent()->erase(iter);

		ENGINE::Safe_Delete(pComponent);

		ENGINE::CResourcesMgr* m_pResourceMgr = ENGINE::Get_ResourcesMgr();
		pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, ObjKey);


		// ������Ʈ �� �����̳ʿ� �߰��ϱ�
		ENGINE::CComponent* pComponent2 = (ENGINE::CComponent*)pComponent;

		pCubeObj->GetmapComponent()->insert(map<wstring, ENGINE::CComponent*>::value_type(L"Texture", pComponent2));

		// CubeObj�� Texture �����ϱ�
		((CObstacle*)pCubeObj)->SetTexture(dynamic_cast<ENGINE::CTexture*>(pComponent2));
		pCubeObj->m_wstrObjKey = ObjKey;


		((CBack*)pObj)->m_vecCube.push_back(pCubeObj);
	
		
	}

	fclose(pFile);


	UpdateData(FALSE);

}

//void CMyForm::OnPopUp()
//{
//	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
//	if(m_PopUp.GetSafeHwnd() == NULL)
//	{
//		m_PopUp.Create(IDD_POPUP);
//	}
//
//	m_PopUp.ShowWindow(SW_SHOW);
//}

void CMyForm::OnPathFind()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	if(m_PathFind.GetSafeHwnd() == NULL)
	{
		m_PathFind.Create(IDD_PATHFIND);
	}

	m_PathFind.ShowWindow(SW_SHOW);
	//((CMainFrame*)AfxGetMainWnd())->Invalidate(FALSE);

	//Invalidate(FALSE);

}
void CMyForm::OnCreateCube()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(m_CreateCube.GetSafeHwnd() == NULL)
	{
		m_CreateCube.Create(IDD_CREATECUBE);
	}
	m_CreateCube.ShowWindow(SW_SHOW);
	//((CMainFrame*)AfxGetMainWnd())->Invalidate(FALSE);

	//Invalidate(TRUE);
}

void CMyForm::OnTextureTool()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	if(m_TextureTool.GetSafeHwnd() == NULL)
	{
		m_TextureTool.Create(IDD_TEXTURETOOL);
	}
	m_TextureTool.ShowWindow(SW_SHOW);
	//((CMainFrame*)AfxGetMainWnd())->Invalidate(FALSE);

	//Invalidate(FALSE);
}
