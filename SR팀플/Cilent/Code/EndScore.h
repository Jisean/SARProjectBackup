/*!
 * \file EndScore.h
 * \date 2016/01/06 14:46
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

#ifndef EndScore_h__
#define EndScore_h__

#include "GameObject.h"

namespace ENGINE
{
	class CTexture;
	class CVIBuffer;

	class CResourcesMgr;
	class CTimeMgr;
}

class CEndScore  : public ENGINE::CGameObject
{
public:
	enum LAYERID { LAYER_ENVIRONMENT, LAYER_GAMELOGIC, /*LAYER_UI*/ };
private:
	ENGINE::CVIBuffer*			m_pBuffer;
	ENGINE::CTexture*			m_pTexture;

	ENGINE::CResourcesMgr*		m_pResourceMgr;
	ENGINE::CTimeMgr*			m_pTimeMgr;

private:
	explicit CEndScore(LPDIRECT3DDEVICE9 pDevice);

public:
	~CEndScore(void);

public:
	static CEndScore* Create(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual void Update(void);
	virtual void Render(void);

private:
	virtual HRESULT Initialize(void);
	HRESULT AddComponent(void);	
	void	Release(void);

};
#endif // EndScore_h__