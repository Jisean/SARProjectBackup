#include "MathMgr.h"

ENGINE::CMathMgr::CMathMgr(void)
{

}

ENGINE::CMathMgr::~CMathMgr()
{

}

void ENGINE::CMathMgr::MyRotationX(D3DXVECTOR3* pOut, D3DXVECTOR3* pIn, float fAngle)
{ /*
	1	0	0	0
	0	cos	sin	0
	0  -sin cos	0
	0	0	0	1*/

	D3DXVECTOR3		vTemp = *pIn;

	pOut->x = vTemp.x;
	pOut->y = vTemp.y * cosf(fAngle) - vTemp.z * sinf(fAngle);
	pOut->z = vTemp.y * sinf(fAngle) + vTemp.z * cosf(fAngle);	
	
}	

void ENGINE::CMathMgr::MyRotationY(D3DXVECTOR3* pOut, D3DXVECTOR3* pIn, float fAngle)
{ /*
	cos	0 -sin	0
	0	1	0	0
	sin	0  cos	0
	0	0	0	1*/

	D3DXVECTOR3		vTemp = *pIn;

	pOut->x = vTemp.x * cosf(fAngle) + vTemp.z * sinf(fAngle);
	pOut->y = vTemp.y;
	pOut->z = vTemp.x * -sinf(fAngle) + vTemp.z * cosf(fAngle);	
}

void ENGINE::CMathMgr::MyRotationZ(D3DXVECTOR3* pOut, D3DXVECTOR3* pIn, float fAngle)
{ /*
	cos	sin	0	0
   -sin cos	0	0
	0	0	1	0
	0	0	0	1*/

	D3DXVECTOR3		vTemp = *pIn;

	pOut->x = vTemp.x * cosf(fAngle) - vTemp.y * sinf(fAngle);
	pOut->y = vTemp.x * sinf(fAngle) + vTemp.y * cosf(fAngle);
	pOut->z = vTemp.z ;	
}

