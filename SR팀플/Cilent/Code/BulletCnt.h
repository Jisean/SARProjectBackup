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

#ifndef BulletCnt_h__
#define BulletCnt_h__

#include "GameObject.h"
#include "enum.h"

namespace ENGINE
{
	class CTexture;
	class CVIBuffer;

	class CResourcesMgr;
	class CTimeMgr;
}

class CBulletCnt
	: public ENGINE::CGameObject
{
private:
	BULLETDIGIT					m_eDigit;
	ENGINE::CVIBuffer*			m_pBuffer;
	ENGINE::CTexture*			m_pTexture[2];

	ENGINE::CResourcesMgr*		m_pResourceMgr;
public:
	static GUNTYPE				m_GunType;
public:
	void SetBulletType(GUNTYPE eType)
	{
		m_GunType = eType;
	}
	void setDigit(BULLETDIGIT eType)
	{
		m_eDigit = eType;
	}
	virtual void Update(void);
	virtual void Render(void);

private:
	virtual HRESULT	Initialize(void);
	HRESULT	AddComponent(void);
	void	Release(void);

public:
	static CBulletCnt*	Create(LPDIRECT3DDEVICE9 pDevice);

private:
	explicit CBulletCnt(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CBulletCnt(void);

};
#endif // BulletCnt_h__