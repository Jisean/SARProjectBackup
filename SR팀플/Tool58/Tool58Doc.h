
// Tool58Doc.h : CTool58Doc Ŭ������ �������̽�
//


#pragma once


class CTool58Doc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CTool58Doc();
	DECLARE_DYNCREATE(CTool58Doc)

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// �����Դϴ�.
public:
	virtual ~CTool58Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
};


