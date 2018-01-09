#pragma once
#include "afxwin.h"


// CTextureTool ��ȭ �����Դϴ�.

namespace ENGINE
{
	class CResourcesMgr;
}


class CTextureTool : public CDialog
{
	DECLARE_DYNAMIC(CTextureTool)

public:
	CTextureTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CTextureTool();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_TEXTURETOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

private:
	ENGINE::CResourcesMgr*		m_pResourceMgr;

public:
	CListBox m_ListBox;
	afx_msg void OnLoad();
	afx_msg void OnClear();
	afx_msg void OnSelect();
};
