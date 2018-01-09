#include "Layer.h"
#include "component.h"

ENGINE::CLayer::CLayer(LPDIRECT3DDEVICE9 pDevice)
: m_pDevice(pDevice)
{

}
ENGINE::CLayer::~CLayer()
{
	Release();
}
ENGINE::CLayer* ENGINE::CLayer::Create(LPDIRECT3DDEVICE9 pDevice)
{
	return new CLayer(pDevice);	
}
void ENGINE::CLayer::Update(void)
{
	MAPOBJLIST::iterator		mapiter = m_mapObjList.begin();
	MAPOBJLIST::iterator		mapiter_end = m_mapObjList.end();

	for( ;mapiter != mapiter_end; ++mapiter)
	{
		OBJLIST::iterator	objiter = mapiter->second.begin();
		OBJLIST::iterator	objiter_end = mapiter->second.end();

		for( ; objiter != objiter_end; ++objiter)
		{
			(*objiter)->Update();
		}
	}

}

void ENGINE::CLayer::Render(void)
{
	MAPOBJLIST::iterator		mapiter = m_mapObjList.begin();
	MAPOBJLIST::iterator		mapiter_end = m_mapObjList.end();

	for( ;mapiter != mapiter_end; ++mapiter)
	{
		OBJLIST::iterator	objiter = mapiter->second.begin();
		OBJLIST::iterator	objiter_end = mapiter->second.end();

		for( ; objiter != objiter_end; ++objiter)
		{
			(*objiter)->Render();
		}
	}
}

void ENGINE::CLayer::Release(void)
{
	MAPOBJLIST::iterator		mapiter = m_mapObjList.begin();
	MAPOBJLIST::iterator		mapiter_end = m_mapObjList.end();

	for( ;mapiter != mapiter_end; ++mapiter)
	{
		OBJLIST::iterator	objiter = mapiter->second.begin();
		OBJLIST::iterator	objiter_end = mapiter->second.end();

		for( ; objiter != objiter_end; ++objiter)
		{
			ENGINE::Safe_Delete(*objiter);
		}
		mapiter->second.clear();
	}

	m_mapObjList.clear();
}

HRESULT ENGINE::CLayer::AddObject(const wstring& wstrObjKey, CGameObject* pGameObject)
{
	if(pGameObject)
	{
		MAPOBJLIST::iterator	iter = m_mapObjList.find(wstrObjKey);

		if(iter == m_mapObjList.end())
		{
			m_mapObjList[wstrObjKey] = OBJLIST();
		}

		m_mapObjList[wstrObjKey].push_back(pGameObject);
	}

	return S_OK;
}

const ENGINE::CComponent* ENGINE::CLayer::GetComponent(const wstring& wstrObjKey, 
											   const wstring& wstrComponentKey)
{
	MAPOBJLIST::iterator	iter = m_mapObjList.find(wstrObjKey);

	if(iter == m_mapObjList.end())
		return NULL;

	OBJLIST::iterator	iter_Begin = iter->second.begin();
	OBJLIST::iterator	iter_end = iter->second.end();
	
		for( ;iter_Begin != iter_end; ++iter_Begin)
		{
			const CComponent* pComponent = (*iter_Begin)->GetComponent(wstrComponentKey);

			if(pComponent != NULL)
				return pComponent;
		}

		return NULL;
}

const ENGINE::VTXTEX* ENGINE::CLayer::GetTerrainVertex(const wstring& wstrObjKey)
{
	MAPOBJLIST::iterator		iter = m_mapObjList.find(wstrObjKey);

	if(iter == m_mapObjList.end())
		return NULL;

	return iter->second.front()->GetTerrainVertex();
}

const ENGINE::VTXCUBE* ENGINE::CLayer::GetCubeVertex(const wstring& wstrObjKey)
{
	MAPOBJLIST::iterator		iter = m_mapObjList.find(wstrObjKey);

	if(iter == m_mapObjList.end())
		return NULL;

	return iter->second.front()->GetCubeVertex();
}

const D3DXVECTOR3* ENGINE::CLayer::GetPos(const wstring& wstrObjKey)
{
	MAPOBJLIST::iterator		iter = m_mapObjList.find(wstrObjKey);

	if(iter == m_mapObjList.end())
		return NULL;

	return iter->second.front()->GetPos();
}
list<ENGINE::CGameObject*>*	ENGINE::CLayer::GetList(const wstring&	wstrObjKey)
{
	MAPOBJLIST::iterator		iter = m_mapObjList.find(wstrObjKey);

	if(iter == m_mapObjList.end())
		return NULL;

	return &iter->second;
}




