#include "stdafx.h"
#include "MouseCol.h"

#include "Export_Function.h"
#include "Include.h"
#include "CameraObserver.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMouseCol::CMouseCol(void)
: m_pInfoSubject(ENGINE::Get_InfoSubject())
, m_pCamObserver(NULL)
{

}

CMouseCol::~CMouseCol(void)
{
	Release();
}
ENGINE::CCollision* CMouseCol::Clone(void)
{
	++m_dwRefCnt;

	return new CMouseCol(*this);
}
CMouseCol* CMouseCol::Create(void)
{
	CMouseCol* pMouseCol = new CMouseCol;
	if(FAILED(pMouseCol->Initialize()))
		ENGINE::Safe_Delete(pMouseCol);

	return pMouseCol;
}
HRESULT CMouseCol::Initialize(void)
{
	m_pCamObserver = CCameraObserver::Create();
	m_pInfoSubject->Subscribe(m_pCamObserver);

	return S_OK;
}

DWORD CMouseCol::Release(void)
{
	if(m_dwRefCnt == 0)
	{
		ENGINE::Safe_Delete(m_pCamObserver);
	}
	else
		--m_dwRefCnt;

	return m_dwRefCnt;
}

void CMouseCol::Translation_ViewSpace(void)
{
	POINT	ptMouse = GetMousePos();

	D3DXMATRIX	matProj = *m_pCamObserver->GetProj();
	D3DXVECTOR3	vTemp;

	// 뷰 포트 영역에서 투영 역으로 마우스 변환
	vTemp.x = ((float(ptMouse.x) / (WINCX >> 1)) - 1.f) / matProj._11;
	vTemp.y = ((float(-ptMouse.y) / (WINCY >> 1)) + 1.f) / matProj._22;
	vTemp.z = 1.f;

	m_vPivotPos = D3DXVECTOR3(0.f, 0.f, 0.f);

	m_vRayDir = vTemp - m_vPivotPos;

	D3DXVec3Normalize(&m_vRayDir, &m_vRayDir);

	/*D3DXMatrixInverse(&matProj, 0, &matProj);
	D3DXVec3TransformCoord(&ptMouse, &ptMouse, &matProj);*/

	/*ptMouse.x /= matProj._11;
	ptMouse.y /= matProj._22;*/
	
}

POINT CMouseCol::GetMousePos(void)
{
	POINT		ptMouse;

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	return ptMouse;
}

void CMouseCol::Translation_Local(const D3DXMATRIX*	pmatWolrd)
{
	D3DXMATRIX		matView = *m_pCamObserver->GetView();
	D3DXMatrixInverse(&matView, 0, &matView);
	D3DXVec3TransformCoord(&m_vPivotPos, &m_vPivotPos, &matView);
	D3DXVec3TransformNormal(&m_vRayDir, &m_vRayDir, &matView);

	D3DXMATRIX		matWorld;
	D3DXMatrixInverse(&matWorld, 0, pmatWolrd);
	D3DXVec3TransformCoord(&m_vPivotPos, &m_vPivotPos, &matWorld);
	D3DXVec3TransformNormal(&m_vRayDir, &m_vRayDir, &matWorld);

}

