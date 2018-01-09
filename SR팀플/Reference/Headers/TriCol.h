/*!
 * \file TriCol.h
 * \date 2016/01/08 14:03
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

#ifndef TriCol_h__
#define TriCol_h__

#include "VIBuffer.h"

BEGIN(ENGINE)

class ENGINE_DLL CTriCol
	: public CVIBuffer
{


public:
	virtual HRESULT	CreateBuffer(void);
	virtual void	Render(void);
	virtual CResources* CloneResource(void);
public:
	static CTriCol*	Create(LPDIRECT3DDEVICE9 pDevice);

private:
	void	Release(void);

private:
	explicit CTriCol(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CTriCol(void);

};

END
#endif // TriCol_h__