// PopUp.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool58.h"
#include "PopUp.h"


// CPopUp 대화 상자입니다.

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


// CPopUp 메시지 처리기입니다.

BOOL CPopUp::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_pMySheet = new CMySheet;
	m_pMySheet->Create(this, WS_CHILD | WS_VISIBLE);
	m_pMySheet->MoveWindow(0, 0, 500, 400);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