bool CMouseCol::PickTerrain(D3DXVECTOR3* pOut, const ENGINE::VTXTEX* pTerrainVtx)
{
	Translation_ViewSpace();

	D3DXMATRIX		matIdentity;
	D3DXMatrixIdentity(&matIdentity);
	Translation_Local(&matIdentity); //지형을 월드행렬로 만들면서 이동을 하지않았으므로 항등행렬을 넣어준 것이다.

	const ENGINE::VTXTEX* pVertex = pTerrainVtx;

	float	fU, fV, fDist;

	for(int z = 0; z < VTXCNTZ - 1; ++z)
	{
		for(int x = 0; x < VTXCNTX - 1; ++x)
		{
			int		iIndex = z * VTXCNTX + x;

			// 오른쪽 위 폴리곤
			if(D3DXIntersectTri(&pVertex[iIndex + VTXCNTX + 1].vPos, 
				&pVertex[iIndex + VTXCNTX].vPos, 
				&pVertex[iIndex + 1].vPos, 
				&m_vPivotPos, &m_vRayDir, 
				&fU, &fV, &fDist))
			{
				*pOut	 = pVertex[iIndex + VTXCNTX + 1].vPos + 
					(pVertex[iIndex + VTXCNTX].vPos - pVertex[iIndex + VTXCNTX + 1].vPos) * fU +
					(pVertex[iIndex + 1].vPos - pVertex[iIndex + VTXCNTX + 1].vPos) * fV;
				
				return true;
			}

			// 
			if(D3DXIntersectTri(&pVertex[iIndex].vPos, 
				&pVertex[iIndex + VTXCNTX].vPos, 
				&pVertex[iIndex + 1].vPos, 
				&m_vPivotPos, &m_vRayDir, 
				&fU, &fV, &fDist))
			{
				*pOut	 = pVertex[iIndex].vPos + 
					(pVertex[iIndex + 1].vPos - pVertex[iIndex].vPos) * fU +
					(pVertex[iIndex + VTXCNTX].vPos - pVertex[iIndex].vPos) * fV;

				return true;
			}
		}
	}
	return false;
}

