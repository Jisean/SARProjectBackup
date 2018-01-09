
// Tool58View.cpp : CTool58View Ŭ������ ����
//

#include "stdafx.h"
#include "Tool58.h"

#include "Tool58Doc.h"
#include "Tool58View.h"
#include "Back.h"
#include "MainFrm.h"
#include "MiniView.h"
#include "MapTool.h"
#include "MyForm.h"
#include "GraphicDev.h"
#include "Export_Function.h"
#include "CreateCube.h"
#include "TimeMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND	g_hWnd;
bool CTool58View::m_bCreateCube = false;
bool CTool58View::m_bPathFind = false;
bool CTool58View::m_bInvalidate = false;

void CTool58View::ImgPath(void)
{
	wifstream		LoadFile;

	LoadFile.open(L"../Data/ImgPath_dds.txt", ios::in);	// ���� ����

	TCHAR	szObjKey[MIN_STR]	= L"";
	TCHAR	szCount[MIN_STR]	= L"";
	TCHAR	szImgPath[MAX_PATH] = L"";

	wstring	wstrCombine = L"";

	while(!LoadFile.eof())
	{
		LoadFile.getline(szObjKey, MIN_STR, '|');
		//LoadFile.getline(szCount, MIN_STR, '|');
		LoadFile.getline(szImgPath, MAX_PATH);

		int iCount = _ttoi(szCount);

		m_pResourceMgr->AddTexture(m_pDevice->GetDevice(), ENGINE::RESOURCE_DYNAMIC, 
			ENGINE::TEX_CUBE, szObjKey, szImgPath, 1);

		const wstring& wstrTemp = szObjKey;
		wstring wstrKey = wstrTemp;

		m_KeyList.push_back(wstrKey);
	}

	LoadFile.close();

	wifstream		LoadFile2;

	LoadFile2.open(L"../Data/ImgPath_png.txt", ios::in);	// ���� ����

	TCHAR	szObjKey2[MIN_STR]	= L"";
	TCHAR	szStateKey2[MIN_STR] = L"";
	TCHAR	szCount2[MIN_STR]	= L"";
	TCHAR	szImgPath2[MAX_PATH] = L"";

	wstring	wstrCombine2 = L"";

	while(!LoadFile2.eof())
	{
		LoadFile2.getline(szObjKey2, MIN_STR, '|');
		LoadFile2.getline(szStateKey2, MIN_STR, '|');
		LoadFile2.getline(szCount2, MIN_STR, '|');
		LoadFile2.getline(szImgPath2, MAX_PATH);
	
		int iCount = _ttoi(szCount2);
		wstrCombine2 = wstring(szObjKey2) + L"_" + wstring(szStateKey2);

		m_pResourceMgr->AddTexture(m_pDevice->GetDevice(), ENGINE::RESOURCE_DYNAMIC, 
			ENGINE::TEX_NORMAL, wstrCombine2, szImgPath2, iCount);
	}

	LoadFile2.close();	

}

// CTool58View

IMPLEMENT_DYNCREATE(CTool58View, CScrollView)

BEGIN_MESSAGE_MAP(CTool58View, CScrollView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CTool58View::OnFilePrintPreview)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CTool58View ����/�Ҹ�

CTool58View::CTool58View()
: m_pDevice(ENGINE::CGraphicDev::GetInstance())
, m_pBack(NULL)
, m_pTimeMgr(ENGINE::Get_Time())
//, m_pSingle(NULL)
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CTool58View::~CTool58View()
{
	
}

BOOL CTool58View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CTool58View �׸���

void CTool58View::OnDraw(CDC* pDC)
{
	CTool58Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.

/*
	if(SUCCEEDED( pDC->Rectangle(100, 100, 200, 200)))
	{
		AfxMessageBox(L"����");
		return;
	}
*/

	//D3DXMATRIX		matWorld, matScale, matRotZ, matTrans;

	//D3DXMatrixIdentity(&matWorld);
	//D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	//D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(45.f));
	//D3DXMatrixTranslation(&matTrans, 400.f, 300.f, 0.f);

	//matWorld = matScale/* * matRotZ*/ * matTrans;

	//float	fX = m_pSingle->GetTexture()->tImgInfo.Width / 2.f;
	//float	fY = m_pSingle->GetTexture()->tImgInfo.Height / 2.f;
	// 
	// 
	//const TEXINFO* pTexture = m_pTexture->GetTexture(L"CUBE");
	/*const TEXINFO* pTexture = m_pTexture->GetTexture(L"TILE", L"Tile", 3);
	
	float	fX = pTexture->tImgInfo.Width / 2.f;
	float	fY = pTexture->tImgInfo.Height / 2.f;*/

	m_pTimeMgr->SetTime();

	m_pDevice->GetDevice()->Clear(0, NULL, 
		D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 
		D3DCOLOR_ARGB(255, 0,0, 255), 1.f, 0);

	//m_pDevice->Render_Begin();
	m_pDevice->GetDevice()->BeginScene();

	UpdateData(TRUE);

	m_pBack->Progress();
	m_pBack->Render();

	//((CMainFrame*)AfxGetMainWnd())->Invalidate(FALSE);
	UpdateData(FALSE);

	

	//m_pDevice->Render_End();
	m_pDevice->GetDevice()->EndScene();
	m_pDevice->GetDevice()->Present(NULL, NULL, NULL, NULL);

	Invalidate(FALSE);


	if(m_bCreateCube == true)
	{
		m_pCreateCube->Invalidate(FALSE);

		if(m_pCreateCube->m_Radio[0].GetCheck())
			((CBack*)m_pBack)->m_bSelRadio = true;
	}

	if(m_bPathFind == true)
	{
		m_pPathFind->Invalidate(FALSE);
	}

	((CMainFrame*)AfxGetMainWnd())->m_pMyForm->RedrawWindow();

	

	//if(m_bInvalidate == true)
	//{
	//	Invalidate(FALSE);	
	//	m_bInvalidate = false;
	//}
}


