#pragma once
#include "afxwin.h"



// CMapTool ��ȭ �����Դϴ�.

class CMapTool : public CDialog
{
	DECLARE_DYNAMIC(CMapTool)

public:
	CMapTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMapTool();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_MAPTOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:		// control type
	CListBox m_ListBox;
	CStatic m_Picture;

public:
	map<CString, CImage*>		m_MapPngImage; 
	int							m_iDrawID;


public:
	void HorizontalScroll(void);

public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
	
	afx_msg void OnListBox();
	afx_msg void OnSaveButton();
};
