#pragma once

class KeyMgr_Song
{
	//우리가 기존에 키가 눌려 있는지에 대한 상태값만 조사할 수 있었다..
#define		MAX_VIRTUAL_KEY	0xEF
private:
	bool	m_bKey[MAX_VIRTUAL_KEY];
public:
	void	Update(void);

	//이건 현재 키가 눌려 있냐?
	bool	GetKeyState(int	iKey);
	//키가 눌렸을 때 최초 한 번.
	bool	GetKeyDown(int iKey);
	//키가 떨어질 떄 한 번..
	bool	GetKeyUp(int iKey);

private:
	static KeyMgr_Song*	m_pInstance;
public:
	static KeyMgr_Song*	GetInstance(void)
	{
		if(m_pInstance == NULL)
			m_pInstance = new KeyMgr_Song;

		return m_pInstance;
	}
	void	DestroyInstance(void)
	{
		if(m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = NULL;
		}
	}
private:
	KeyMgr_Song(void);
	~KeyMgr_Song(void);
};
