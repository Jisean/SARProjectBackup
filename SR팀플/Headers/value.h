#pragma once

extern	HWND g_hWnd;

const int WINCX = 800;
const int WINCY = 600;

const D3DXVECTOR3		g_vLook = D3DXVECTOR3(0.f, 0.f, 1.f);
const WORD		VTXCNTX = 30;
const WORD		VTXCNTZ = 300;
const WORD		VTXITV	= 1;	

const WORD	MIN_STR		= 64;
const WORD	MIDDLE_STR	= 128;
const WORD	MAX_STR		= 256;

// 타일

const int	TILECX = 130;
const int	TILECY = 68;

const int	TILEX = 20;
const int	TILEY = 30;

// 아이템

const BYTE	RUBY		= 0x0001;
const BYTE	SAPPHIRE	= 0x0002;
const BYTE	DIAMOND		= 0x0004;

// 키 값

const DWORD		KEY_LBUTTON = 0x00000001;
const DWORD		KEY_RBUTTON = 0x00000002;
const DWORD		KEY_ENTER	= 0x00000004;
const DWORD		KEY_SPACE	= 0x00000008;
const DWORD		KEY_1		= 0x00000010;
const DWORD		KEY_2		= 0x00000020;
const DWORD		KEY_3		= 0x00000040;
const DWORD		KEY_4		= 0x00000080;
