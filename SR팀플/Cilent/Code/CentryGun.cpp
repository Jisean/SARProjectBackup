#include "stdafx.h"
#include "CentryGun.h"

#include "Include.h"
#include "Texture.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Pipeline.h"
#include "CameraObserver.h"
#include "CollisionMgr.h"
#include "TerrainCol.h"
#include "Stage.h"
#include "ObjectCol.h"
#include "KeyMgr.h"
#include "MonsterTex.h"
#include "Player.h"
#include "Bullet.h"

CCentryGun::CCentryGun(LPDIRECT3DDEVICE9 pDevice)
: CObstacle(pDevice)
, m_pResourceMgr(ENGINE::Get_ResourcesMgr())
, m_pManagement(ENGINE::Get_Management())
, m_pTimeMgr(ENGINE::Get_Time())
, m_pInfoSubject(ENGINE::Get_InfoSubject())
, m_pCollisionMgr(CCollisionMgr::GetInstance())
, m_pTerrainCol(NULL)
, m_pTerrainVertex(NULL)
, m_bMove(false)
, vTrans(D3DXVECTOR3(2.f, 0.f, 0.f))
, m_iHp(3)
{

}

CCentryGun::~CCentryGun()
{
	Release();
}

HRESULT CCentryGun::Initialize(void)
{
	m_pCameraObserver = CCameraObserver::Create();
	m_pInfoSubject->Subscribe(m_pCameraObserver);


	m_fSpeed = 10.f;
	m_fDistance = 10.f;
	m_fCameraAngle = D3DXToRadian(45.f);

	m_dwVtxCnt = 8;


	m_pVertex			= new ENGINE::VTXCUBE[m_dwVtxCnt];
	m_pConvertVertex	= new ENGINE::VTXCUBE[m_dwVtxCnt];
	m_pColVtx			 = new ENGINE::VTXCUBE[m_dwVtxCnt];
	m_pResourceMgr->GetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer_CubeTex", m_pVertex);

	FAILED_CHECK(AddComponent());

	m_fAngle = 0.f;

	m_dwTime = GetTickCount();

	TexToPos();

	m_iflag = 3;

	m_pMonsterTex = CMonsterTex::Create(m_pDevice);
	NULL_CHECK_RETURN(m_pMonsterTex, E_FAIL);

	return S_OK;
}

void CCentryGun::Update(void)
{

	list<ENGINE::CGameObject*>* pPlayerList = m_pManagement->GetList(CStage::LAYER_GAMELOGIC,L"Player");
	m_pTarget = pPlayerList->front();

	//const D3DXVECTOR3* vTargetPos = m_pTarget->GetPos();
	//m_pInfo->m_vDir = (*vTargetPos) - m_pInfo->m_vDir;

	//m_pTerrainVertex = m_pManagement->GetTerrainVertex(CStage::LAYER_GAMELOGIC, L"Terrain");
	//m_pTerrainCol->SetColInfo(&m_pInfo->m_vPos, m_pTerrainVertex);

	D3DXVec3TransformNormal(&m_pInfo->m_vDir, &g_vLook, &m_pInfo->m_matWorld);
	D3DXVec3Normalize(&m_pInfo->m_vDir, &m_pInfo->m_vDir);

	ENGINE::CGameObject::Update();

	m_pMonsterTex->GetpInfo()->m_vPos = m_pInfo->m_vPos;
	m_pMonsterTex->GetpInfo()->m_vScale = m_pInfo->m_vScale;

	CObstacle::Update();

	SetTransForm();

	if(m_dwTime + 2000 < GetTickCount())
	{
		m_dwTime = GetTickCount();
		CBullet * pBullet = CBullet::Create(m_pDevice);
		pBullet->SetPos(m_pInfo->m_vPos);
	
		const D3DXVECTOR3* vTargetPos = &(((CPlayer*)m_pTarget)->GetInfo()->m_vPos);
		D3DXVECTOR3 vBulletDir = *vTargetPos - m_pInfo->m_vPos;
		pBullet->SetBulletDir(vBulletDir);
		m_BulletList.push_back(pBullet);
	}
	list<CBullet*>::iterator iter = m_BulletList.begin();
	for(; iter != m_BulletList.end(); ++iter)
	{
		(*iter)->Update();
	}

	//총알이 센트리건으로부터 일정거리 이상 떨어지면 총알 삭제
	iter = m_BulletList.begin();
	for(; iter != m_BulletList.end(); )
	{
		D3DXVECTOR3 vLenght = *((*iter)->GetPos()) - m_pInfo->m_vPos;
		float fLenght = D3DXVec3Length(&vLenght);
		if(fLenght > 10.f)
		{
			ENGINE::Safe_Delete((*iter));
			iter = m_BulletList.erase(iter);
		}
		else
			++iter;
	}
	if(m_pMonsterTex != NULL)
	{
		m_pMonsterTex->SetMonsterTex(&m_pInfo->m_vPos);
		m_pMonsterTex->Update();
	}
}

