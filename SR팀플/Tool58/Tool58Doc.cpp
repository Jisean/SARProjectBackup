
// Tool58Doc.cpp : CTool58Doc Ŭ������ ����
//

#include "stdafx.h"
#include "Tool58.h"

#include "Tool58Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTool58Doc

IMPLEMENT_DYNCREATE(CTool58Doc, CDocument)

BEGIN_MESSAGE_MAP(CTool58Doc, CDocument)
END_MESSAGE_MAP()


// CTool58Doc ����/�Ҹ�

CTool58Doc::CTool58Doc()
{
	// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.

}

CTool58Doc::~CTool58Doc()
{
}

BOOL CTool58Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���⿡ ���ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.

	return TRUE;
}




// CTool58Doc serialization

void CTool58Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	}
	else
	{
		// TODO: ���⿡ �ε� �ڵ带 �߰��մϴ�.
	}
}


// CTool58Doc ����

#ifdef _DEBUG
void CTool58Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTool58Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CTool58Doc ���
