/*!
 * \file Component.h
 * \date 2016/01/18 14:17
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
#ifndef Component_h__
#define Component_h__

#include "Engine_Include.h"

BEGIN(ENGINE)

class ENGINE_DLL	CComponent	//interface //abstract	
{
protected:
	CComponent(void);
public:
	virtual ~CComponent() = 0;


public:
	virtual void Update() {};
};

END
#endif // Component_h__