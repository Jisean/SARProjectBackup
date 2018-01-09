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

	// 기능 1: 주소를 얻는 기능
	// 기능 2: 공간을 잠그는 기능

	m_pVB->Lock(0, 0, (void**)&pVtxCol, 0);

	// 1인자 : 어디서부터 잠글 것인가
	// 2인자 : 0인 경우 전체 영역을 다 잠그겠다. EX) sizeof(VTXCOL)
	// 3인자 : 잠근 영역의 주소를 어디에 저장할 것인가
	// 4인자 : 잠그는 형태를 묻는 인자, 정적버퍼인 경우 0을 사용

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
	// 장치에게 현재 내가 그리려는 버퍼를 연결해주는 함수
	// 장치는 이 데이터를 보관하기 위한 슬롯을 가지고 있다.

	// 1인자 : 몇 번 슬롯에 보관할 것인가?
	// 2인자 : 어떤 것을 넘겨줄 것인가?
	// 3인자 : 어디서부터 그릴 것인가? 0인 경우 버퍼의 처음부터 그려라.
	// 4인자 : 어떤 단위로 표현할 것인가?

	m_pDevice->SetFVF(m_dwVtxFVF);
	// 월요일날 설명
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