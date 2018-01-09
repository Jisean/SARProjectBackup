#pragma once
#include "afxwin.h"


// CPathFind ��ȭ �����Դϴ�.

class CPathFind : public CDialog
{
	DECLARE_DYNAMIC(CPathFind)

public:
	CPathFind(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPathFind();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_PATHFIND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	list<IMGPATH*>			m_PathList;

public:
	CListBox m_ListBox;
	CButton m_Radio[2];

private:
	bool	m_bStart;

public:
	afx_msg void OnListBox();
	afx_msg void OnSave();
	afx_msg void OnLoad();
	afx_msg void OnDropFiles(HDROP hDropInfo);

	
};
