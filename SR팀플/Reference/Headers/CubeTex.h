/*!
 * \file CubeTex.h
 * \date 2016/01/27 13:13
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
#ifndef CubeTex_h__
#define CubeTex_h__

#include "VIBuffer.h"

BEGIN(ENGINE)

class ENGINE_DLL CCubeTex : public CVIBuffer
{
public:
	HRESULT		CreateBuffer(void);
	static		CCubeTex*	Create(LPDIRECT3DDEVICE9	pDevice);

public:
	virtual void Render(void);
	virtual CResources* CloneResource(void);

private:
	explicit	CCubeTex(LPDIRECT3DDEVICE9	pDevice);
public:
	virtual ~CCubeTex(void);
};

END
#endif // CubeTex_h__
