#pragma once

#include "UnitTool.h"
#include "MapTool.h"
#include "PathFind.h"
#include "CreateCube.h"
#include "TextureTool.h"

// CMyForm 폼 뷰입니다.


class CMyForm : public CFormView
{
	DECLARE_DYNCREATE(CMyForm)

protected:
	CMyForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMyForm();

public:
	enum { IDD = IDD_MYFORM };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	CFont			m_Font;
	CUnitTool		m_UnitTool;
	CMapTool		m_MapTool;
	CPathFind		m_PathFind;
	CCreateCube		m_CreateCube;
	CTextureTool	m_TextureTool;

public:

	virtual void OnInitialUpdate();
	afx_msg void OnSave();
	afx_msg void OnLoad();
	afx_msg void OnPathFind();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnCreateCube();
	afx_msg void OnTextureTool();
};


