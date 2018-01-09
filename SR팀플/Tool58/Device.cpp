#include "StdAfx.h"
#include "Device.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_SINGLETON2(CDevice)

CDevice::CDevice(void)
: m_p3D(NULL)
, m_pDevice(NULL)
, m_pSprite(NULL)
, m_pFont(NULL)
{
}

CDevice::~CDevice(void)
{
	Release();
}

HRESULT CDevice::InitDevice(void)
{
	m_p3D	= Direct3DCreate9(D3D_SDK_VERSION);

	D3DCAPS9	DeviceCaps;	
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	if(FAILED(m_p3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
	{
		//MessageBox(g_hWnd, L"장치 생성 실패", L"System Error", NULL);
		AfxMessageBox(L"장치 생성 실패");
		return E_FAIL;
	}

	DWORD		vp;

	if(DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;

	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	D3DPRESENT_PARAMETERS		d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	SetParameters(d3dpp);

	// 디바이스 초기화

	if(FAILED(m_p3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, 
		g_hWnd, vp, &d3dpp, &m_pDevice)))
	{
		AfxMessageBox(L"Device Init Failed");
		return E_FAIL;
	}

	// 스프라이트 객체 초기화

	if(FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
	{
		AfxMessageBox(L"Sprite Init Failed");
		return E_FAIL;
	}

	// 폰트 객체 초기화

	D3DXFONT_DESCW			tFontInfo;
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESCW));

	tFontInfo.Height = 20;
	tFontInfo.Width  = 10;
	tFontInfo.Weight = FW_HEAVY; 
	tFontInfo.CharSet = HANGUL_CHARSET;
	lstrcpy(tFontInfo.FaceName, L"궁서");


	if(FAILED(D3DXCreateFontIndirect(m_pDevice, &tFontInfo, &m_pFont)))
	{
		AfxMessageBox(L"Font Init Failed");
		return E_FAIL;
	}

	return S_OK;
}

void CDevice::Release(void)
{
	m_pFont->Release();
	m_pSprite->Release();
	m_pDevice->Release();
	m_p3D->Release();
}

void CDevice::SetParameters(D3DPRESENT_PARAMETERS& d3dpp)
{
	d3dpp.BackBufferWidth	= WINCX;
	d3dpp.BackBufferHeight	= WINCY;
	d3dpp.BackBufferFormat	= D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount	= 1;

	d3dpp.MultiSampleType	= D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = g_hWnd;
	d3dpp.Windowed	= TRUE;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
		
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}

void CDevice::Render_Begin(void)
{
	

	m_pDevice->BeginScene();
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void CDevice::Render_End(void)
{
	m_pSprite->End();
	m_pDevice->EndScene();
	
}

LPDIRECT3DDEVICE9 CDevice::GetDevice(void)
{
	return m_pDevice;
}

LPD3DXSPRITE CDevice::GetSprite(void)
{
	return m_pSprite;
}

LPD3DXFONT CDevice::GetFont(void)
{
	return m_pFont;
}
