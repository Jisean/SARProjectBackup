#pragma once


// CPopUp ��ȭ �����Դϴ�.

#include "MySheet.h"

class CPopUp : public CDialog
{
	DECLARE_DYNAMIC(CPopUp)

public:
	CPopUp(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPopUp();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_POPUP };

	CMySheet*		m_pMySheet;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