// CTool58View �μ�


void CTool58View::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CTool58View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CTool58View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CTool58View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}

void CTool58View::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CTool58View::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CTool58View ����

#ifdef _DEBUG
void CTool58View::AssertValid() const
{
	CView::AssertValid();
}

void CTool58View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTool58Doc* CTool58View::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTool58Doc)));
	return (CTool58Doc*)m_pDocument;
}
#endif //_DEBUG


// CTool58View �޽��� ó����

void CTool58View::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	SetScrollSizes(MM_TEXT, CSize(TILECX * TILEX, (TILECY / 2) * TILEY));

	CMainFrame*		pMainFrm = ((CMainFrame*)AfxGetMainWnd());

	RECT		rcWindow;

	pMainFrm->GetWindowRect(&rcWindow);
	// ��ü �������� ����� ������ �Լ�

	SetRect(&rcWindow, 0, 0, rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top);

	RECT	rcMainView;
	GetClientRect(&rcMainView);		// ���� �� â�� ũ�⸦ �����ִ� �Լ�

	float	fRowFrm = float(rcWindow.right - rcMainView.right);
	float	fColFrm = float(rcWindow.bottom - rcMainView.bottom);

	pMainFrm->SetWindowPos(NULL, 
		0, 0, 
		int(WINCX + fRowFrm), 
		int(WINCY + fColFrm),
		SWP_NOZORDER);


	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	g_hWnd = m_hWnd;

	if(FAILED(m_pDevice->InitGraphicDev(ENGINE::CGraphicDev::MODE_WIN, 
		g_hWnd, WINCX, WINCY)))//InitDevice()))
	{
		AfxMessageBox(L"Device Create Failed");
		return;
	}

	m_pResourceMgr = ENGINE::Get_ResourcesMgr();

	/*m_pResourceMgr->AddTexture(m_pDevice->GetDevice(), ENGINE::RESOURCE_DYNAMIC, 
		ENGINE::TEX_CUBE, L"Texture_Cube", 
			L"../Cilent/bin/Resources/Texture/Monster%d.dds", 1);

	*/

	ImgPath();

	m_pResourceMgr->AddBuffer(m_pDevice->GetDevice(), ENGINE::RESOURCE_DYNAMIC, 
		ENGINE::CVIBuffer::BUFFER_CUBETEX, L"Buffer_CubeTex");

	// Terrain Texture
	//m_pResourceMgr->AddTexture(m_pDevice->GetDevice(), ENGINE::RESOURCE_DYNAMIC, 
	//		ENGINE::TEX_NORMAL, L"Texture_Terrain",  
	//		L"../Cilent/bin/Resources/Texture/Terrain/Terrain%d.png", 1);

	// Terrain Buffer
	m_pResourceMgr->AddBuffer(m_pDevice->GetDevice(), ENGINE::RESOURCE_DYNAMIC, 
		ENGINE::CVIBuffer::TERRAIN_TEX, L"Buffer_Terrain", VTXCNTX, VTXCNTZ, VTXITV);

	m_pBack = new CBack(m_pDevice->GetDevice());
	m_pBack->Initialize();
	((CBack*)m_pBack)->SetMainView(this);

	m_pCreateCube = ((CCreateCube*)&((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_CreateCube);
	m_pPathFind = ((CPathFind*)&((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_PathFind);

	m_pTimeMgr->InitTime();
}

void CTool58View::OnDestroy()
{
	CView::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.

		/*if(m_pSingle)
		::Safe_Delete(m_pSingle);
		*/

	::Safe_Delete(m_pBack);

	m_pDevice->DestroyInstance();//DestroyInst();
}

void CTool58View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	D3DXVECTOR3	vMouse = D3DXVECTOR3(::GetMouse().x + GetScrollPos(0), 
									 ::GetMouse().y + GetScrollPos(1), 
									 0.f);	// ��ũ�� ���� ����� ����� ��

	CMapTool*	pMapTool = &((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_MapTool;

	//((CBack*)m_pBack)->TileChange(vMouse, pMapTool->m_iDrawID);

	//((CMainFrame*)AfxGetMainWnd())->m_pMyForm->Invalidate(FALSE);
	
	Invalidate(FALSE);


	CScrollView::OnLButtonDown(nFlags, point);
}

void CTool58View::OnMouseMove(UINT nFlags, CPoint point)	
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CScrollView::OnMouseMove(nFlags, point);


	if(GetAsyncKeyState(VK_LBUTTON))
		OnLButtonDown(nFlags, point);
}
void CTool58View::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CScrollView::OnTimer(nIDEvent);
}
