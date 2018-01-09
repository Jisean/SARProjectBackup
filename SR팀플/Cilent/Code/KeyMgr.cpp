#include "StdAfx.h"
#include "KeyMgr.h"
#include "value.h"

IMPLEMENT_SINGLETON(CKeyMgr)

CKeyMgr::CKeyMgr(void)
: m_dwKey(0)
, m_dwKeyDown(0)
, m_dwKeyPressed(0)
{
	ZeroMemory(m_dwKeyPresseds, sizeof(m_dwKeyPresseds));
}

CKeyMgr::~CKeyMgr(void)
{
}

void CKeyMgr::KeyCheck(void)
{
	m_dwKey = 0;

	if(GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		m_dwKey |= KEY_LBUTTON;

	if(GetAsyncKeyState(VK_RBUTTON) & 0x8000)
		m_dwKey |= KEY_RBUTTON;

	if(GetAsyncKeyState(VK_SPACE) & 0x8000)
		m_dwKey |= KEY_SPACE;

	if(GetAsyncKeyState(VK_RETURN) & 0x8000)
		m_dwKey |= KEY_ENTER;

	if(GetAsyncKeyState('1') & 0x8000)
		m_dwKey |= KEY_1;

	if(GetAsyncKeyState('2') & 0x8000)
		m_dwKey |= KEY_2;

	if(GetAsyncKeyState('3') & 0x8000)
		m_dwKey |= KEY_3;

	if(GetAsyncKeyState('4') & 0x8000)
		m_dwKey |= KEY_4;


}

bool CKeyMgr::KeyPressed(DWORD _dwKey, int index)
{
	if(m_dwKey & _dwKey)
	{
		m_dwKeyPresseds[index] |= _dwKey;
		return false;
	}
	else if(m_dwKeyPresseds[index] & _dwKey)
	{
		m_dwKeyPresseds[index] = m_dwKey;
		return true;
	}

	return false;
}

bool CKeyMgr::KeyPressed(DWORD _dwKey)
{
	if(m_dwKey & _dwKey)
	{
		m_dwKeyPressed |= _dwKey;
		return false;
	}
	else if(m_dwKeyPressed & _dwKey)
	{
		m_dwKeyPressed = m_dwKey;
		return true;
	}

	return false;
}
							
bool CKeyMgr::KeyDown(DWORD _dwKey, int index)
{
	if(KeyPressed(_dwKey))
		m_dwKeyDown ^= _dwKey;

	if(!(m_dwKeyDown & _dwKey))
	{
		if(m_dwKey & _dwKey)
		{
			m_dwKeyDown |= _dwKey;
			return true;
		}
	}

	return false;
}
bool CKeyMgr::KeyDown(DWORD _dwKey)
{
	if(KeyPressed(_dwKey))
		m_dwKeyDown ^= _dwKey;

	if(!(m_dwKeyDown & _dwKey))
	{
		if(m_dwKey & _dwKey)
		{
			m_dwKeyDown |= _dwKey;
			return true;
		}
	}

	return false;
}

bool CKeyMgr::KeyCombine(DWORD _dwFirstKey, DWORD _dwSecondKey)
{
	if(m_dwKey & _dwFirstKey)
	{
		if(KeyDown(_dwSecondKey))
			return true;
	}

	return false;
}
