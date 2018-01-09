/*!
 * \file Collision.h
 * \date 2016/01/25 14:29
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

#ifndef Collision_h__
#define Collision_h__

#include "Component.h"

BEGIN(ENGINE)

class ENGINE_DLL CCollision : public CComponent
{
protected:
	CCollision(void);
public:
	virtual ~CCollision(void);

public:
	virtual CCollision*	Clone(void)PURE;
	DWORD GetRefCnt(void);

public:
	void AddRef(void);
	virtual DWORD Release(void)PURE;

protected:
	DWORD			m_dwRefCnt;

};

END
#endif // Collision_h__