void CCentryGun::Render(void)
{
	//m_pDevice->SetTransform(D3DTS_WORLD, &m_pInfo->m_matWorld);

	//m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00000088);
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pResourceMgr->SetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer_CubeTex", m_pConvertVertex);

	if(m_bBehind == false)
	{
		//m_pTexture->Render(0);
		//m_pBuffer->Render();
		if(m_pMonsterTex != NULL)
			m_pMonsterTex->Render();
		
	}
	list<CBullet*>::iterator iter = m_BulletList.begin();
	for(; iter != m_BulletList.end(); ++iter)
	{
		(*iter)->Render();
	}
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	
}

void CCentryGun::Release(void)
{
	m_pInfoSubject->UnSubscribe(m_pCameraObserver);
	ENGINE::Safe_Delete(m_pCameraObserver);

	if(m_pVertex)
		ENGINE::Safe_Delete_Array(m_pVertex);

	if(m_pConvertVertex)
		ENGINE::Safe_Delete_Array(m_pConvertVertex);

	if(m_pColVtx)
		ENGINE::Safe_Delete_Array(m_pColVtx);
	
	list<CBullet*>::iterator iter = m_BulletList.begin();
	for(; iter != m_BulletList.end(); ++iter)
	{
		ENGINE::Safe_Delete((*iter));
	}
	m_BulletList.clear();

	/*if(m_pColVtx)
		ENGINE::Safe_Delete_Array(m_pColVtx);

	if(m_pMonsterTex)
		ENGINE::Safe_Delete(m_pMonsterTex);*/
}



HRESULT CCentryGun::AddComponent(void)
{
	ENGINE::CComponent*			pComponent = NULL;

	// Transform
	pComponent = m_pInfo = ENGINE::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	// Buffer
	pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, L"Buffer_CubeTex");
	m_pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	// Collision Terrain
	pComponent = m_pCollisionMgr->CloneColObject(CCollisionMgr::COL_TERRAIN);
	m_pTerrainCol = dynamic_cast<CTerrainCol*>(pComponent);
	NULL_CHECK_RETURN(m_pTerrainCol, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"TerrainCol", pComponent));

	// Texture
	pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_STATIC, L"Monster1");
	m_pTexture = dynamic_cast<ENGINE::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));


	return S_OK;
}

CCentryGun* CCentryGun::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CCentryGun* pPlayer = new CCentryGun(pDevice);

	if(FAILED(pPlayer->Initialize()))
		ENGINE::Safe_Delete(pPlayer);

	return pPlayer;
}


void CCentryGun::SetTransForm(void)
{
	/*for(int i = 0 ; i < m_dwVtxCnt; ++i)
	{
		m_pVertex[i] *= matworld;
		m_pVertex[i] *= matView;
		m_pVertex[i] *= matProj;
	}*/

	const D3DXMATRIX*		pMatView = m_pCameraObserver->GetView();
	const D3DXMATRIX*		pMatProj = m_pCameraObserver->GetProj();

	for(size_t i = 0; i < m_dwVtxCnt; ++i)
	{
		m_pConvertVertex[i] = m_pVertex[i];

		// world
		ENGINE::CPipeline::MyTransformCoord(&m_pConvertVertex[i].vPos, 
			&m_pConvertVertex[i].vPos, &m_pInfo->m_matWorld);

		m_pColVtx[i] = m_pConvertVertex[i];

		// view
		ENGINE::CPipeline::MyTransformCoord(&m_pConvertVertex[i].vPos, 
			&m_pConvertVertex[i].vPos, pMatView);

		if(m_pConvertVertex[i].vPos.z < 1.f)
			m_pConvertVertex[i].vPos.z = 1.f;

		// proj
		ENGINE::CPipeline::MyTransformCoord(&m_pConvertVertex[i].vPos, 
			&m_pConvertVertex[i].vPos, pMatProj);
	}

}

void CCentryGun::SetPos(const D3DXVECTOR3& vPos)
{
	m_pInfo->m_vPos = vPos;
}



const	ENGINE::VTXCUBE* CCentryGun::GetCubeVertex(void)
{
	return m_pColVtx;
}
const D3DXVECTOR3* CCentryGun::GetPos(void)
{
	return &m_pInfo->m_vPos;
}


void CCentryGun::TexToPos(void)
{
	for(int i = 0; i < 8; ++i)
	{
		m_pVertex[i].vPos = m_pVertex[i].vTex;
	}
}
