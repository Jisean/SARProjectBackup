#include "TriCol.h"

ENGINE::CTriCol::CTriCol(LPDIRECT3DDEVICE9 pDevice)
: CVIBuffer(pDevice)
{

}

ENGINE::CTriCol::~CTriCol(void)
{
	Release();
}

ENGINE::CTriCol* ENGINE::CTriCol::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CTriCol*		pTriCol = new CTriCol(pDevice);
	
	if(FAILED(pTriCol->CreateBuffer()))
		ENGINE::Safe_Delete(pTriCol);

	return pTriCol;

}
HRESULT ENGINE::CTriCol::CreateBuffer(void)
{
	m_dwVtxSize		= sizeof(VTXCOL);
	m_dwVtxCount	= 3;
	m_dwVtxFVF		= VTXFVF_COL;
	m_dwTriCnt		= 1;
	m_dwIdxSize		= sizeof(INDEX32);
	m_IdxFmt		= D3DFMT_INDEX32;

	if(FAILED(CVIBuffer::CreateBuffer()))
		return E_FAIL;

	VTXCOL*		pVtxCol = NULL;

	// ��� 1: �ּҸ� ��� ���
	// ��� 2: ������ ��״� ���

	m_pVB->Lock(0, 0, (void**)&pVtxCol, 0);

	// 1���� : ��𼭺��� ��� ���ΰ�
	// 2���� : 0�� ��� ��ü ������ �� ��װڴ�. EX) sizeof(VTXCOL)
	// 3���� : ��� ������ �ּҸ� ��� ������ ���ΰ�
	// 4���� : ��״� ���¸� ���� ����, ���������� ��� 0�� ���

	pVtxCol[0].vPos		= D3DXVECTOR3(0.f, 1.f, 0.f);
	pVtxCol[0].dwColor	= D3DCOLOR_ARGB(255, 255, 0, 0);

	pVtxCol[1].vPos		= D3DXVECTOR3(1.f, -1.f, 0.f);
	pVtxCol[1].dwColor	= D3DCOLOR_ARGB(255, 0, 255, 0);

	pVtxCol[2].vPos		= D3DXVECTOR3(-1.f, -1.f, 0.f);
	pVtxCol[2].dwColor	= D3DCOLOR_ARGB(255, 0, 0, 255);

	m_pVB->Unlock();

	return S_OK;
}

void ENGINE::CTriCol::Render(void)
{
	m_pDevice->SetStreamSource(0, m_pVB, 0, sizeof(VTXCOL));
	// ��ġ���� ���� ���� �׸����� ���۸� �������ִ� �Լ�
	// ��ġ�� �� �����͸� �����ϱ� ���� ������ ������ �ִ�.

	// 1���� : �� �� ���Կ� ������ ���ΰ�?
	// 2���� : � ���� �Ѱ��� ���ΰ�?
	// 3���� : ��𼭺��� �׸� ���ΰ�? 0�� ��� ������ ó������ �׷���.
	// 4���� : � ������ ǥ���� ���ΰ�?

	m_pDevice->SetFVF(m_dwVtxFVF);
	// �����ϳ� ����
	m_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_dwTriCnt);
}

void ENGINE::CTriCol::Release(void)
{

}

ENGINE::CResources* ENGINE::CTriCol::CloneResource(void)
{
	++m_wRefCnt;

	return new CTriCol(*this);	
}