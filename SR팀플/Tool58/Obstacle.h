#ifndef Obstacle_h__
#define Obstacle_h__

#include "GameObject.h"

namespace	ENGINE
{
	class CTransform;
	class CTexture;
}

class CCameraObserver;
class CObstacle :
	public ENGINE::CGameObject
{
public:
	virtual ~CObstacle(void);
protected:
	explicit CObstacle(LPDIRECT3DDEVICE9 pDevice);

protected:
	ENGINE::CTexture*		m_pTexture;

public:
	ENGINE::VTXCUBE*		m_pVertex;
	ENGINE::VTXCUBE*		m_pConvertVertex;
	DWORD					m_dwVtxCnt;

	int						m_iflag;
	int						m_iStageFlag;

public:
	ENGINE::CTransform*		m_pInfo;
	wstring					m_wstrObjKey;

public:
	CCameraObserver*		m_pCameraObserver;

public:
	virtual HRESULT	Initialize(void);
	virtual void	Update(void);
	virtual void	Render(void);
	virtual const ENGINE::VTXTEX*	GetTerrainVertex(void);
	virtual const ENGINE::VTXCUBE*  GetCubeVertex(void);
	virtual const D3DXVECTOR3*		GetPos(void);

	ENGINE::CTransform* GetInfo(void);
	wstring				GetObjKey(void);

	ENGINE::VTXCUBE* GetpVertex(void)
	{
		return m_pVertex;
	}

	CCameraObserver*	GetCamObserver(void)
	{
		return m_pCameraObserver;
	}

	void TexToPos(void)
	{
		for(int i = 0; i < 8; ++i)
		{
			m_pVertex[i].vPos = m_pVertex[i].vTex;
		}
	}

	map<wstring, ENGINE::CComponent*>*	GetmapComponent(void);
	void	SetTexture(ENGINE::CTexture* pTexture);
	void	ChangeTexture(CObstacle* pCubeObj,const wstring& wstrName);
	
private:
	virtual void		Release(void);
};

#endif // Obstacle_h__