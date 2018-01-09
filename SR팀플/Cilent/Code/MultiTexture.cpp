#include "StdAfx.h"
#include "MultiTexture.h"
#include "Engine_Include.h"
#include "GraphicDev.h"
CMultiTexture::CMultiTexture(void)
{
}

CMultiTexture::~CMultiTexture(void)
{
	Release();
}

HRESULT CMultiTexture::InsertTexture(const wstring& wstrFilePath, 
									 const wstring& wstrStateKey /*= L""*/, 
									 const int& iCount /*= 0*/)
{
	TCHAR	szPath[MAX_PATH] = L"";
	vector<TEXINFO*>		vecTexture;

	for(int i = 0; i < iCount; ++i)
	{
		wsprintf(szPath, wstrFilePath.c_str(), i);

		TEXINFO*		pTexInfo = new TEXINFO;
		ZeroMemory(pTexInfo, sizeof(TEXINFO));

		if(FAILED(D3DXGetImageInfoFromFile(szPath, &pTexInfo->tImgInfo)))
		{
			ERR_MSG(szPath);
			return E_FAIL;
		}
		
		if(FAILED(D3DXCreateTextureFromFileEx(ENGINE::CGraphicDev::GetInstance()->GetDevice(), 
			szPath, 
			pTexInfo->tImgInfo.Width,
			pTexInfo->tImgInfo.Height, 
			pTexInfo->tImgInfo.MipLevels,
			0,
			pTexInfo->tImgInfo.Format,
			D3DPOOL_MANAGED,
			D3DX_DEFAULT, // 테두리 표현
			D3DX_DEFAULT, // 확대, 축소 표현
			NULL,	// 제거할 색상
			&pTexInfo->tImgInfo, // 이미지 정보를 저장할 구조체
			NULL, // 8비트 미만의 이미지 처리
			&pTexInfo->pTexture)))
		{
			ERR_MSG(L"Multi Texture Image Load Failed");
			return E_FAIL;
		}
		vecTexture.push_back(pTexInfo);

		++g_iImgCnt;
	}
	
	m_MapMultiTexture.insert(make_pair(wstrStateKey, vecTexture));	
	

	return S_OK;
}

const TEXINFO* CMultiTexture::GetTexture(const wstring& wstrStateKey /*= L""*/, 
										 const int& iCount /*= 0*/)
{
	map<wstring, vector<TEXINFO*>>::iterator iter = m_MapMultiTexture.find(wstrStateKey);

	if(iter == m_MapMultiTexture.end())
		return NULL;


	return iter->second[iCount];
}

void CMultiTexture::Release(void)
{
	for(map<wstring, vector<TEXINFO*>>::iterator	
		iter = m_MapMultiTexture.begin();
		iter != m_MapMultiTexture.end(); ++iter)
	{
		for(size_t i = 0; i < iter->second.size(); ++i)
		{
			iter->second[i]->pTexture->Release();
			//::Safe_Delete(iter->second[i]);
			if(iter->second[i])
			{
				delete iter->second[i];
				iter->second[i] = NULL;
			}
		}
		iter->second.clear();
		vector<TEXINFO*>().swap(iter->second);
	}
	m_MapMultiTexture.clear();
}

size_t CMultiTexture::GetImageCount(wstring wstrStateKey)
{
	map<wstring, vector<TEXINFO*>>::iterator		iter = m_MapMultiTexture.find(wstrStateKey);

	if(iter == m_MapMultiTexture.end())
		return -1;

	return iter->second.size();
}
