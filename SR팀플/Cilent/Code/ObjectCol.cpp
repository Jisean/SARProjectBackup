#include "stdafx.h"
#include "ObjectCol.h"

#include "Export_Function.h"
#include "Include.h"


CObjectCol::CObjectCol(void)
{

}

CObjectCol::~CObjectCol(void)
{
	Release();
}

//void CObjectCol::Update(void)
//{
//	
//
//}

ENGINE::CCollision* CObjectCol::Clone(void)
{
	++m_dwRefCnt;

	return new CObjectCol(*this);
}

CObjectCol* CObjectCol::Create(void)
{
	return new CObjectCol;
}


DWORD CObjectCol::Release(void)
{
	if(m_dwRefCnt == 0)
	{

	}
	else
		--m_dwRefCnt;

	return m_dwRefCnt;
}

D3DXVECTOR3 CObjectCol::GetMaxPos(const ENGINE::VTXCUBE* pCube)
{
	D3DXVECTOR3	vMax;

	float x = pCube[0].vPos.x;
	float y = pCube[0].vPos.y;
	float z = pCube[0].vPos.z;

	for(int i = 0; i != 8; ++i)
	{
		if(pCube[i].vPos.x > x)
			x = pCube[i].vPos.x;

		if(pCube[i].vPos.y > y)
			y = pCube[i].vPos.y;

		if(pCube[i].vPos.z > z)
			z = pCube[i].vPos.z;
	}
	vMax.x = x;
	vMax.y = y;
	vMax.z = z;

	return vMax;

}

D3DXVECTOR3 CObjectCol::GetMinPos(const ENGINE::VTXCUBE* pCube)
{

	D3DXVECTOR3	vMin;

	float x = pCube[0].vPos.x;
	float y = pCube[0].vPos.y;
	float z = pCube[0].vPos.z;

	for(int i = 0; i != 8; ++i)
	{
		if(pCube[i].vPos.x < x)
			x = pCube[i].vPos.x;

		if(pCube[i].vPos.y < y)
			y = pCube[i].vPos.y;

		if(pCube[i].vPos.z < z)
			z = pCube[i].vPos.z;
	}

	vMin.x = x;
	vMin.y = y;
	vMin.z = z;

	return vMin;
}

bool CObjectCol::SetColCheck(const ENGINE::VTXCUBE* pSelf, const ENGINE::VTXCUBE* pTarget)
{
	D3DXVECTOR3 SelfMax, SelfMin;
	D3DXVECTOR3 TargetMax, TargetMin;

	SelfMax = GetMaxPos(pSelf);
	SelfMin = GetMinPos(pSelf);

	TargetMax = GetMaxPos(pTarget);
	TargetMin = GetMinPos(pTarget);

	// x 
	if(SelfMin.x > TargetMax.x || SelfMax.x < TargetMin.x)
	{
		return false;
	}

	// y
	if(SelfMin.y > TargetMax.y || SelfMax.y < TargetMin.y)
	{
		return false;
	}

	// z
	if(SelfMin.z > TargetMax.z || SelfMax.z < TargetMin.z)
	{
		return false;
	}

	return true;
}

bool CObjectCol::SetColCheck2(D3DXVECTOR3* pselfMax,D3DXVECTOR3* pselfMin, D3DXVECTOR3* pTargetMax,D3DXVECTOR3* pTargetMin)
{
	if(pselfMin->x > pTargetMax->x || pselfMax->x < pTargetMin->x)
	{
		return false;
	}

	if(pselfMin->y > pTargetMax->y || pselfMax->y < pTargetMin->y)
	{
		return false;
	}

	if(pselfMin->z > pTargetMax->z || pselfMax->z < pTargetMin->z)
	{
		return false;
	}
	
	return true;
}

