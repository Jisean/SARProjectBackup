#include "StdAfx.h"
#include "SingleTexture.h"

CSingleTexture::CSingleTexture(void)
{
}

CSingleTexture::~CSingleTexture(void)
{
	Release();
}

HRESULT CSingleTexture::InsertTexture(const wstring& wstrFilePath, 
									  const wstring& wstrStateKey /*= L""*/, 
									  const int& iCount /*= 0*/)
{
	m_pTexInfo = new TEXINFO;
	ZeroMemory(m_pTexInfo, sizeof(TEXINFO));

	if(FAILED(D3DXGetImageInfoFromFile(wstrFilePath.c_str(), &m_pTexInfo->tImgInfo)))
	{
		ERR_MSG(wstrFilePath.c_str());
		return E_FAIL;
	}

	if(FAILED(D3DXCreateTextureFromFileEx(CDevice::GetInst()->GetDevice(), 
		wstrFilePath.c_str(), 
		m_pTexInfo->tImgInfo.Width,
		m_pTexInfo->tImgInfo.Height, 
		m_pTexInfo->tImgInfo.MipLevels,
		0,
		m_pTexInfo->tImgInfo.Format,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT, // �׵θ� ǥ��
		D3DX_DEFAULT, // Ȯ��, ��� ǥ��
		NULL,	// ������ ����
		&m_pTexInfo->tImgInfo, // �̹��� ������ ������ ����ü
		NULL, // 8��Ʈ �̸��� �̹��� ó��
		&m_pTexInfo->pTexture)))
	{
		ERR_MSG(L"Single Texture Image Load Failed");
		return E_FAIL;
	}



	return S_OK;
}

const TEXINFO* CSingleTexture::GetTexture(const wstring& wstrStateKey /*= L""*/, 
										  const int& iCount /*= 0*/)
{
	return m_pTexInfo;
}

void CSingleTexture::Release(void)
{
	m_pTexInfo->pTexture->Release();
	::Safe_Delete(m_pTexInfo);
}
