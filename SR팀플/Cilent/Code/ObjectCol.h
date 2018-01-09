/*!
 * \file ObjectCol.h
 * \date 2016/01/30 18:45
 *
 * \author han
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef ObjectCol_h__
#define ObjectCol_h__

#include "Collision.h"

namespace ENGINE
{
	class CInfoSubject;
}

class CCameraObserver;
class CObjectCol : public ENGINE::CCollision
{

private:
	CObjectCol(void);
public:
	virtual ~CObjectCol(void);

public:
	//virtual void Update(void);
	virtual ENGINE::CCollision* Clone(void);

public:
	static	CObjectCol* Create(void);

	//void	SetCol(const ENGINE::VTXCUBE* pCube);

	D3DXVECTOR3	GetMaxPos(const ENGINE::VTXCUBE* pCube);
	D3DXVECTOR3	GetMinPos(const ENGINE::VTXCUBE* pCube);

	bool	SetColCheck(const ENGINE::VTXCUBE* pSelf, const ENGINE::VTXCUBE* pTarget);
	bool	SetColCheck2(D3DXVECTOR3* pselfMax,D3DXVECTOR3* pselfMin, D3DXVECTOR3* pTargetMax,D3DXVECTOR3* pTargetMin);


	DWORD   Release(void);

private:
	const ENGINE::VTXCUBE*		m_pCube;
};


#endif // ObjectCol_h__