
// Tool58View.h : CTool58View 클래스의 인터페이스
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
protected: // serialization에서만 만들어집니다.
	CTool58View();
	DECLARE_DYNCREATE(CTool58View)

// 특성입니다.
public:
	CTool58Doc* GetDocument() const;

// 작업입니다.
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

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CTool58View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
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

#ifndef _DEBUG  // Tool58View.cpp의 디버그 버전
inline CTool58Doc* CTool58View::GetDocument() const
   { return reinterpret_cast<CTool58Doc*>(m_pDocument); }
#endif

