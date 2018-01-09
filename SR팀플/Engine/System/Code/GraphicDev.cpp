#include "GraphicDev.h"

IMPLEMENT_SINGLETON(ENGINE::CGraphicDev)

ENGINE::CGraphicDev::CGraphicDev(void)
: m_pSDK(NULL)
, m_pDevice(NULL)
{

}

ENGINE::CGraphicDev::~CGraphicDev(void)
{
	Release();
}

HRESULT ENGINE::CGraphicDev::InitGraphicDev(WINMODE Mode, HWND hWnd, 
											const WORD& wSizeX, const WORD& wSizeY)
{
	//바꿨어 내용을 또 바꿨어
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);

	D3DCAPS9		devicecaps;
	ZeroMemory(&devicecaps, sizeof(D3DCAPS9));

	if(FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &devicecaps)))
	{
		/*MSG_BOX(L"GetDeviceCaps Failed");*/
		return E_FAIL;
	}

	DWORD		vp;

	if(devicecaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	vp |= D3DCREATE_MULTITHREADED;

	D3DPRESENT_PARAMETERS	d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	SetParameters(d3dpp, Mode, hWnd, wSizeX, wSizeY);

	if(FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL
		, hWnd, vp, &d3dpp, &m_pDevice)))
	{
		/*MSG_BOX(L"CreateDevice Failed");*/
		return E_FAIL;
	}
	// 스프라이트 객체 초기화

	if(FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
	{
		//ERR_MSG(L"Sprite Init Failed");
		return E_FAIL;
	}

	return S_OK;
}

void ENGINE::CGraphicDev::SetParameters(D3DPRESENT_PARAMETERS& d3dpp, WINMODE Mode, HWND hWnd, const WORD& wSizeX, const WORD& wSizeY)
{
	d3dpp.BackBufferWidth = wSizeX;
	d3dpp.BackBufferHeight = wSizeY;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;

	d3dpp.MultiSampleQuality = 0;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;

	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.EnableAutoDepthStencil = TRUE;

	d3dpp.hDeviceWindow = hWnd;
	d3dpp.Windowed		= Mode;

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}

void ENGINE::CGraphicDev::Release(void)
{
	//Engine::Safe_Release(m_pDevice);
	//Engine::Safe_Release(m_pSDK);

	m_pSprite->Release();
	m_pDevice->Release();
	m_pSDK->Release();
}
LPD3DXSPRITE ENGINE::CGraphicDev::GetSprite(void)
{
	return m_pSprite;
}
void ENGINE::CGraphicDev::Render_Begin(void)
{
	m_pDevice->BeginScene();
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void ENGINE::CGraphicDev::Render_End(void)
{
	m_pSprite->End();
	m_pDevice->EndScene();
	
}