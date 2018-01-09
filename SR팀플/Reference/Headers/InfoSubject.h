#ifndef InfoSubject_h__
#define InfoSubject_h__

#include "Subject.h"

BEGIN(ENGINE)

typedef list<void*>				DATALIST;
typedef map<int, DATALIST>		MAPDATALIST;

class ENGINE_DLL CInfoSubject : public CSubject
{
DECLARE_SINGLETON(CInfoSubject)

private:
	CInfoSubject(void);
	virtual ~CInfoSubject(void);

public:
	void		AddData(int iMessage, void* pData);
	void		RemoveData(int iMessage, void* pData);
	DATALIST*	GetDataList(int iMessage);	

private:
	void		Release(void);

private:
	MAPDATALIST			m_MapDataList;

};

END

#endif // InfoSubject_h__
