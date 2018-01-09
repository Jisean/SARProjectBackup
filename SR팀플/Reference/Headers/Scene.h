/*!
 * \file Scene.h
 * \date 2016/01/06 14:38
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

#ifndef Scene_h__
#define Scene_h__

#include "Engine_Include.h"

BEGIN(ENGINE)

class CLayer;
class CGameObject;
class ENGINE_DLL CScene
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9	pDevice);

public:
	virtual ~CScene(void);

public:
	virtual HRESULT	Initialize(void);
	virtual void Update(void);
	virtual void Render(void);

	const VTXTEX* GetTerrainVertex(const WORD& LayerID, const wstring& wstrObjKey);
	const VTXCUBE*	GetCubeVertex(const WORD& LayerID, const wstring&	wstrObjKey);
	const D3DXVECTOR3* GetPos(const WORD& LayerID, const wstring&	wstrObjKey);
	list<CGameObject*>*	GetList(const WORD& LayerID, const wstring&	wstrObjKey);

public:
	void	Release(void);

protected:
	LPDIRECT3DDEVICE9				m_pDevice;
	typedef map<WORD, CLayer*>		MAPLAYER;
	MAPLAYER						m_mapLayer;

};

END
#endif // Scene_h__