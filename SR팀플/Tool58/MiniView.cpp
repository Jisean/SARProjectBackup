// MiniView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool58.h"
#include "MiniView.h"
#include "Device.h"
#include "Back.h"
#include "MainFrm.h"
#include "Tool58View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMiniView

IMPLEMENT_DYNCREATE(CMiniView, CView)

CMiniView::CMiniView()
{

}

CMiniView::~CMiniView()
{
}

BEGIN_MESSAGE_MAP(CMiniView, CView)
END_MESSAGE_MAP()


// CMiniView �׸����Դϴ�.

void CMiniView::OnDraw(CDC* /*pDC*/)
{
	CDocument* pDoc = GetDocument();
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.

	/*CObj*		pBack = ((CMainFrame*)AfxGetMainWnd())->m_pMainView->m_pBack;

	CDevice::GetInst()->GetDevice()->Clear(0, NULL, 
		D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, 
		D3DCOLOR_ARGB(255, 0, 0, 255), 1.f, 0);
	CDevice::GetInst()->Render_Begin();

	((CBack*)pBack)->MiniViewRender();
	
	CDevice::GetInst()->Render_End();
	CDevice::GetInst()->GetDevice()->Present(NULL, NULL, m_hWnd, NULL);*/

}


// CMiniView �����Դϴ�.

#ifdef _DEBUG
void CMiniView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CMiniView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMiniView �޽��� ó�����Դϴ�.
