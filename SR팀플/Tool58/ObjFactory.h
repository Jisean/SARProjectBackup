#pragma once

#include "Include.h"
#include "Obj.h"

template<typename T>
class CObjFactory
{
public:
	static	CObj* CreateObj()
	{
		CObj*		pObj = new T;
		pObj->Initialize();

		return pObj;
	}


public:
	CObjFactory(void){	}
	~CObjFactory(void){	}
};
