// TextureTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool58.h"
#include "TextureTool.h"

#include "Export_Function.h"
#include "ResourcesMgr.h"
#include "MainFrm.h"
#include "Tool58View.h"
#include "CubeObj.h"
#include "Back.h"
#include "..\Engine\Utility\Code\Component.h"
#include "../Reference/Headers/Texture.h"
#include "..\Engine\Utility\Code\GameObject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTextureTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTextureTool, CDialog)

CTextureTool::CTextureTool(CWnd* pParent /*=NULL*/)
	: CDialog(CTextureTool::IDD, pParent)
{

}

CTextureTool::~CTextureTool()
{
}

void CTextureTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
}


BEGIN_MESSAGE_MAP(CTextureTool, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CTextureTool::OnLoad)
	ON_BN_CLICKED(IDC_BUTTON2, &CTextureTool::OnClear)
	ON_LBN_SELCHANGE(IDC_LIST1, &CTextureTool::OnSelect)
END_MESSAGE_MAP()


// CTextureTool 메시지 처리기입니다.

void CTextureTool::OnLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_ListBox.ResetContent();

	//m_pResourceMgr = ENGINE::Get_ResourcesMgr();

	list<wstring> KeyList = ((CMainFrame*)AfxGetMainWnd())->m_pMainView->m_KeyList;

	list<wstring>::iterator iter = KeyList.begin();
	list<wstring>::iterator iter_end = KeyList.end();

	for(iter; iter != iter_end; ++iter)
	{
		m_ListBox.AddString(iter->c_str());
	}
}

void CTextureTool::OnClear()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_ListBox.ResetContent();
}

void CTextureTool::OnSelect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	CString	strSelName;

	int	iSelIndex = m_ListBox.GetCurSel();

	if(iSelIndex < 0)
		return;

	m_ListBox.GetText(iSelIndex, strSelName);

	// Texture 컴포넌트 찾아오기
	CObstacle* pCubeObj = NULL;

	CObj*	pObj = ((CMainFrame*)AfxGetMainWnd())->m_pMainView->m_pBack;
	pCubeObj = ((CBack*)pObj)->m_pCubeForDel;

	if(pCubeObj == NULL)
		return;

	const ENGINE::CComponent*	pComponent = pCubeObj->GetComponent(L"Texture");

	// 컴포넌트 맵 컨테이너 찾아와 Texture 삭제하기
	map<wstring, ENGINE::CComponent*>::iterator iter = pCubeObj->GetmapComponent()->find(L"Texture");

	pCubeObj->GetmapComponent()->erase(iter);

	ENGINE::Safe_Delete(pComponent);
	

	// 컴포넌트 만들기
	wstring wstrSelName = strSelName.operator LPCWSTR();

	pCubeObj->m_wstrObjKey = wstrSelName;

	m_pResourceMgr = ENGINE::Get_ResourcesMgr();
	pComponent = m_pResourceMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, wstrSelName);


	// 컴포넌트 맵 컨테이너에 추가하기
	ENGINE::CComponent* pComponent2 = (ENGINE::CComponent*)pComponent;

	pCubeObj->GetmapComponent()->insert(map<wstring, ENGINE::CComponent*>::value_type(L"Texture", pComponent2));

	// CubeObj의 Texture 수정하기
	pCubeObj->SetTexture(dynamic_cast<ENGINE::CTexture*>(pComponent2));


	

	UpdateData(FALSE);
}
