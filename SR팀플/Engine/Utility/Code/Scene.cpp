#include "Scene.h"
#include "Layer.h"

ENGINE::CScene::CScene(LPDIRECT3DDEVICE9 pDevice)
: m_pDevice(pDevice)
{

}

ENGINE::CScene::~CScene(void)
{
	Release();
}

HRESULT ENGINE::CScene::Initialize(void)
{
	return S_OK;
}

void ENGINE::CScene::Update(void)
{
	MAPLAYER::iterator		iter		= m_mapLayer.begin();
	MAPLAYER::iterator		iter_end	= m_mapLayer.end();

	for( ;iter != iter_end; ++iter)
	{
		iter->second->Update();
	}
}

void ENGINE::CScene::Render(void)
{
	MAPLAYER::iterator		iter		= m_mapLayer.begin();
	MAPLAYER::iterator		iter_end	= m_mapLayer.end();

	for( ;iter != iter_end; ++iter)
	{
		iter->second->Render();
	}
}

void ENGINE::CScene::Release(void)
{
	for_each(m_mapLayer.begin(), m_mapLayer.end(), CDeleteMap());
	m_mapLayer.clear();
}

const ENGINE::VTXTEX* ENGINE::CScene::GetTerrainVertex(const WORD& LayerID, 
											   const wstring& wstrObjKey)
{
	MAPLAYER::iterator	iter = m_mapLayer.find(LayerID);

	if(iter == m_mapLayer.end())
		return NULL;

	return iter->second->GetTerrainVertex(wstrObjKey);
}

const ENGINE::VTXCUBE* ENGINE::CScene::GetCubeVertex(const WORD& LayerID, const wstring&	wstrObjKey)
{
	MAPLAYER::iterator	iter = m_mapLayer.find(LayerID);

	if(iter == m_mapLayer.end())
		return NULL;

	return iter->second->GetCubeVertex(wstrObjKey);
}

const D3DXVECTOR3* ENGINE::CScene::GetPos(const WORD& LayerID, const wstring& wstrObjKey)
{
	MAPLAYER::iterator	iter = m_mapLayer.find(LayerID);

	if(iter == m_mapLayer.end())
		return NULL;

	return iter->second->GetPos(wstrObjKey);
}

list<ENGINE::CGameObject*>*	ENGINE::CScene::GetList(const WORD& LayerID, const wstring&	wstrObjKey)
{
	MAPLAYER::iterator	iter = m_mapLayer.find(LayerID);

	if(iter == m_mapLayer.end())
		return NULL;

	return iter->second->GetList(wstrObjKey);
}