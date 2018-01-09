#include "InfoSubject.h"

IMPLEMENT_SINGLETON(ENGINE::CInfoSubject)

ENGINE::CInfoSubject::CInfoSubject(void)
{

}

ENGINE::CInfoSubject::~CInfoSubject(void)
{
	Release();
}

void ENGINE::CInfoSubject::AddData(int iMessage, void* pData)
{
	if(pData)
	{
		MAPDATALIST::iterator	iter = m_MapDataList.find(iMessage);

		if(iter == m_MapDataList.end())
		{
			m_MapDataList[iMessage] = DATALIST();
		}
		m_MapDataList[iMessage].push_back(pData);
		Notify(iMessage);

	}
}

void ENGINE::CInfoSubject::RemoveData(int iMessage, void* pData)
{
	MAPDATALIST::iterator		iter = m_MapDataList.find(iMessage);

	if(iter != m_MapDataList.end())
	{
		DATALIST::iterator	iterList = iter->second.begin();
		DATALIST::iterator	iterList_end = iter->second.end();

		for( ; iterList != iterList_end; ++iterList)
		{
			if((*iterList) == pData)
			{
				iter->second.erase(iterList);
				return;
			}
		}
	}
}

ENGINE::DATALIST* ENGINE::CInfoSubject::GetDataList(int iMessage)
{
	MAPDATALIST::iterator		iter = m_MapDataList.find(iMessage);

	if(iter == m_MapDataList.end())
		return NULL;

	return &iter->second;
}

void ENGINE::CInfoSubject::Release(void)
{
	for(MAPDATALIST::iterator	iter = m_MapDataList.begin();
		iter != m_MapDataList.end(); ++iter)
	{
		iter->second.clear();
	}
	m_MapDataList.clear();
}

