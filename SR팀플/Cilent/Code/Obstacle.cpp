#include "StdAfx.h"
#include "Obstacle.h"
#include "Transform.h"
#include "Stage.h"
#include "Management.h"
#include "Player.h"
#include "Export_Function.h"
#include "ResourcesMgr.h"
#include "Texture.h"

CObstacle::CObstacle(LPDIRECT3DDEVICE9 pDevice)
: CGameObject(pDevice)
, m_pVertex(NULL)
, m_pConvertVertex(NULL)
, m_dwVtxCnt(0)
, m_pCameraObserver(NULL)
, m_iflag(0)
, m_bBehind(false)
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
	m_pPlayer = ENGINE::CManagement::GetInstance()->GetList(CStage::LAYER_GAMELOGIC, L"Player")->front();
	D3DXVECTOR3 vLook(((CPlayer*)m_pPlayer)->GetInfo()->m_vDir);
	D3DXVec3Normalize(&vLook, &vLook);
	D3DXVECTOR3 vPlayerToObstacle = m_pInfo->m_vPos - ((CPlayer*)m_pPlayer)->GetInfo()->m_vPos;
	D3DXVec3Normalize(&vPlayerToObstacle, &vPlayerToObstacle);
	float fCos = D3DXVec3Dot(&vLook, &vPlayerToObstacle);
	float fAngle = acosf(fCos);
	if(D3DXToRadian(120.f) <= fAngle)
		m_bBehind = true;
	else
		m_bBehind = false;
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

	// 컴포넌트 맵 컨테이너 찾아와 Texture 삭제하기
	map<wstring, ENGINE::CComponent*>::iterator iter = pCubeObj->GetmapComponent()->find(L"Texture");

	pCubeObj->GetmapComponent()->erase(iter);

	ENGINE::Safe_Delete(pComponent);


	// 컴포넌트 만들기
	pCubeObj->m_wstrObjKey = wstrName;

	ENGINE::CResourcesMgr* m_pResourceMgr = ENGINE::Get_ResourcesMgr();
	pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_STATIC, wstrName);


	// 컴포넌트 맵 컨테이너에 추가하기
	ENGINE::CComponent* pComponent2 = (ENGINE::CComponent*)pComponent;

	pCubeObj->GetmapComponent()->insert(map<wstring, ENGINE::CComponent*>::value_type(L"Texture", pComponent2));

	// CubeObj의 Texture 수정하기
	pCubeObj->SetTexture(dynamic_cast<ENGINE::CTexture*>(pComponent2));
}
