/*!
 * \file Ending.h
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

#ifndef Ending_h__
#define Ending_h__

#include "GameObject.h"
#include "Scene.h"
namespace ENGINE
{
	class CManagement;
	class CResourcesMgr;
	class CVIBuffer;
	class CTexture;
	class CGraphicDev;
}

class CEnding  : public ENGINE::CScene
{
public:
	enum LAYERID { LAYER_ENVIRONMENT, LAYER_GAMELOGIC, /*LAYER_UI*/ };

private:
	ENGINE::CManagement*		m_pManagement;
	ENGINE::CResourcesMgr*		m_pResourceMgr;
	ENGINE::CGraphicDev*		m_pGraphicDev;
private:
	explicit CEnding(LPDIRECT3DDEVICE9 pDevice);

public:
	~CEnding(void);

public:
	static CEnding* Create(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual void Update(void);
	virtual void Render(void);
/////////
private:

	ENGINE::CVIBuffer*	m_pBuffer;
	ENGINE::CTexture*	m_pTexture;

/////////
private:
	virtual HRESULT Initialize(void);
	
	HRESULT	Add_Environment_Layer(void);
	HRESULT Add_GameLogic_Layer(void);	
	
	void	Release(void);
	void	SetUp(void);

};
#endif // Ending_h__