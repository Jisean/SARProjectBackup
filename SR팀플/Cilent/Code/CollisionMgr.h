/*!
 * \file CollisionMgr.h
 * \date 2016/01/26 13:09
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

#ifndef CollisionMgr_h__
#define CollisionMgr_h__

#include "Engine_Include.h"

namespace ENGINE
{
	class CCollision;
}

class CCollisionMgr
{
public:
	DECLARE_SINGLETON(CCollisionMgr)

public:
	enum COLLISIONID { COL_TERRAIN, COL_MOUSE, COL_OBJ };

public:
	HRESULT	AddColObject(COLLISIONID eCollisionID);
	ENGINE::CCollision* CloneColObject(COLLISIONID eCollisionID);

private:
	void	Release(void);

private:
	typedef	map<COLLISIONID, ENGINE::CCollision*>	MAPCOLLISION;
	MAPCOLLISION		m_mapCollision;

private:
	CCollisionMgr(void);
	~CCollisionMgr();
};

#endif // CollisionMgr_h__