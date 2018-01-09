#include "StdAfx.h"
#include "SingleTexture.h"
#include "GraphicDev.h"

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

	if(FAILED(D3DXCreateTextureFromFileEx(ENGINE::CGraphicDev::GetInstance()->GetDevice(), 
		wstrFilePath.c_str(), 
		m_pTexInfo->tImgInfo.Width,
		m_pTexInfo->tImgInfo.Height, 
		m_pTexInfo->tImgInfo.MipLevels,
		0,
		m_pTexInfo->tImgInfo.Format,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT, // 테두리 표현
		D3DX_DEFAULT, // 확대, 축소 표현
		NULL,	// 제거할 색상
		&m_pTexInfo->tImgInfo, // 이미지 정보를 저장할 구조체
		NULL, // 8비트 미만의 이미지 처리
		&m_pTexInfo->pTexture)))
	{
		ERR_MSG(L"Single Texture Image Load Failed");
		return E_FAIL;
	}


	++g_iImgCnt;


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
	//::Safe_Delete(m_pTexInfo);
	if(m_pTexInfo)
	{
		delete m_pTexInfo;
		m_pTexInfo = NULL;
	}
}
