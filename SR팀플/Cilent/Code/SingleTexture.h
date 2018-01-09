#pragma once
#include "2DTexture.h"

class CSingleTexture :
	public TwoDCTexture
{
private:
	TEXINFO*			m_pTexInfo;

public:
	CSingleTexture(void);
	~CSingleTexture(void);

public:
	virtual HRESULT InsertTexture(	const wstring& wstrFilePath, 
									const wstring& wstrStateKey = L"", 
									const int& iCount = 0);

	virtual const TEXINFO* GetTexture(	const wstring& wstrStateKey = L"",	
										const int& iCount = 0);

	virtual void Release(void);

};
