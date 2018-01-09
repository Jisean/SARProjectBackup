/*!
 * \file CrossLine.cpp
 * \date 2016/02/12 20:24
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

#ifndef CrossLine_h__
#define CrossLine_h__

#include "GameObject.h"

namespace ENGINE
{
	class CTexture;
	class CVIBuffer;

	class CResourcesMgr;
	class CTimeMgr;
}

class CCrossLine
	: public ENGINE::CGameObject
{
private:
	ENGINE::CVIBuffer*			m_pBuffer;
	ENGINE::CTexture*			m_pTexture;

	ENGINE::CResourcesMgr*		m_pResourceMgr;

public:
	virtual void Update(void);
	virtual void Render(void);

private:
	virtual HRESULT	Initialize(void);
	HRESULT	AddComponent(void);
	void	Release(void);

public:
	static CCrossLine*	Create(LPDIRECT3DDEVICE9 pDevice);

private:
	explicit CCrossLine(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CCrossLine(void);

};
#endif // CrossLine_h__