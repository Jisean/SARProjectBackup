#pragma once
#include "afxwin.h"


// CPathFind 대화 상자입니다.

class CPathFind : public CDialog
{
	DECLARE_DYNAMIC(CPathFind)

public:
	CPathFind(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPathFind();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PATHFIND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
