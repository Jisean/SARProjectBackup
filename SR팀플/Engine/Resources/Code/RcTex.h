#ifndef RcTex_h__
#define RcTex_h__

#include "VIBuffer.h"

BEGIN(ENGINE)

class ENGINE_DLL CRcTex : public CVIBuffer
{
public:
	HRESULT	CreateBuffer(void);
	static CRcTex* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	void	Release(void);

public:
	virtual void Render();
	virtual CResources* CloneResource(void);

private:
	explicit CRcTex(LPDIRECT3DDEVICE9 pDevice);
public:
	virtual ~CRcTex(void);
};


END
#endif // RcTex_h__
