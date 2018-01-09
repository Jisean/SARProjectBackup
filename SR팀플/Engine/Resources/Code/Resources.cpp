#include "Resources.h"

WORD ENGINE::CResources::m_wRefCnt = 0;

ENGINE::CResources::CResources(LPDIRECT3DDEVICE9 pDevice)
: m_pDevice(pDevice)
{

}

ENGINE::CResources::~CResources()
{
	
}

void ENGINE::CResources::Release(void)
{

}
