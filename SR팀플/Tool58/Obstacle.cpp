#include "StdAfx.h"
#include "Obstacle.h"
#include "Transform.h"
#include "..\Engine\Resources\Code\ResourcesMgr.h"
#include "Export_Function.h"
#include "../Reference/Headers/Texture.h"

CObstacle::CObstacle(LPDIRECT3DDEVICE9 pDevice)
: CGameObject(pDevice)
, m_pVertex(NULL)
, m_pConvertVertex(NULL)
, m_dwVtxCnt(0)
, m_pCameraObserver(NULL)
, m_iflag(0)
{
	m_wstrObjKey = L"Monster1";
}

CObstacle::~CObstacle(void)
{
}

ENGINE::CTransform* CObstacle::GetInfo(void)
{
	return m_pInfo;
}

std::wstring CObstacle::GetObjKey(void)
{
	return m_wstrObjKey;
}

void CObstacle::SetTexture(ENGINE::CTexture* pTexture)
{
	m_pTexture = pTexture;
}

map<wstring, ENGINE::CComponent*>* CObstacle::GetmapComponent(void)
{
	return &m_mapComponent;
}

const ENGINE::VTXTEX* CObstacle::GetTerrainVertex(void)
{
	return NULL;
}

const ENGINE::VTXCUBE* CObstacle::GetCubeVertex(void)
{
	return m_pVertex;
}

const D3DXVECTOR3* CObstacle::GetPos(void)
{
	return &m_pInfo->m_vPos;
}

HRESULT CObstacle::Initialize(void)
{
	return S_OK;
}

void CObstacle::Update(void)
{

}

void CObstacle::Render(void)
{

}

void CObstacle::Release(void)
{

}

void CObstacle::ChangeTexture(CObstacle* pCubeObj, const wstring& wstrName)
{
	if(pCubeObj == NULL)
		return;

	const ENGINE::CComponent*	pComponent = pCubeObj->GetComponent(L"Texture");

	// ������Ʈ �� �����̳� ã�ƿ� Texture �����ϱ�
	map<wstring, ENGINE::CComponent*>::iterator iter = pCubeObj->GetmapComponent()->find(L"Texture");

	pCubeObj->GetmapComponent()->erase(iter);

	ENGINE::Safe_Delete(pComponent);


	// ������Ʈ �����
	pCubeObj->m_wstrObjKey = wstrName;

	ENGINE::CResourcesMgr* m_pResourceMgr = ENGINE::Get_ResourcesMgr();
	pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, wstrName);


	// ������Ʈ �� �����̳ʿ� �߰��ϱ�
	ENGINE::CComponent* pComponent2 = (ENGINE::CComponent*)pComponent;

	pCubeObj->GetmapComponent()->insert(map<wstring, ENGINE::CComponent*>::value_type(L"Texture", pComponent2));

	// CubeObj�� Texture �����ϱ�
	pCubeObj->SetTexture(dynamic_cast<ENGINE::CTexture*>(pComponent2));
}
