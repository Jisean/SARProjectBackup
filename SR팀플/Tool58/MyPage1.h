#pragma once


// CMyPage1 ��ȭ �����Դϴ�.

class CMyPage1 : public CPropertyPage
{
	DECLARE_DYNAMIC(CMyPage1)

public:
	CMyPage1();
	virtual ~CMyPage1();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_MYPAGE1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
