#include "TerrainTex.h"

ENGINE::CTerrainTex::CTerrainTex(LPDIRECT3DDEVICE9 pDevice)
: CVIBuffer(pDevice)
{

}

ENGINE::CTerrainTex::~CTerrainTex(void)
{	
	Release();
}
HRESULT ENGINE::CTerrainTex::CreateBuffer(const WORD& wCntX, const WORD& wCntZ, const WORD& wItv)
{
	m_dwVtxSize		= sizeof(VTXTEX);
	m_dwVtxCount	= wCntX * wCntZ;
	m_dwVtxFVF		= VTXFVF_TEX;
	m_dwTriCnt		= (wCntX - 1) * (wCntZ - 1) * 2;
	m_dwIdxSize		= sizeof(INDEX16);
	m_IdxFmt		= D3DFMT_INDEX16;

	if(FAILED(CVIBuffer::CreateBuffer()))
		return E_FAIL;

	//DWORD*		pdwPixel = LoadImage();

	VTXTEX*		pVtxTex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVtxTex, 0);

	int	 iIndex = 0;

	for(int z = 0; z < wCntZ; ++z)
	{
		for(int x = 0; x < wCntX; ++x)
		{
			iIndex = z * wCntX + x;

			pVtxTex[iIndex].vPos    = D3DXVECTOR3(
				float(x) * wItv, 
				0.f, 
				float(z) * wItv);
			pVtxTex[iIndex].vTex	= D3DXVECTOR2(x / (wCntX - 1.f), z / (wCntZ - 1.f)); 

		}
	}

	m_pVB->Unlock();

	INDEX16*	pIndex = NULL;

	m_pIB->Lock(0, 0 , (void**)&pIndex, 0);

	int	iTriCnt = 0;

	for(int z = 0; z < wCntZ - 1; ++z)
	{
		for(int x = 0; x < wCntX - 1; ++x)
		{
			iIndex = z * wCntX + x;

			pIndex[iTriCnt]._1 = iIndex + wCntX;
			pIndex[iTriCnt]._2 = iIndex + wCntX + 1;
			pIndex[iTriCnt]._3 = iIndex + 1;
			++iTriCnt;

			pIndex[iTriCnt]._1 = iIndex + wCntX;
			pIndex[iTriCnt]._2 = iIndex + 1;
			pIndex[iTriCnt]._3 = iIndex;
			++iTriCnt;

		}
	}

	m_pIB->Unlock();

	//ENGINE::Safe_Delete_Array(pdwPixel);

	return S_OK;
}

DWORD* ENGINE::CTerrainTex::LoadImage(void)
{
	HANDLE		hFile = NULL;
	DWORD		dwByte = 0;

	hFile = CreateFile(L"../bin/Resources/Texture/Terrain/Height.bmp", GENERIC_READ, 
		0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	BITMAPFILEHEADER	fh;
	BITMAPINFOHEADER	ih;

	ReadFile(hFile, &fh, sizeof(BITMAPFILEHEADER), &dwByte, NULL);
	ReadFile(hFile, &ih, sizeof(BITMAPINFOHEADER), &dwByte, NULL);

	DWORD*		pdwPixel = new DWORD[ih.biHeight * ih.biWidth];

	ReadFile(hFile, pdwPixel, sizeof(DWORD) * ih.biHeight * ih.biWidth, &dwByte, NULL);

	CloseHandle(hFile);


	return pdwPixel;
}

ENGINE::CTerrainTex* ENGINE::CTerrainTex::Create(LPDIRECT3DDEVICE9 pDevice, 
												 const WORD& wCntX, const WORD& wCntZ, const WORD& vItv)
{
	CTerrainTex*		pTerrainTex = new CTerrainTex(pDevice);
	if(FAILED(pTerrainTex->CreateBuffer(wCntX, wCntZ, vItv)))
		ENGINE::Safe_Delete(pTerrainTex);

	return pTerrainTex;
}

void ENGINE::CTerrainTex::Release(void)
{
		
}

ENGINE::CResources* ENGINE::CTerrainTex::CloneResource(void)
{
	++m_wRefCnt;

	return new CTerrainTex(*this);
}
