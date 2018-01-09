/*!
 * \file Subject.h
 * \date 2016/01/22 14:40
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
#ifndef Subject_h__
#define Subject_h__

#include "Engine_Include.h"

BEGIN(ENGINE)

class CObserver;
class ENGINE_DLL CSubject
{

protected:
	CSubject(void);
	virtual ~CSubject(void);

public:
	virtual void Subscribe(CObserver* pObserver);		// ������ ���
	virtual void UnSubscribe(CObserver* pObserver);		// ������ ����
	virtual void Notify(int iMessage);					// ���� ����

private:
	void Release(void);

protected:
	typedef list<CObserver*>		OBSERVERLIST;
	OBSERVERLIST					m_ObserverList;

};
END

#endif // Subject_h__