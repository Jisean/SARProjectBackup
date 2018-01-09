#include "StdAfx.h"
#include "MultiTexture.h"

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

		if(FAILED(D3DXCreateTextureFromFileEx(CDevice::GetInst()->GetDevice(), 
			szPath, 
			pTexInfo->tImgInfo.Width,
			pTexInfo->tImgInfo.Height, 
			pTexInfo->tImgInfo.MipLevels,
			0,
			pTexInfo->tImgInfo.Format,
			D3DPOOL_MANAGED,
			D3DX_DEFAULT, // �׵θ� ǥ��
			D3DX_DEFAULT, // Ȯ��, ��� ǥ��
			NULL,	// ������ ����
			&pTexInfo->tImgInfo, // �̹��� ������ ������ ����ü
			NULL, // 8��Ʈ �̸��� �̹��� ó��
			&pTexInfo->pTexture)))
		{
			ERR_MSG(L"Multi Texture Image Load Failed");
			return E_FAIL;
		}
		vecTexture.push_back(pTexInfo);
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
			::Safe_Delete(iter->second[i]);
		}
		iter->second.clear();
		vector<TEXINFO*>().swap(iter->second);
	}
	m_MapMultiTexture.clear();
}
