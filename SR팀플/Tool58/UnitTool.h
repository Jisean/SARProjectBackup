#pragma once
#include "afxwin.h"


// CUnitTool 대화 상자입니다.

class CUnitTool : public CDialog
{
	DECLARE_DYNAMIC(CUnitTool)

public:
	CUnitTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CUnitTool();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_UNITTOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:	// value type
	CString m_strTemp;
	CString m_strCopy;

	CString m_strName;
	int m_iAttack;
	int m_iHp;

	CString m_strFindName;

public: // control type
	CListBox m_ListBox;
	CButton m_iRadio[3];
	CButton m_Check[3];
	CButton m_BitMapButton;



public:	// user type
	map<CString, UNITDATA*>			m_MapUnitData;	
	bool	m_bStart;

public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnUnitAdd();
	afx_msg void OnListBox();
	
	
	
	afx_msg void OnDeleteButton();
	afx_msg void OnSearchFunc();
	
	afx_msg void OnSaveFunc();
	afx_msg void OnLoadFunc();
};
