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


#ifndef Stage2_h__
#define Stage2_h__


#include "Scene.h"

namespace ENGINE
{
	class CManagement;
	class CResourcesMgr;
}

class CBack;
class CTerrain;
class CObstacle;
class CStage2  : public ENGINE::CScene
{
public:
	enum LAYERID { LAYER_ENVIRONMENT, LAYER_GAMELOGIC, LAYER_UI };

private:
	ENGINE::CManagement*		m_pManagement;
	ENGINE::CResourcesMgr*		m_pResourceMgr;

	ENGINE::CGameObject*			m_pPlayer;

	list<wstring>			m_KeyList;
	bool						m_bInitHp;
private:
	explicit CStage2(LPDIRECT3DDEVICE9 pDevice);
public:
	~CStage2(void);

public:
	static CStage2* Create(LPDIRECT3DDEVICE9	pDevice);

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

	void	SetUp(void);
	void Release(void);

	void ChangeTexture(CObstacle* pCubeObj, const wstring& wstrName);
	void ChangeTerrain(CTerrain* pCubeObj, const wstring& wstrName);
	void ChangeBack(CBack* pCubeObj, const wstring& wstrName);
	HRESULT CreateBack(ENGINE::CLayer* pLayer);


};



#endif // Stage2_h__