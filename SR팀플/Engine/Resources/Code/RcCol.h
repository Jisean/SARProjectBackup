/*!
 * \file RcCol.h
 * \date 2016/01/12 13:27
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

#ifndef RcCol_h__
#define RcCol_h__

#include "VIBuffer.h"

BEGIN(ENGINE)

class ENGINE_DLL CRcCol
	: public CVIBuffer
{
public:
	virtual HRESULT		CreateBuffer(void);
	virtual CResources* CloneResource(void);
public:
	static CRcCol*		Create(LPDIRECT3DDEVICE9 pDevice);

public:
	void	Render();
	void	Release(void);
	

private:
	explicit CRcCol(LPDIRECT3DDEVICE9 pDevice);
	
public:
	~CRcCol();

};

END
#endif // RcCol_h__
