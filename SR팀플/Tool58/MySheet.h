#pragma once



// CMySheet

#include "MyPage1.h"
#include "MyPage2.h"

class CMySheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CMySheet)

public:
	CMySheet(void);
	CMySheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CMySheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CMySheet();

public:
	CMyPage1			m_MyPage1;
	CMyPage2			m_MyPage2;


protected:
	DECLARE_MESSAGE_MAP()
};


