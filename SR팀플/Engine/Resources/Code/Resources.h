/*!
 * \file Resources.h
 * \date 2016/01/13 13:25
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

#ifndef Resources_h__
#define Resources_h__

#include "component.h"

BEGIN(ENGINE)

class ENGINE_DLL CResources : public CComponent
{
protected:
	static WORD					m_wRefCnt;

protected:
	LPDIRECT3DDEVICE9		m_pDevice;

public:
	virtual void Render() {};
	virtual CResources* CloneResource(void)PURE;

protected:
	void Release(void);

public:
	explicit CResources(LPDIRECT3DDEVICE9 pDevice);
public:
	virtual ~CResources();
};	
END

#endif // Resources_h__