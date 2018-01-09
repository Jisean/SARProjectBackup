#include "Renderer.h"
#include "Scene.h"

ENGINE::CRenderer::CRenderer(LPDIRECT3DDEVICE9 pDevice)
: m_pDevice(pDevice)
, m_pScene(NULL)
, m_pD3DXFont(NULL)
, m_fTime(0.f)
, m_wFrameCnt(0)

{
	ZeroMemory(m_szFPS, sizeof(TCHAR) * 128);
}

ENGINE::CRenderer::~CRenderer()
{
	
}

ENGINE::CRenderer* ENGINE::CRenderer::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CRenderer*		pRenderer = new CRenderer(pDevice);

	if(FAILED(pRenderer->InitRenderer()))
		ENGINE::Safe_Delete(pRenderer);

	return pRenderer;
}

HRESULT ENGINE::CRenderer::InitRenderer(void)
{	
	D3DXFONT_DESC			hFont;
	ZeroMemory(&hFont, sizeof(D3DXFONT_DESC));

	hFont.Width = 10;
	hFont.Height = 15;
	hFont.Weight = FW_NORMAL;
	hFont.CharSet = HANGUL_CHARSET;
	lstrcpy(hFont.FaceName, L"±Ã¼­");

	FAILED_CHECK(D3DXCreateFontIndirect(m_pDevice, &hFont, &m_pD3DXFont));

	return S_OK;
}

void ENGINE::CRenderer::Render(float fTime)
{
	RECT		rc = { 100, 100, 800, 600 };

	++ m_wFrameCnt;
	m_fTime += fTime;

	if(m_fTime > 1.f)
	{
		m_fTime = 0.f;
		wsprintf(m_szFPS, L"FPS : %d", m_wFrameCnt);
		m_wFrameCnt = 0;
	}

	m_pDevice->Clear(0, NULL, 
		D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 
		D3DCOLOR_ARGB(255, 0,0, 255), 1.f, 0);
	
	m_pDevice->BeginScene();
	
	m_pD3DXFont->DrawText(NULL, m_szFPS, lstrlen(m_szFPS), 
		&rc, DT_NOCLIP, D3DCOLOR_ARGB(255, 0, 255, 0));


	if(NULL != m_pScene)
		m_pScene->Render();

	m_pDevice->EndScene();
	m_pDevice->Present(NULL, NULL, NULL, NULL);
}

void ENGINE::CRenderer::SetScene(CScene* pScene)
{
	m_pScene = pScene;
}


