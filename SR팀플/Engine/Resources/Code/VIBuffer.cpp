#include "VIBuffer.h"

ENGINE::CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pDevice)
: CResources(pDevice)
, m_pVB(NULL)
, m_pIB(NULL)
, m_dwVtxSize(0)
, m_dwVtxCount(0)
, m_dwVtxFVF(0)
, m_dwTriCnt(0)
, m_dwIdxSize(0)
{

}

ENGINE::CVIBuffer::~CVIBuffer(void)
{
	Release();
}

HRESULT ENGINE::CVIBuffer::CreateBuffer(void)
{
	m_pDevice->CreateVertexBuffer(m_dwVtxSize * m_dwVtxCount, 0, 
		m_dwVtxFVF, D3DPOOL_MANAGED, &m_pVB, NULL);

	// 1���� : �ε� ������ ������ 
	// 2���� : ������ ����ϴ� ��� �÷���, 0�� ��� �������� ���۸� �Ҵ���
	// 3���� : �ε��� ������ ũ�� ����
	// 4���� : �޸� Ǯ
	// 5���� : �ε����� ������ ��ü�� �ּҰ�

	m_pDevice->CreateIndexBuffer(m_dwIdxSize * m_dwTriCnt, 0, 
		m_IdxFmt, D3DPOOL_MANAGED, &m_pIB, NULL);



	return S_OK;
}

void ENGINE::CVIBuffer::Render()
{
	m_pDevice->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	m_pDevice->SetFVF(m_dwVtxFVF);

	m_pDevice->SetIndices(m_pIB);

	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCount, 0 ,m_dwTriCnt);

	// 2���� : ���ؽ��� �ε��� ��(�� �� �ε������� �׸� ���ΰ�?)
	// 3���� : �ε��� ���ۿ� �� �ִ� �ε��� �� �� ������ �׸� ���ΰ�?
	// 4���� : ���ؽ� ����
	// 5���� : �ε��� ���ۿ� �� �ִ� �ε��� �� �׸������ϴ� ������
	// 6���� : �ﰢ�� ����
}

void ENGINE::CVIBuffer::Release(void)
{
	if(m_wRefCnt == 0)
	{
		ENGINE::Safe_Release(m_pIB);
		ENGINE::Safe_Release(m_pVB);
		//CResources::Release();
	}

	else
	{
		--m_wRefCnt;
	}
	
	
}

ENGINE::CResources* ENGINE::CVIBuffer::CloneResource(void)
{
	++m_wRefCnt;

	return new CVIBuffer(*this);
}

void ENGINE::CVIBuffer::GetVtxInfo(void* pVertex)
{
	void*		pOriginVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pOriginVertex, 0);
	
	memcpy(pVertex, pOriginVertex, m_dwVtxSize * m_dwVtxCount);

	m_pVB->Unlock();
}


void ENGINE::CVIBuffer::SetVtxInfo(void* pVertex)
{
	void*		pOriginVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pOriginVertex, 0);

	memcpy(pOriginVertex, pVertex, m_dwVtxSize * m_dwVtxCount);

	m_pVB->Unlock();
}



