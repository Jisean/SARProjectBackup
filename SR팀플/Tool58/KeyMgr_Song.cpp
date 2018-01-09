#include "StdAfx.h"
#include "KeyMgr_Song.h"

KeyMgr_Song*	KeyMgr_Song::m_pInstance = NULL;

KeyMgr_Song::KeyMgr_Song(void)
{
	memset(m_bKey, 0, sizeof(m_bKey));
}

KeyMgr_Song::~KeyMgr_Song(void)
{
}



bool KeyMgr_Song::GetKeyState(int iKey)
{
	if(GetAsyncKeyState(iKey))
		return true;
	
	return false;
}

bool KeyMgr_Song::GetKeyDown(int iKey)
{
	if(!m_bKey[iKey] && GetAsyncKeyState(iKey))
	{
		m_bKey[iKey] = !m_bKey[iKey];
		return true;
	}

	return false;
}

bool KeyMgr_Song::GetKeyUp(int iKey)
{
	if(m_bKey[iKey] && !GetAsyncKeyState(iKey))
	{
		m_bKey[iKey] = !m_bKey[iKey];
		return true;
	}
	return false;
}

void KeyMgr_Song::Update(void)
{
	for(int i = 0; i < MAX_VIRTUAL_KEY; ++i)
	{
		if(!m_bKey[i] && GetAsyncKeyState(i))
			m_bKey[i] = !m_bKey[i];
		else if(m_bKey[i] && !GetAsyncKeyState(i))
			m_bKey[i] = !m_bKey[i];
	}
}