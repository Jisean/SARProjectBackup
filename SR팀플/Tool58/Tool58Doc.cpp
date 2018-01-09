
// Tool58Doc.cpp : CTool58Doc 클래스의 구현
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


// CTool58Doc 생성/소멸

CTool58Doc::CTool58Doc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CTool58Doc::~CTool58Doc()
{
}

BOOL CTool58Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CTool58Doc serialization

void CTool58Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}


// CTool58Doc 진단

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


// CTool58Doc 명령
