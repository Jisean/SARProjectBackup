// MyPage1.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool58.h"
#include "MyPage1.h"


// CMyPage1 ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CMyPage1, CPropertyPage)

CMyPage1::CMyPage1()
	: CPropertyPage(CMyPage1::IDD)
{

}

CMyPage1::~CMyPage1()
{
}

void CMyPage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMyPage1, CPropertyPage)
END_MESSAGE_MAP()


// CMyPage1 �޽��� ó�����Դϴ�.
