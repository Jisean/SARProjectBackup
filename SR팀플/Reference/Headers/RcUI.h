/*!
 * \file RcUI.h
 * \date 2016/02/10 16:02
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
#ifndef RcUI_h__
#define RcUI_h__

#include "VIBuffer.h"

BEGIN(ENGINE)

class ENGINE_DLL CRcUI : public CVIBuffer
{
public:
	HRESULT	CreateBuffer(void);
	static CRcUI* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	void	Release(void);

public:
	virtual void Render();
	virtual CResources* CloneResource(void);

private:
	explicit CRcUI(LPDIRECT3DDEVICE9 pDevice);
public:
	virtual ~CRcUI(void);
};


END

#endif // RcUI_h__