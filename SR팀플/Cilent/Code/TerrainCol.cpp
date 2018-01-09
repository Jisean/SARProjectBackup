#include "stdafx.h"
#include "TerrainCol.h"

#include "Export_Function.h"
#include "Include.h"

CTerrainCol::CTerrainCol(void)
{

}

CTerrainCol::~CTerrainCol(void)
{
	Release();
}

void CTerrainCol::Update(void)
{
	//int		iIndex = ((int)m_pPos->z / VTXITV)	* VTXCNTX + (int(m_pPos->x) / VTXITV);

	//float	fRatioX = (m_pPos->x -m_pTerrainVtx[iIndex + VTXCNTX].vPos.x) / VTXITV;
	//float	fRatioZ = (m_pTerrainVtx[iIndex + VTXCNTX].vPos.z - m_pPos->z)  / VTXITV;

	//D3DXPLANE	Plane;

	//// 오른쪽 위 폴리곤
	//if(fRatioX > fRatioZ)
	//{
	//	// 3 개의 점으로부터 평면을 생성 한다.

	//	D3DXPlaneFromPoints(&Plane, &m_pTerrainVtx[iIndex + VTXCNTX].vPos, 
	//		&m_pTerrainVtx[iIndex + VTXCNTX + 1].vPos,
	//		&m_pTerrainVtx[iIndex + 1].vPos);
	//}
	////왼쪽 아래 폴리곤
	//else
	//{
	//	D3DXPlaneFromPoints(&Plane, &m_pTerrainVtx[iIndex + VTXCNTX].vPos, 
	//		&m_pTerrainVtx[iIndex + 1].vPos,
	//		&m_pTerrainVtx[iIndex].vPos);
	//}

	//// 평면의 방정식을 적용
	//// ax + by + cz + d = 0;
	//// y = (-ax - cz -d) / b

	//m_pPos->y = (-Plane.a * m_pPos->x - Plane.c * m_pPos->z - Plane.d) / Plane.b;
	//m_pPos->y += 1.f;

}

ENGINE::CCollision* CTerrainCol::Clone(void)
{
	++m_dwRefCnt;

	return new CTerrainCol(*this);
}

CTerrainCol* CTerrainCol::Create(void)
{
	return new CTerrainCol;
}

void CTerrainCol::SetColInfo(D3DXVECTOR3* pPos, const ENGINE::VTXTEX* pTerrainVtx)
{
	m_pPos			= pPos;
	m_pTerrainVtx	= pTerrainVtx;


	int		iIndex = ((int)m_pPos->z / VTXITV)	* VTXCNTX + (int(m_pPos->x) / VTXITV);

	float	fRatioX = (m_pPos->x -m_pTerrainVtx[iIndex + VTXCNTX].vPos.x) / VTXITV;
	float	fRatioZ = (m_pTerrainVtx[iIndex + VTXCNTX].vPos.z - m_pPos->z)  / VTXITV;

	D3DXPLANE	Plane;

	// 오른쪽 위 폴리곤
	if(fRatioX > fRatioZ)
	{
		// 3 개의 점으로부터 평면을 생성 한다.

		D3DXPlaneFromPoints(&Plane, &m_pTerrainVtx[iIndex + VTXCNTX].vPos, 
			&m_pTerrainVtx[iIndex + VTXCNTX + 1].vPos,
			&m_pTerrainVtx[iIndex + 1].vPos);
	}
	//왼쪽 아래 폴리곤
	else
	{
		D3DXPlaneFromPoints(&Plane, &m_pTerrainVtx[iIndex + VTXCNTX].vPos, 
			&m_pTerrainVtx[iIndex + 1].vPos,
			&m_pTerrainVtx[iIndex].vPos);
	}

	// 평면의 방정식을 적용
	// ax + by + cz + d = 0;
	// y = (-ax - cz -d) / b

	m_pPos->y = (-Plane.a * m_pPos->x - Plane.c * m_pPos->z - Plane.d) / Plane.b;
	m_pPos->y += 1.f;

}

DWORD CTerrainCol::Release(void)
{
	if(m_dwRefCnt == 0)
	{

	}
	else
		--m_dwRefCnt;

	return m_dwRefCnt;
}

