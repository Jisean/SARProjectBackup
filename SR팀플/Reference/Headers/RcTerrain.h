#ifndef RcTerrain_h__
#define RcTerrain_h__

#include "VIBuffer.h"

BEGIN(ENGINE)

class ENGINE_DLL	CRcTerrain
	:  public CVIBuffer
{
public:
	virtual HRESULT CreateBuffer(const WORD& wCntX, const WORD& wCntZ, const WORD& wItv);
	virtual void Release(void);

public:
	static CRcTerrain*	Create(LPDIRECT3DDEVICE9 pDevice, const WORD& wCntX, 
		const WORD& wCntZ, const WORD& wItv);
	virtual CResources* CloneResource(void);

private:
	explicit CRcTerrain(LPDIRECT3DDEVICE9 pDevice);
public:
	virtual ~CRcTerrain(void);
};

END
#endif // RcTerrain_h__
