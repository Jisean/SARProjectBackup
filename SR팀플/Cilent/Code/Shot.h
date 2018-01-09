/*!
 * \file Shot.h
 * \date 2016/02/15 13:24
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


#ifndef Shot_h__
#define Shot_h__


#include "Scene.h"

namespace ENGINE
{
	class CManagement;
	class CResourcesMgr;
	class CGraphicDev;
	class CTimeMgr;
}

class CBack;
class CTerrain;
class CObstacle;
class CShot  : public ENGINE::CScene
{
public:
	enum LAYERID { LAYER_ENVIRONMENT, LAYER_GAMELOGIC, LAYER_UI, LAYER_TARGET,
	LAYER_TARGET2, LAYER_TARGET3};

private:
	// Å¸°Ù Á¨ ½Ã°£
	float				m_fTargetZen;
	float				m_fDragoonZen;
	float				m_fSkullZen;

	bool				m_bStartStage;
	int					m_iCount;
	int					m_iCBookTemp[2];
	int					m_iDragonBookTemp[2];
	int					m_iSkullBookTemp[2];

	float				m_fTime;
private:
	ENGINE::CManagement*		m_pManagement;
	ENGINE::CResourcesMgr*		m_pResourceMgr;
	ENGINE::CGraphicDev*		m_pGraphicDev;
	ENGINE::CGameObject*			m_pPlayer;
	ENGINE::CTimeMgr*			m_pTimeMgr;

	list<wstring>			m_KeyList;
	ENGINE::CLayer*		pLayer2;
private:
	explicit CShot(LPDIRECT3DDEVICE9 pDevice);
public:
	~CShot(void);

public:
	static CShot* Create(LPDIRECT3DDEVICE9	pDevice);

	void	ImgPath(void);
	void	Load(ENGINE::CLayer* pLayer);

public:
	virtual void Update(void);
	virtual void Render(void);

private:
	virtual HRESULT Initialize(void);

	HRESULT	Add_Environment_Layer(void);
	HRESULT Add_GameLogic_Layer(void);	
	HRESULT Add_UI_Layer(void);

	HRESULT Add_Target_Layer(void);

	void	SetUp(void);
	void Release(void);

	void ChangeTexture(CObstacle* pCubeObj, const wstring& wstrName);
	void ChangeTerrain(CTerrain* pCubeObj, const wstring& wstrName);
	void ChangeBack(CBack* pCubeObj, const wstring& wstrName);
	HRESULT CreateBack(ENGINE::CLayer* pLayer);


};



#endif // Shot_h__