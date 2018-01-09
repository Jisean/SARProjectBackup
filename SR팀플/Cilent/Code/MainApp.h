/*!
 * \file MainApp.h
 * \date 2016/01/05 14:16
 *
 * \author Yoon Myung
 * Contact: Yoon Myung@company.com
 *
 * \brief : 메인 게임 클래스
 *
 * TODO: long description
 *
 * \note
*/

#ifndef MainApp_h__
#define MainApp_h__

namespace ENGINE
{
	class CGraphicDev;
	class CManagement;
	class CTimeMgr;
	class CResourcesMgr;
}

class CMainApp
{
private:
	ENGINE::CGraphicDev*		m_pGraphicDev;
	ENGINE::CManagement*		m_pManagement;
	ENGINE::CTimeMgr*			m_pTimeMgr;
	ENGINE::CResourcesMgr*		m_pResourceMgr;
	LPDIRECT3DDEVICE9			m_pDevice;

public:
	static CMainApp*		Create(void);

public:
	void	Update(void);
	void	Render(void);

private:
	HRESULT	Initialize(void);
	void	Release(void);

private:
	CMainApp(void);
public:
	~CMainApp();
};

#endif // MainApp_h__