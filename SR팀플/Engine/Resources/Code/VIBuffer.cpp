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

	// 1인자 : 인덱 버퍼의 사이즈 
	// 2인자 : 정점을 사용하는 방식 플래그, 0인 경우 정적으로 버퍼를 할당함
	// 3인자 : 인덱스 버퍼의 크기 포맷
	// 4인자 : 메모리 풀
	// 5인자 : 인덱스를 저장할 객체의 주소값

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

	// 2인자 : 버텍스의 인덱스 값(몇 번 인덱스부터 그릴 것인가?)
	// 3인자 : 인덱스 버퍼에 들어가 있는 인덱스 중 몇 번부터 그릴 것인가?
	// 4인자 : 버텍스 개수
	// 5인자 : 인덱스 버퍼에 들어가 있는 인덱스 중 그리고자하는 시작점
	// 6인자 : 삼각형 개수
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