bool CMouseCol::PickObject(D3DXVECTOR3* pOut, 
						   const ENGINE::VTXCUBE* pVertex, 
						   const D3DXMATRIX* pmatWorld)
{
	Translation_ViewSpace();
	Translation_Local(pmatWorld);

	const ENGINE::VTXCUBE* pVertex2 = pVertex;

	float	fU, fV, fDist;
	/////////////////////////////////////////////////
	// 오른쪽 위 폴리곤
	if(D3DXIntersectTri(&pVertex2[1].vPos, 
				&pVertex2[0].vPos, 
				&pVertex2[2].vPos, 
				&m_vPivotPos, &m_vRayDir, 
				&fU, &fV, &fDist))
	{
		*pOut	 = pVertex2[1].vPos + 
		(pVertex2[0].vPos - pVertex2[1].vPos) * fU +
		(pVertex2[2].vPos - pVertex2[1].vPos) * fV;

			return true;
	}
	// 왼쪽 아래 폴리곤
	if(D3DXIntersectTri(&pVertex2[3].vPos, 
				&pVertex2[2].vPos, 
				&pVertex2[0].vPos, 
				&m_vPivotPos, &m_vRayDir, 
				&fU, &fV, &fDist))
	{
		*pOut	 = pVertex2[3].vPos + 
		(pVertex2[2].vPos - pVertex2[3].vPos) * fU +
		(pVertex2[0].vPos - pVertex2[3].vPos) * fV;

			return true;
	}
	/////////////////////////정면///////////////////

	/////////////////////////////////////////////////
	// 오른쪽 위 폴리곤
	if(D3DXIntersectTri(&pVertex2[5].vPos, 
				&pVertex2[1].vPos, 
				&pVertex2[6].vPos, 
				&m_vPivotPos, &m_vRayDir, 
				&fU, &fV, &fDist))
	{
		*pOut	 = pVertex2[5].vPos + 
		(pVertex2[1].vPos - pVertex2[5].vPos) * fU +
		(pVertex2[6].vPos - pVertex2[5].vPos) * fV;

			return true;
	}
	// 왼쪽 아래 폴리곤
	if(D3DXIntersectTri(&pVertex2[2].vPos, 
				&pVertex2[6].vPos, 
				&pVertex2[1].vPos, 
				&m_vPivotPos, &m_vRayDir, 
				&fU, &fV, &fDist))
	{
		*pOut	 = pVertex2[2].vPos + 
		(pVertex2[6].vPos - pVertex2[2].vPos) * fU +
		(pVertex2[1].vPos - pVertex2[2].vPos) * fV;

			return true;
	}
	/////////////////////////오른쪽면///////////////////

	/////////////////////////////////////////////////
	// 오른쪽 위 폴리곤
	if(D3DXIntersectTri(&pVertex2[0].vPos, 
				&pVertex2[4].vPos, 
				&pVertex2[3].vPos, 
				&m_vPivotPos, &m_vRayDir, 
				&fU, &fV, &fDist))
	{
		*pOut	 = pVertex2[0].vPos + 
		(pVertex2[4].vPos - pVertex2[0].vPos) * fU +
		(pVertex2[3].vPos - pVertex2[0].vPos) * fV;

			return true;
	}
	// 왼쪽 아래 폴리곤
	if(D3DXIntersectTri(&pVertex2[7].vPos, 
				&pVertex2[3].vPos, 
				&pVertex2[4].vPos, 
				&m_vPivotPos, &m_vRayDir, 
				&fU, &fV, &fDist))
	{
		*pOut	 = pVertex2[7].vPos + 
		(pVertex2[3].vPos - pVertex2[7].vPos) * fU +
		(pVertex2[4].vPos - pVertex2[7].vPos) * fV;

			return true;
	}
	/////////////////////////왼쪽면///////////////////

	/////////////////////////////////////////////////
	// 오른쪽 위 폴리곤
	if(D3DXIntersectTri(&pVertex2[4].vPos, 
				&pVertex2[5].vPos, 
				&pVertex2[7].vPos, 
				&m_vPivotPos, &m_vRayDir, 
				&fU, &fV, &fDist))
	{
		*pOut	 = pVertex2[4].vPos + 
		(pVertex2[5].vPos - pVertex2[4].vPos) * fU +
		(pVertex2[7].vPos - pVertex2[4].vPos) * fV;

			return true;
	}
	// 왼쪽 아래 폴리곤
	if(D3DXIntersectTri(&pVertex2[6].vPos, 
				&pVertex2[7].vPos, 
				&pVertex2[5].vPos, 
				&m_vPivotPos, &m_vRayDir, 
				&fU, &fV, &fDist))
	{
		*pOut	 = pVertex2[6].vPos + 
		(pVertex2[7].vPos - pVertex2[6].vPos) * fU +
		(pVertex2[5].vPos - pVertex2[6].vPos) * fV;

			return true;
	}
	/////////////////////////뒷면///////////////////

	/////////////////////////////////////////////////
	// 오른쪽 위 폴리곤
	if(D3DXIntersectTri(&pVertex2[5].vPos, 
				&pVertex2[4].vPos, 
				&pVertex2[1].vPos, 
				&m_vPivotPos, &m_vRayDir, 
				&fU, &fV, &fDist))
	{
		*pOut	 = pVertex2[5].vPos + 
		(pVertex2[4].vPos - pVertex2[5].vPos) * fU +
		(pVertex2[1].vPos - pVertex2[5].vPos) * fV;

			return true;
	}
	// 왼쪽 아래 폴리곤
	if(D3DXIntersectTri(&pVertex2[0].vPos, 
				&pVertex2[1].vPos, 
				&pVertex2[4].vPos, 
				&m_vPivotPos, &m_vRayDir, 
				&fU, &fV, &fDist))
	{
		*pOut	 = pVertex2[0].vPos + 
		(pVertex2[1].vPos - pVertex2[0].vPos) * fU +
		(pVertex2[4].vPos - pVertex2[0].vPos) * fV;

			return true;
	}
	/////////////////////////윗면///////////////////

	/////////////////////////////////////////////////
	// 오른쪽 위 폴리곤
	if(D3DXIntersectTri(&pVertex2[2].vPos, 
				&pVertex2[3].vPos, 
				&pVertex2[6].vPos, 
				&m_vPivotPos, &m_vRayDir, 
				&fU, &fV, &fDist))
	{
		*pOut	 = pVertex2[2].vPos + 
		(pVertex2[3].vPos - pVertex2[2].vPos) * fU +
		(pVertex2[6].vPos - pVertex2[2].vPos) * fV;

			return true;
	}
	// 왼쪽 아래 폴리곤
	if(D3DXIntersectTri(&pVertex2[7].vPos, 
				&pVertex2[6].vPos, 
				&pVertex2[3].vPos, 
				&m_vPivotPos, &m_vRayDir, 
				&fU, &fV, &fDist))
	{
		*pOut	 = pVertex2[7].vPos + 
		(pVertex2[6].vPos - pVertex2[7].vPos) * fU +
		(pVertex2[3].vPos - pVertex2[7].vPos) * fV;

			return true;
	}
	/////////////////////////아랫면///////////////////

	int a = 10;
	return false;
}