/*!
 * \file GameObject.h
 * \date 2016/01/18 14:11
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

#ifndef GameObject_h__
#define GameObject_h__

#include "Engine_Include.h"

BEGIN(ENGINE)

class CComponent;

class ENGINE_DLL CGameObject
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pDevice);
public:
	virtual ~CGameObject();

public:
	virtual HRESULT	Initialize(void);
	virtual void	Update(void);
	virtual void	Render(void);
	virtual const ENGINE::VTXTEX*	GetTerrainVertex(void);
	virtual const ENGINE::VTXCUBE*  GetCubeVertex(void);
	virtual const D3DXVECTOR3*		GetPos(void);
	const CComponent*	GetComponent(const wstring& wstrComponentKey);

private:
	virtual void		Release(void);

protected:
	LPDIRECT3DDEVICE9		m_pDevice;

protected:
	typedef map<wstring, CComponent*>		MAPCOMPONENT;
	MAPCOMPONENT							m_mapComponent;


};


END
#endif // GameObject_h__