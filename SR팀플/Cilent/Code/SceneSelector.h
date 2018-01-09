/*!
 * \file SceneSelector.h
 * \date 2016/01/07 14:25
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
#ifndef SceneSelector_h__
#define SceneSelector_h__

#include "Include.h"
#include "Logo.h"
#include "Stage.h"
#include "Shot.h"
#include "Stage1.h"
#include "Stage2.h"
#include "Ending.h"

class CSceneSelector
{
private:
	SCENEID		m_eScene;

public:
	explicit CSceneSelector(SCENEID	_eScene)
		: m_eScene(_eScene) {}

public:
	HRESULT	operator()(ENGINE::CScene** ppScene, LPDIRECT3DDEVICE9  pDevice)
	{
		switch(m_eScene)
		{
		case SC_LOGO:
			*ppScene = CLogo::Create(pDevice);
			break;

		case SC_STAGE:
			*ppScene = CStage::Create(pDevice);
			break;

		case SC_SHOT:
			*ppScene = CShot::Create(pDevice);
			break;

		case SC_STAGE1:
			*ppScene = CStage1::Create(pDevice);
			break;
		case SC_STAGE2:
			*ppScene = CStage2::Create(pDevice);
			break;
		case SC_ENDING:
			*ppScene = CEnding::Create(pDevice);
			break;


		}

		NULL_CHECK_RETURN(*ppScene, E_FAIL);

		return S_OK;
	}

};


#endif // SceneSelector_h__
