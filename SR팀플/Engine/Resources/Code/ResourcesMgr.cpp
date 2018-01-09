#include "ResourcesMgr.h"
#include "TriCol.h"
#include "RcCol.h"
#include "RcTerrain.h"
#include "Texture.h"
#include "TerrainTex.h"
#include "RcTex.h"
#include "Component.h"
#include "CubeTex.h"
#include "RcUI.h"
#include "RcTexTarget.h"

IMPLEMENT_SINGLETON(ENGINE::CResourcesMgr)

ENGINE::CResourcesMgr::CResourcesMgr()
{

}

ENGINE::CResourcesMgr::~CResourcesMgr()
{
	Release();
}

HRESULT ENGINE::CResourcesMgr::AddBuffer(LPDIRECT3DDEVICE9 pDevice, RESOURCETYPE eType, 
										 ENGINE::CVIBuffer::BUFFERTYPE eBufferType, 
										 const wstring& wstrResourceKey, const WORD& wCntX /*= 0*/, 
										 const WORD& wCntY /*= 0*/, const WORD& wItv /*= 1*/)
{
	
	MAPRESOURCE::iterator		iter = m_MapResource[eType].find(wstrResourceKey);


	/*MAPRESOURCE::iterator		iter2 = 
		find_if(m_MapResource[eType].begin(), 
		m_MapResource[eType].end(), CTagFinder(pResourceKeyasdf));*/


	if(iter != m_MapResource[eType].end())
	{
		TAGMSG_BOX(wstrResourceKey.c_str(), L"중복된 리소스");
		return E_FAIL;
	}

	/*if(NULL != m_pResource)
		ENGINE::Safe_Delete(m_pResource);*/

	CResources* pResources = NULL;

	switch(eBufferType)
	{
	case CVIBuffer::BUFFER_TRICOL:
		pResources = CTriCol::Create(pDevice);
		break;

	case CVIBuffer::BUFFER_RCCOL:
		pResources = CRcCol::Create(pDevice);
		break;

	case CVIBuffer::BUFFER_TERRAIN:
		pResources = CRcTerrain::Create(pDevice, wCntX, wCntY, wItv);
		break;

	case CVIBuffer::BUFFER_RCTEX:
		pResources = CRcTex::Create(pDevice);
		break;

	case CVIBuffer::TERRAIN_TEX:
		pResources = CTerrainTex::Create(pDevice, wCntX, wCntY, wItv);
		break;

	case CVIBuffer::BUFFER_CUBETEX:
		pResources = CCubeTex::Create(pDevice);
		break;

	case CVIBuffer::BUFFER_RCUI:
		pResources = CRcUI::Create(pDevice);
		break;

	case CVIBuffer::BUFFER_RCTARGET:
		pResources = CRcTexTarget::Create(pDevice);
		break;
	}

	NULL_CHECK_RETURN(pResources, NULL);
	
	m_MapResource[eType].insert(map<wstring, CResources*>::value_type(wstrResourceKey, pResources));


	return S_OK;
}

void ENGINE::CResourcesMgr::Release(void)
{
	for(int i = 0; i < RESOURCE_END; ++i)
	{
		for_each(m_MapResource[i].begin(), m_MapResource[i].end(), CDeleteMap());
		m_MapResource[i].clear();
	}
}

HRESULT ENGINE::CResourcesMgr::AddTexture(LPDIRECT3DDEVICE9 pDevice, RESOURCETYPE eType, 
										  TEXTURETYPE eTextureType, const wstring& wstrResourceKey, 
										  const wstring& wstrFilePath, const WORD& wCnt)
{
	MAPRESOURCE::iterator		iter = m_MapResource[eType].find(wstrResourceKey);

	if(iter != m_MapResource[eType].end())
	{
		TAGMSG_BOX(wstrResourceKey.c_str(), L"중복된 리소스");
		return E_FAIL;
	}

	CResources*		pResource = CTexture::Create(pDevice, eTextureType, wstrFilePath, wCnt);
	NULL_CHECK_RETURN(pResource, E_FAIL);

	m_MapResource[eType].insert(MAPRESOURCE::value_type(wstrResourceKey, pResource));

	return S_OK;
}

void ENGINE::CResourcesMgr::ResetDynamic(void)
{
	for_each(m_MapResource[RESOURCE_DYNAMIC].begin(), m_MapResource[RESOURCE_DYNAMIC].end(),
		CDeleteMap());

	m_MapResource[RESOURCE_DYNAMIC].clear();
}

ENGINE::CComponent* ENGINE::CResourcesMgr::CloneResource(RESOURCETYPE eResourceType, 
												 const wstring& wstrResourceKey)
{
	
	MAPRESOURCE::iterator	iter = m_MapResource[eResourceType].find(wstrResourceKey);

	if(iter == m_MapResource[eResourceType].end())
	{
		TAGMSG_BOX(wstrResourceKey.c_str(), L"복사 실패");
		return NULL;
	}

	return iter->second->CloneResource();

}

void ENGINE::CResourcesMgr::GetVtxInfo(RESOURCETYPE eResourcetype, 
									   const wstring& wstrResourceKey, void* pVertex)
{
	MAPRESOURCE::iterator		iter = m_MapResource[eResourcetype].find(wstrResourceKey);

	if(iter == m_MapResource[eResourcetype].end())
	{
		TAGMSG_BOX(wstrResourceKey.c_str(), L"버텍스 검색 실패");
		return;
	}

	((CVIBuffer*)iter->second)->GetVtxInfo(pVertex);
}

void ENGINE::CResourcesMgr::SetVtxInfo(RESOURCETYPE eResourcetype, 
									   const wstring& wstrResourceKey, void* pVertex)
{
	MAPRESOURCE::iterator		iter = m_MapResource[eResourcetype].find(wstrResourceKey);

	if(iter == m_MapResource[eResourcetype].end())
	{
		TAGMSG_BOX(wstrResourceKey.c_str(), L"버텍스 검색 실패");
		return;
	}

	((CVIBuffer*)iter->second)->SetVtxInfo(pVertex);
}



