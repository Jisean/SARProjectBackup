/*!
 * \file Logo.h
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

#ifndef Logo_h__
#define Logo_h__

#include "Scene.h"
namespace ENGINE
{
	class CManagement;
	class CResourcesMgr;
	class CVIBuffer;
	class CTexture;
	class CGraphicDev;
}

class CLogo  : public ENGINE::CScene
{
public:
	enum LAYERID { LAYER_ENVIRONMENT, LAYER_GAMELOGIC, /*LAYER_UI*/ };

private:
	ENGINE::CManagement*		m_pManagement;
	ENGINE::CResourcesMgr*		m_pResourceMgr;
	ENGINE::CGraphicDev*		m_pGraphicDev;
public:
	static ENGINE::CResourcesMgr* m_pStaticResourceMgr;
	static LPDIRECT3DDEVICE9	m_pStaticDevice;

private:
	explicit CLogo(LPDIRECT3DDEVICE9 pDevice);

public:
	~CLogo(void);

public:
	static CLogo* Create(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual void Update(void);
	virtual void Render(void);
/////////
private:
	static int			m_iImageCnt;
	HANDLE				m_hThread;
	CRITICAL_SECTION	m_Crt;
	ENGINE::CVIBuffer*	m_pBuffer;
	ENGINE::CTexture*	m_pTexture;
public:
	static unsigned int __stdcall LoadThread(void* pArg);
	CRITICAL_SECTION	GetCrt(void);
	static void	ImgPath(void);
/////////
private:
	virtual HRESULT Initialize(void);
	
	HRESULT	Add_Environment_Layer(void);
	HRESULT Add_GameLogic_Layer(void);	
	HRESULT Add_UI_Layer(void);


	void	Release(void);
	void	SetUp(void);

};
#endif // Logo_h__