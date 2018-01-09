#include "GameObject.h"
#include "Component.h"

ENGINE::CGameObject::CGameObject(LPDIRECT3DDEVICE9 pDevice)
: m_pDevice(pDevice)
{

}

ENGINE::CGameObject::~CGameObject()
{
	Release();
}

HRESULT ENGINE::CGameObject::Initialize(void)
{
	return S_OK;
}

void ENGINE::CGameObject::Update(void)
{
	MAPCOMPONENT::iterator	iter = m_mapComponent.begin();
	MAPCOMPONENT::iterator	iter_end = m_mapComponent.end();

	for( ;iter != iter_end; ++iter)
	{
		iter->second->Update();
	}
}

void ENGINE::CGameObject::Render(void)
{

}

void ENGINE::CGameObject::Release(void)
{
	for_each(m_mapComponent.begin(), m_mapComponent.end(), CDeleteMap());
	m_mapComponent.clear();
}

const  ENGINE::CComponent* ENGINE::CGameObject::GetComponent(const wstring& wstrComponentKey)
{
	MAPCOMPONENT::iterator	iter = m_mapComponent.find(wstrComponentKey);
	if(iter == m_mapComponent.end())
		return NULL;

	return iter->second;
}

const ENGINE::VTXTEX* ENGINE::CGameObject::GetTerrainVertex(void)
{
	return NULL;
}
const ENGINE::VTXCUBE* ENGINE::CGameObject::GetCubeVertex(void)
{
	return NULL;
}
const D3DXVECTOR3* ENGINE::CGameObject::GetPos(void)
{
	return NULL;
}


