/*!
 * \file Layer.h
 * \date 2016/01/18 13:46
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

#ifndef Layer_h__
#define Layer_h__

#include "GameObject.h"

BEGIN(ENGINE)

class CComponent;
class ENGINE_DLL CLayer
{
private:
	LPDIRECT3DDEVICE9			m_pDevice;

protected:
	typedef		list<CGameObject*>		OBJLIST;
	typedef		map<wstring, OBJLIST>	MAPOBJLIST;
	MAPOBJLIST							m_mapObjList;	

public:
	void Update(void);
	void Render(void);
	HRESULT	AddObject(const wstring& wstrObjKey, CGameObject* pGameObject);
	const ENGINE::VTXTEX*	GetTerrainVertex(const wstring& wstrObjKey);
	const ENGINE::VTXCUBE*	GetCubeVertex(const wstring&	wstrObjKey);
	const D3DXVECTOR3*		GetPos(const wstring&		wstrObjKey);
	list<CGameObject*>*	GetList(const wstring&	wstrObjKey);
public:
	const CComponent* GetComponent(const wstring& wstrObjKey, const wstring& wstrComponentKey);

private:
	void Release(void);

public:
	static CLayer*	Create(LPDIRECT3DDEVICE9 pDevice);
private:
	explicit CLayer(LPDIRECT3DDEVICE9 pDevice);

public:
	~CLayer();
};
END
#endif // Layer_h__