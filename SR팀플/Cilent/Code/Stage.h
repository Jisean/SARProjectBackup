/*!
 * \file Stage.h
 * \date 2016/01/06 14:51
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

#ifndef Stage_h__
#define Stage_h__

#include "Scene.h"

namespace ENGINE
{
	class CManagement;
	class CResourcesMgr;
	class CLayer;
}

class CObstacle;
class CStage  : public ENGINE::CScene
{
public:
	enum LAYERID { LAYER_ENVIRONMENT, LAYER_GAMELOGIC, LAYER_UI };

private:
	ENGINE::CManagement*		m_pManagement;
	ENGINE::CResourcesMgr*		m_pResourceMgr;

	ENGINE::CGameObject*			m_pPlayer;
	bool						m_bInitHp;

	list<wstring>			m_KeyList;
private:
	explicit CStage(LPDIRECT3DDEVICE9 pDevice);
public:
	~CStage(void);

public:
	static CStage* Create(LPDIRECT3DDEVICE9	pDevice);

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
	HRESULT CreateBack(ENGINE::CLayer* pLayer);

};



#endif // Stage_h__