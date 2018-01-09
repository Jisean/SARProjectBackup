/*!
 * \file TerrainCol.h
 * \date 2016/01/25 14:35
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

#ifndef TerrainCol_h__
#define TerrainCol_h__

#include "Collision.h"

class CTerrainCol
	: public ENGINE::CCollision
{
private:
	CTerrainCol(void);
public:
	virtual ~CTerrainCol(void);

public:
	virtual void Update(void);
	virtual ENGINE::CCollision*	Clone(void);

public:
	static CTerrainCol*	Create(void);
	
	void   SetColInfo(D3DXVECTOR3* pPos, 
		const ENGINE::VTXTEX* pTerrainVtx);

	DWORD   Release(void);

private:
	D3DXVECTOR3*				m_pPos;
	const ENGINE::VTXTEX*		m_pTerrainVtx;

};
#endif // TerrainCol_h__