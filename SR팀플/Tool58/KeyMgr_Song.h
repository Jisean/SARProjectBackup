#pragma once

class KeyMgr_Song
{
	//�츮�� ������ Ű�� ���� �ִ����� ���� ���°��� ������ �� �־���..
#define		MAX_VIRTUAL_KEY	0xEF
private:
	bool	m_bKey[MAX_VIRTUAL_KEY];
public:
	void	Update(void);

	//�̰� ���� Ű�� ���� �ֳ�?
	bool	GetKeyState(int	iKey);
	//Ű�� ������ �� ���� �� ��.
	bool	GetKeyDown(int iKey);
	//Ű�� ������ �� �� ��..
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
