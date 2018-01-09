#pragma once

#include "Include.h"
#include "Engine_Include.h"
class TwoDCTexture;
class CTextureMgr
{
	DECLARE_SINGLETON(CTextureMgr)
private:
	map<wstring, TwoDCTexture*>		m_MapTexture;
	wstring						m_wstrFullPath;

public:
	void	SetPathName(const wstring& wstrName);
	const wstring& GetPathName(void);

public:
	HRESULT	InsertTexture(	const wstring& wstrFilePath, 
							const wstring& wstrObjKey,
							TEXTYPE	eType,
							const wstring& wstrStateKey = L"", 
							const int& iCount = 0);

	const TEXINFO* GetTexture(	const wstring& wstrObjKey,
								const wstring& wstrStateKey = L"",	
								const int& iCount = 0);

	void Release(void);

	HRESULT	ReadImgPath(const wstring& wstrImgPath);
	
	size_t GetImageCount(const wstring& wstrObjKey, 
						const wstring& wstrStateKey = L"");


private:
	CTextureMgr(void);
	~CTextureMgr(void);
};
