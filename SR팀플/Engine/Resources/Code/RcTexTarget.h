#ifndef RcTexTarget_h__
#define RcTexTarget_h__

#include "VIBuffer.h"

BEGIN(ENGINE)

class ENGINE_DLL CRcTexTarget : public CVIBuffer
{
public:
	HRESULT	CreateBuffer(void);
	static CRcTexTarget* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	void	Release(void);

public:
	virtual void Render();
	virtual CResources* CloneResource(void);

private:
	explicit CRcTexTarget(LPDIRECT3DDEVICE9 pDevice);
public:
	virtual ~CRcTexTarget(void);
};


END
#endif // RcTexTarget_h__
