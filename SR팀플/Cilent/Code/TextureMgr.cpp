#include "StdAfx.h"
#include "TextureMgr.h"
#include "SingleTexture.h"
#include "MultiTexture.h"
#include "Engine_Include.h"
IMPLEMENT_SINGLETON(CTextureMgr)

CTextureMgr::CTextureMgr(void)
{
}

CTextureMgr::~CTextureMgr(void)
{
	Release();
}

HRESULT CTextureMgr::InsertTexture(const wstring& wstrFilePath, 
								   const wstring& wstrObjKey, 
								   TEXTYPE eType, 
								   const wstring& wstrStateKey /*= L""*/, 
								   const int& iCount /*= 0*/)
{
	map<wstring, TwoDCTexture*>::iterator		iter = m_MapTexture.find(wstrObjKey);

	m_wstrFullPath = wstrFilePath;		 

	if(iter == m_MapTexture.end())
	{
		TwoDCTexture*	pTexture = NULL;

		switch(eType)
		{
		case TEX_SINGLE:
			pTexture = new CSingleTexture;
			break;

		case TEX_MULTI:
			pTexture = new CMultiTexture;
			break;
		}

		if(FAILED(pTexture->InsertTexture(wstrFilePath, wstrStateKey, iCount)))
		{
			ERR_MSG(wstrObjKey.c_str());
			return E_FAIL;
		}

		m_MapTexture.insert(make_pair(wstrObjKey, pTexture));
	}
	else
	{
		if(TEX_MULTI == eType)
			iter->second->InsertTexture(wstrFilePath, wstrStateKey, iCount);
	}


	return S_OK;

}

void CTextureMgr::Release(void)
{
	/*for(map<wstring, CTexture*>::iterator		
		iter = m_MapTexture.begin();
		iter != m_MapTexture.end(); ++iter)
	{
		Safe_Delete(iter->second);
	}
	m_MapTexture.clear();*/

	for_each(m_MapTexture.begin(), m_MapTexture.end(), ENGINE::CDeleteMap());
	m_MapTexture.clear();
}

const TEXINFO* CTextureMgr::GetTexture(const wstring& wstrObjKey, 
									   const wstring& wstrStateKey /*= L""*/, 
									   const int& iCount /*= 0*/)
{
	map<wstring, TwoDCTexture*>::iterator	iter = m_MapTexture.find(wstrObjKey);

	if(iter == m_MapTexture.end())
		return NULL;

	return iter->second->GetTexture(wstrStateKey, iCount);
}

HRESULT CTextureMgr::ReadImgPath(const wstring& wstrImgPath)
{
	wifstream		LoadFile;

	LoadFile.open(wstrImgPath.c_str(), ios::in);	// 파일 개방

	TCHAR	szObjKey[MIN_STR]	= L"";
	TCHAR	szStateKey[MIN_STR] = L"";
	TCHAR	szCount[MIN_STR]	= L"";
	TCHAR	szImgPath[MAX_PATH] = L"";

	wstring	wstrCombine = L"";

	while(!LoadFile.eof())
	{
		LoadFile.getline(szObjKey, MIN_STR, '|');
		LoadFile.getline(szStateKey, MIN_STR, '|');
		LoadFile.getline(szCount, MIN_STR, '|');
		LoadFile.getline(szImgPath, MAX_PATH);

		int iCount = _ttoi(szCount);

		if(FAILED(CTextureMgr::GetInstance()->InsertTexture(szImgPath, szObjKey, 
		TEX_MULTI, szStateKey, iCount)))
		{
			return E_FAIL;
		}
		
	}

	LoadFile.close();	

	return S_OK;

}

size_t CTextureMgr::GetImageCount(const wstring& wstrObjKey, 
								  const wstring& wstrStateKey /*= L""*/)
{
	map<wstring, TwoDCTexture*>::iterator		iter = m_MapTexture.find(wstrObjKey);

	if(iter == m_MapTexture.end())
		return -1;

	return ((CMultiTexture*)iter->second)->GetImageCount(wstrStateKey);
}

void CTextureMgr::SetPathName(const wstring& wstrName)
{
	m_wstrFullPath = wstrName;
}

const wstring& CTextureMgr::GetPathName(void)
{
	return m_wstrFullPath;
}
