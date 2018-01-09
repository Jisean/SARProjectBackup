#include "stdafx.h"
#include "MouseCol.h"

#include "Export_Function.h"
#include "Include.h"
#include "CameraObserver.h"

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
	m_pInfoSubject->UnSubscribe(m_pCamObserver);

	if(m_dwRefCnt == 0)
	{

	}
	else
		--m_dwRefCnt;

	return m_dwRefCnt;
	
}

void CMouseCol::Translation_ViewSpace(void)
{
	POINT	ptMouse = GetMousePos();
	ptMouse.x = 400;
	ptMouse.y = 300;

	D3DXMATRIX	matProj = *m_pCamObserver->GetProj();
	D3DXVECTOR3	vTemp;

	// ºä Æ÷Æ® ¿µ¿ª¿¡¼­ Åõ¿µ ¿ªÀ¸·Î ¸¶¿ì½º º¯È¯
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

void CMouseCol::PickTerrain(D3DXVECTOR3* pOut, const ENGINE::VTXTEX* pTerrainVtx)
{
	Translation_ViewSpace();

	D3DXMATRIX		matIdentity;
	D3DXMatrixIdentity(&matIdentity);
	Translation_Local(&matIdentity);

	const ENGINE::VTXTEX* pVertex = pTerrainVtx;

	float	fU, fV, fDist;

	for(int z = 0; z < VTXCNTZ - 1; ++z)
	{
		for(int x = 0; x < VTXCNTX - 1; ++x)
		{
			int		iIndex = z * VTXCNTX + x;

			// ¿À¸¥ÂÊ À§ Æú¸®°ï
			if(D3DXIntersectTri(&pVertex[iIndex + VTXCNTX + 1].vPos, 
				&pVertex[iIndex + VTXCNTX].vPos, 
				&pVertex[iIndex + 1].vPos, 
				&m_vPivotPos, &m_vRayDir, 
				&fU, &fV, &fDist))
			{
				*pOut	 = pVertex[iIndex + VTXCNTX + 1].vPos + 
					(pVertex[iIndex + VTXCNTX].vPos - pVertex[iIndex + VTXCNTX + 1].vPos) * fU +
					(pVertex[iIndex + 1].vPos - pVertex[iIndex + VTXCNTX + 1].vPos) * fV;
				
				return;
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

				return;
			}
		}
	}

}

bool CMouseCol::PickObject(D3DXVECTOR3* pOut, 
						   const ENGINE::VTXTEX* pVertex, 
						   const D3DXMATRIX* pmatWorld)
{
	Translation_ViewSpace();
	Translation_Local(pmatWorld);

	const ENGINE::VTXTEX* pVertex2 = pVertex;

	float	fU, fV, fDist;

	// ¿À¸¥ÂÊ À§ Æú¸®°ï
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
	// ¿ÞÂÊ ¾Æ·¡ Æú¸®°ï
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
	return false;
}


bool CMouseCol::PickCube(D3DXVECTOR3* pOut, 
						   const ENGINE::VTXCUBE* pVertex, 
						   const D3DXMATRIX* pmatWorld)
{
	Translation_ViewSpace();
	Translation_Local(pmatWorld);

	const ENGINE::VTXCUBE* pVertex2 = pVertex;

	float	fU, fV, fDist;
	/////////////////////////////////////////////////
	// ¿À¸¥ÂÊ À§ Æú¸®°ï
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
	// ¿ÞÂÊ ¾Æ·¡ Æú¸®°ï
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
	/////////////////////////Á¤¸é///////////////////

	/////////////////////////////////////////////////
	// ¿À¸¥ÂÊ À§ Æú¸®°ï
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
	// ¿ÞÂÊ ¾Æ·¡ Æú¸®°ï
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
	/////////////////////////¿À¸¥ÂÊ¸é///////////////////

	/////////////////////////////////////////////////
	// ¿À¸¥ÂÊ À§ Æú¸®°ï
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
	// ¿ÞÂÊ ¾Æ·¡ Æú¸®°ï
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
	/////////////////////////¿ÞÂÊ¸é///////////////////

	/////////////////////////////////////////////////
	// ¿À¸¥ÂÊ À§ Æú¸®°ï
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
	// ¿ÞÂÊ ¾Æ·¡ Æú¸®°ï
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
	/////////////////////////µÞ¸é///////////////////

	/////////////////////////////////////////////////
	// ¿À¸¥ÂÊ À§ Æú¸®°ï
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
	// ¿ÞÂÊ ¾Æ·¡ Æú¸®°ï
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
	/////////////////////////À­¸é///////////////////

	/////////////////////////////////////////////////
	// ¿À¸¥ÂÊ À§ Æú¸®°ï
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
	// ¿ÞÂÊ ¾Æ·¡ Æú¸®°ï
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
	/////////////////////////¾Æ·§¸é///////////////////

	int a = 10;
	return false;
}


