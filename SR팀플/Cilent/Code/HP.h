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

#ifndef HP_h__
#define HP_h__

#include "GameObject.h"
#include "enum.h"

namespace ENGINE
{
	class CTexture;
	class CVIBuffer;

	class CResourcesMgr;
	class CTimeMgr;
}

class CHP
	: public ENGINE::CGameObject
{
private:
	HPDIGIT						m_DigitType;
	ENGINE::CVIBuffer*			m_pBuffer;
	ENGINE::CTexture*			m_pTexture[3];

	ENGINE::CResourcesMgr*		m_pResourceMgr;

public:
	void SetDigit(HPDIGIT eType)
	{
		m_DigitType = eType;
	}
	virtual void Update(void);
	virtual void Render(void);

private:
	virtual HRESULT	Initialize(void);
	HRESULT	AddComponent(void);
	void	Release(void);

public:
	static CHP*	Create(LPDIRECT3DDEVICE9 pDevice);

private:
	explicit CHP(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CHP(void);

};
#endif // HP_h__