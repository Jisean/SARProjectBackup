#pragma once
#include "2DTexture.h"

class CMultiTexture :
	public TwoDCTexture
{
private:
	map<wstring, vector<TEXINFO*>>		m_MapMultiTexture;

public:
	CMultiTexture(void);
	~CMultiTexture(void);

public:
	virtual HRESULT InsertTexture(	const wstring& wstrFilePath, 
									const wstring& wstrStateKey = L"", 
									const int& iCount = 0);

	virtual const TEXINFO* GetTexture(const wstring& wstrStateKey = L"", 
									const int& iCount = 0);

	virtual void Release(void);

	size_t GetImageCount(wstring wstrStateKey);

};
