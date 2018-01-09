#include "Texture.h"

int ENGINE::CTexture::m_iImgCnt = 0;

ENGINE::CTexture::CTexture(LPDIRECT3DDEVICE9 pDevice)
:CResources(pDevice)
{

}

ENGINE::CTexture::~CTexture(void)
{
	Release();
}


HRESULT ENGINE::CTexture::LoadTexture(TEXTURETYPE eTexType, const wstring& wstrFilePath, const WORD& wCnt)
{
	IDirect3DBaseTexture9*		pTexture = NULL;

	m_vecTexture.reserve(wCnt);

	TCHAR	szFullPath[MAX_PATH] = L"";

	for(size_t i = 0; i < wCnt; ++i)
	{
		wsprintf(szFullPath, wstrFilePath.c_str(), i);

		HRESULT	hr = NULL;

		switch(eTexType)
		{
		case TEX_NORMAL:
			hr = D3DXCreateTextureFromFile(m_pDevice, szFullPath, (LPDIRECT3DTEXTURE9*)&pTexture);
			break;

		case TEX_CUBE:
			hr = D3DXCreateCubeTextureFromFile(m_pDevice, szFullPath, (LPDIRECT3DCUBETEXTURE9*)&pTexture);
			break;
		}

		FAILED_CHECK_MSG(hr, szFullPath);

		++m_iImgCnt;

		m_vecTexture.push_back(pTexture);
	}

	m_dwContainerSize = m_vecTexture.size();

	return S_OK;
}

ENGINE::CTexture* ENGINE::CTexture::Create(LPDIRECT3DDEVICE9 pDevice, 
										   TEXTURETYPE eTexType, const wstring& wstrFilePath, const WORD& wCnt)
{
	CTexture*		pTexture = new CTexture(pDevice);
	
	if(FAILED(pTexture->LoadTexture(eTexType, wstrFilePath, wCnt)))
		ENGINE::Safe_Delete(pTexture);

	return pTexture;
}

void ENGINE::CTexture::Release(void)
{
	if(m_wRefCnt == 0)
	{
		for(size_t i = 0; i < m_vecTexture.size(); ++i)
		{
			ENGINE::Safe_Release(m_vecTexture[i]);
		}
		m_vecTexture.clear();
	}

	else
	{
		--m_wRefCnt;
	}
}

void ENGINE::CTexture::Render(const DWORD& iIndex)
{
	if(iIndex >= m_dwContainerSize)
		return;

	m_pDevice->SetTexture(0, m_vecTexture[iIndex]);
}

ENGINE::CResources* ENGINE::CTexture::CloneResource(void)
{
	++m_wRefCnt;

	return new CTexture(*this);
}
