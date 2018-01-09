/*!
 * \file MathMgr.cpp
 * \date 2016/01/20 14:27
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef MathMgr_h__
#define MathMgr_h__

#include "Engine_Include.h"

BEGIN(ENGINE)

class ENGINE_DLL CMathMgr
{
private:
	CMathMgr(void);
	~CMathMgr();

public:
	static void MyRotationX(D3DXVECTOR3* pOut, D3DXVECTOR3* pIn, float fAngle);
	static void MyRotationY(D3DXVECTOR3* pOut, D3DXVECTOR3* pIn, float fAngle);
	static void MyRotationZ(D3DXVECTOR3* pOut, D3DXVECTOR3* pIn, float fAngle);
};
END
#endif // MathMgr_h__