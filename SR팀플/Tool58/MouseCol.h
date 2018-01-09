/*!
 * \file MouseCol.h
 * \date 2016/01/27 14:41
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

#ifndef MouseCol_h__
#define MouseCol_h__

#include "Collision.h"

namespace ENGINE
{
	class CInfoSubject;
}
class CCameraObserver;
class CMouseCol : public ENGINE::CCollision
{
private:
	CCameraObserver*			m_pCamObserver;
	D3DXVECTOR3					m_vPivotPos;
	D3DXVECTOR3					m_vRayDir;
	ENGINE::CInfoSubject*		m_pInfoSubject;

private:
	CMouseCol(void);
public:
	virtual ~CMouseCol(void);

public:
	void	Translation_ViewSpace(void);
	void	Translation_Local(const D3DXMATRIX*	pmatWolrd);

public:
	static POINT	GetMousePos(void);
	static CMouseCol*	Create(void);
	virtual ENGINE::CCollision*	Clone(void);

	bool PickTerrain(D3DXVECTOR3* pOut, const ENGINE::VTXTEX* pTerrainVtx);
	bool PickObject(D3DXVECTOR3* pOut, const ENGINE::VTXCUBE* pVertex, const D3DXMATRIX* pmatWorld);

private:
	HRESULT	Initialize(void);
	DWORD	Release(void);
};


#endif // MouseCol_h__