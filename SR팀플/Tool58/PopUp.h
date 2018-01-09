#pragma once


// CPopUp 대화 상자입니다.

#include "MySheet.h"

class CPopUp : public CDialog
{
	DECLARE_DYNAMIC(CPopUp)

public:
	CPopUp(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPopUp();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_POPUP };

	CMySheet*		m_pMySheet;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
