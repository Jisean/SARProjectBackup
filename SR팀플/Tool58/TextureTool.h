#pragma once
#include "afxwin.h"


// CTextureTool 대화 상자입니다.

namespace ENGINE
{
	class CResourcesMgr;
}


class CTextureTool : public CDialog
{
	DECLARE_DYNAMIC(CTextureTool)

public:
	CTextureTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTextureTool();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TEXTURETOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	ENGINE::CResourcesMgr*		m_pResourceMgr;

public:
	CListBox m_ListBox;
	afx_msg void OnLoad();
	afx_msg void OnClear();
	afx_msg void OnSelect();
};
