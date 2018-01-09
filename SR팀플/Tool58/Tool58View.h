
// Tool58View.h : CTool58View Ŭ������ �������̽�
//


#pragma once

#include "SingleTexture.h"

class CDevice;
class CObj;
class CTool58Doc;
class CCreateCube;
class CPathFind;
namespace ENGINE
{
	class CGraphicDev;
	class CResourcesMgr;
	class CTimeMgr;
}
class CTool58View : public CScrollView
{
protected: // serialization������ ��������ϴ�.
	CTool58View();
	DECLARE_DYNCREATE(CTool58View)

// Ư���Դϴ�.
public:
	CTool58Doc* GetDocument() const;

// �۾��Դϴ�.
public:
	ENGINE::CGraphicDev*		m_pDevice;
	ENGINE::CResourcesMgr*		m_pResourceMgr;
	ENGINE::CTimeMgr*			m_pTimeMgr;

	//CSingleTexture*	m_pSingle;
	CObj*			m_pBack;
	CCreateCube*	m_pCreateCube;
	CPathFind*		m_pPathFind;

	list<wstring>			m_KeyList;

public:
	static bool		m_bCreateCube;
	static bool		m_bPathFind;
	static bool		m_bInvalidate;

public:
	void		ImgPath(void);

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CTool58View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

#ifndef _DEBUG  // Tool58View.cpp�� ����� ����
inline CTool58Doc* CTool58View::GetDocument() const
   { return reinterpret_cast<CTool58Doc*>(m_pDocument); }
#endif

