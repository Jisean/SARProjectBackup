#include "StdAfx.h"
#include "Back.h"
#include "Tool58View.h"
#include "CubeObj.h"
#include "Engine_Include.h"
#include "StaticCamera.h"
#include "Transform.h"
#include "GraphicDev.h"
#include "CollisionMgr.h"
#include "Transform.h"
#include "Terrain.h"
#include "ResourcesMgr.h"
#include "Management.h"
#include "TimeMgr.h"
#include "MouseCol.h"
#include "InfoSubject.h"
#include "Export_Function.h"
#include "CameraObserver.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

vector<CObstacle*> CBack::m_vecCube;
CObstacle* CBack::m_pCubeForDel = NULL;
ENGINE::CTransform*	CBack::m_pTransform = NULL;
CTerrain* CBack::m_pTerrain = NULL;
bool	CBack::bIsPressed = false;
bool	CBack::m_bSelRadio = true;

CBack::CBack(void)
: m_pMainView(NULL)
{

}
CBack::CBack(LPDIRECT3DDEVICE9 pDevice)
: m_pMainView(NULL)
, m_pCollisionMgr(CCollisionMgr::GetInstance())
, m_pInfoSubject(ENGINE::Get_InfoSubject())
{
	m_Device = pDevice;
}
CBack::~CBack(void)
{
	Release();
}

HRESULT CBack::Initialize(void)
{
	CCollisionMgr::GetInstance()->AddColObject(CCollisionMgr::COL_MOUSE);

	m_pTransform = ENGINE::CTransform::Create(g_vLook);
	m_pTransform->m_vPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_pStaticCamera = CStaticCamera::Create(m_Device, m_pTransform);

	m_pTerrain = CTerrain::Create(m_Device);

	/*CCubeObj* m_pCubeObj = CCubeObj::Create(m_Device);
	const D3DXVECTOR3 vPos(10.f, 0.f, 10.f);
	m_pCubeObj->SetPos(vPos);

	CCubeObj* m_pCubeObj2 = CCubeObj::Create(m_Device);
	const D3DXVECTOR3 vPos2(12.f, 0.f, 10.f);
	m_pCubeObj2->SetPos(vPos2);

	CCubeObj* m_pCubeObj3 = CCubeObj::Create(m_Device);
	const D3DXVECTOR3 vPos3(14.f, 0.f, 10.f);
	m_pCubeObj3->SetPos(vPos3);

	CCubeObj* m_pCubeObj4 = CCubeObj::Create(m_Device);
	const D3DXVECTOR3 vPos4(16.f, 0.f, 10.f);
	m_pCubeObj4->SetPos(vPos4);

	CBack::m_vecCube.push_back(m_pCubeObj);
	CBack::m_vecCube.push_back(m_pCubeObj2);
	CBack::m_vecCube.push_back(m_pCubeObj3);
	CBack::m_vecCube.push_back(m_pCubeObj4);*/

	m_pMouseCol = dynamic_cast<CMouseCol*>(m_pCollisionMgr->CloneColObject(CCollisionMgr::COL_MOUSE));

	//m_pCreateCube = ((CMainFrame*)AfxGetMainWnd())->m_pMainView->m_pCreateCube;

	return S_OK;
}

int CBack::Progress(void)
{
	m_pMouseCol->Update();

	// 마우스 충돌 체크
	vector<CObstacle*>::iterator iter = CBack::m_vecCube.begin();
	vector<CObstacle*>::iterator iter_end = CBack::m_vecCube.end();
	if(GetAsyncKeyState(VK_LBUTTON) && CBack::bIsPressed == false)
	{
		for(iter; iter != iter_end; ++iter)
		{
			if(m_pMouseCol->PickObject(&m_vDestPos, (*iter)->m_pVertex, &(*iter)->m_pInfo->m_matWorld))
			{
				CBack::m_pCubeForDel = (*iter);	

				if(m_bSelRadio == false)
				{
					m_pInfoSubject->UnSubscribe((*iter)->m_pCameraObserver);
					m_vecCube.erase(iter);
					ENGINE::Safe_Delete(m_pCubeForDel);
					m_pCubeForDel = NULL;

					break;
				}
			}
		}
	}

	if(!GetAsyncKeyState(VK_LBUTTON) && CBack::bIsPressed == true)
		CBack::bIsPressed = false;

	iter = CBack::m_vecCube.begin();
	iter_end = CBack::m_vecCube.end();

	for(; iter != iter_end; ++iter)
	{
		(*iter)->Update();
	}

	
	////// 선택
	//if(m_bSelRadio == true)
	//{
	//	//(*iter)->Update();
	//}
	//else if(m_bSelRadio == false)
	//{
	//	iter = CBack::m_vecCube.begin();
	//	for(; iter != CBack::m_vecCube.end();)
	//	{
	//		if((*iter) == m_pCubeForDel)
	//		{
	//			// 삭제
	//			m_pInfoSubject->UnSubscribe((*iter)->m_pCameraObserver);
	//			iter = m_vecCube.erase(iter);
	//			ENGINE::Safe_Delete(m_pCubeForDel);
	//			m_pCubeForDel = NULL;
	//			return 0;
	//		}
	//		else
	//			++iter;
	//	}

	//}
	

	m_pTerrain->Update();
	m_pStaticCamera->Update();


	
	((CMainFrame*)AfxGetMainWnd())->m_pMainView->Invalidate(FALSE);

	return 0;
}

void CBack::Render(void)
{	
	vector<CObstacle*>::iterator iter = CBack::m_vecCube.begin();
	vector<CObstacle*>::iterator iter_end = CBack::m_vecCube.end();
	for(; iter != iter_end; ++iter)
	{
		(*iter)->Render();
	}

	m_pTerrain->Render();

	//((CMainFrame*)AfxGetMainWnd())->m_pMainView->Invalidate(TRUE);
}

void CBack::Release(void)
{
	/*for_each(m_vecTile.begin(), m_vecTile.end(), DELETEOBJ());
	m_vecTile.clear();*/
	vector<CObstacle*>::iterator iter = CBack::m_vecCube.begin();
	vector<CObstacle*>::iterator iter_end = CBack::m_vecCube.end();
	for(; iter != iter_end; ++iter)
	{
		ENGINE::Safe_Delete(*iter);
	}
	CBack::m_vecCube.clear();

	ENGINE::Safe_Delete(m_pTerrain);
	ENGINE::Safe_Delete(m_pStaticCamera);
	ENGINE::Safe_Delete(m_pTransform);
	
	m_pCollisionMgr->DestroyInstance();
	ENGINE::CManagement::GetInstance()->DestroyInstance();
	ENGINE::CTimeMgr::GetInstance()->DestroyInstance();
	ENGINE::Safe_Delete(m_pMouseCol);
	//ENGINE::CResourcesMgr::GetInstance()->DestroyInstance();
}

void CBack::SetMainView(CTool58View* pMainView)
{
	m_pMainView = pMainView;
}

void CBack::MiniViewRender(void)
{

}

const vector<TILE*>* CBack::GetTile(void)
{
	return &m_vecTile;
}
