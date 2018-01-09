#include "RcUI.h"

ENGINE::CRcUI::CRcUI(LPDIRECT3DDEVICE9 pDevice)
: CVIBuffer(pDevice)
{

}

ENGINE::CRcUI::~CRcUI(void)
{
	Release();
}

ENGINE::CRcUI* ENGINE::CRcUI::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CRcUI* pRcUI = new CRcUI(pDevice);

	if(FAILED(pRcUI->CreateBuffer()))
		ENGINE::Safe_Delete(pRcUI);

	return pRcUI;
}
void ENGINE::CRcUI::Release(void)
{

}

void ENGINE::CRcUI::Render()
{
	CVIBuffer::Render();
}

HRESULT ENGINE::CRcUI::CreateBuffer(void)
{
	m_dwVtxSize		= sizeof(VTXTEX);
	m_dwVtxCount	= 4;
	m_dwVtxFVF		= VTXFVF_TEX;
	m_dwTriCnt		= 2;
	m_dwIdxSize		= sizeof(INDEX16);
	m_IdxFmt		= D3DFMT_INDEX16;

	if(FAILED(CVIBuffer::CreateBuffer()))
		return E_FAIL;

	VTXTEX*		pVtxTex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVtxTex, 0);

	pVtxTex[0].vPos		= D3DXVECTOR3(-1.f, 1.f, 0.f);
	pVtxTex[0].vTex		= D3DXVECTOR2(0.f, 0.f);

	pVtxTex[1].vPos		= D3DXVECTOR3(1.f, 1.f, 0.f);
	pVtxTex[1].vTex		= D3DXVECTOR2(1.f, 0.f);

	pVtxTex[2].vPos		= D3DXVECTOR3(1.f, -1.f, 0.f);
	pVtxTex[2].vTex		= D3DXVECTOR2(1.f, 1.f);

	pVtxTex[3].vPos		= D3DXVECTOR3(-1.f, -1.f, 0.f);
	pVtxTex[3].vTex		= D3DXVECTOR2(0.f, 1.f);

	m_pVB->Unlock();

	INDEX16*	pIndex = NULL;

	m_pIB->Lock(0, 0 , (void**)&pIndex, 0);

	pIndex[0]._1 = 0;
	pIndex[0]._2 = 1;
	pIndex[0]._3 = 2;

	pIndex[1]._1 = 0;
	pIndex[1]._2 = 2;
	pIndex[1]._3 = 3;

	m_pIB->Unlock();

	return S_OK;
}

ENGINE::CResources* ENGINE::CRcUI::CloneResource(void)
{
	++m_wRefCnt;

	return new CRcUI(*this);
}


