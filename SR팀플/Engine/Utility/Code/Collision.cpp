#include "Collision.h"

ENGINE::CCollision::CCollision(void)
: m_dwRefCnt(0)
{

}

ENGINE::CCollision::~CCollision(void)
{

}

void ENGINE::CCollision::AddRef(void)
{
	++m_dwRefCnt;
}

DWORD ENGINE::CCollision::GetRefCnt(void)
{
	return m_dwRefCnt;
}
