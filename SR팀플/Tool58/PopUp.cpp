// PopUp.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool58.h"
#include "PopUp.h"


// CPopUp ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CPopUp, CDialog)

CPopUp::CPopUp(CWnd* pParent /*=NULL*/)
	: CDialog(CPopUp::IDD, pParent)
	, m_pMySheet(NULL)
{

}

CPopUp::~CPopUp()
{
	::Safe_Delete(m_pMySheet);
}

void CPopUp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPopUp, CDialog)
END_MESSAGE_MAP()


// CPopUp �޽��� ó�����Դϴ�.

BOOL CPopUp::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	m_pMySheet = new CMySheet;
	m_pMySheet->Create(this, WS_CHILD | WS_VISIBLE);
	m_pMySheet->MoveWindow(0, 0, 500, 400);


	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
