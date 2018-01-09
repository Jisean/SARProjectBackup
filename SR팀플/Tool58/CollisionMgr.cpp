#include "stdafx.h"
#include "CollisionMgr.h"
#include "Collision.h"
#include "TerrainCol.h"
#include "MouseCol.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_SINGLETON(CCollisionMgr)

CCollisionMgr::CCollisionMgr(void)
{

}

CCollisionMgr::~CCollisionMgr()
{
	Release();
}

HRESULT CCollisionMgr::AddColObject(COLLISIONID eCollisionID)
{
	MAPCOLLISION::iterator		iter = m_mapCollision.find(eCollisionID);

	if(iter != m_mapCollision.end())
	{
		TAGMSG_BOX(L"CollisionMgr", L"비어있음");
		return E_FAIL;
	}

	ENGINE::CCollision*			pCollision = NULL;

	switch(eCollisionID)
	{
	case COL_TERRAIN:
		pCollision = CTerrainCol::Create();
		break;

	case COL_MOUSE:
		pCollision = CMouseCol::Create();
		break;
	}
	NULL_CHECK_RETURN(pCollision, E_FAIL);

	m_mapCollision.insert(MAPCOLLISION::value_type(eCollisionID, pCollision));

	return S_OK;
}

ENGINE::CCollision* CCollisionMgr::CloneColObject(COLLISIONID eCollisionID)
{
	MAPCOLLISION::iterator	iter = m_mapCollision.find(eCollisionID);
	if(iter == m_mapCollision.end())
	{
		return NULL;
	}

	return iter->second->Clone();
}

void CCollisionMgr::Release(void)
{
	for_each(m_mapCollision.begin(), m_mapCollision.end(), ENGINE::CDeleteMap());
	m_mapCollision.clear();
}

