#pragma once

template<typename T>
static void Safe_Delete(T& Temp)
{
	if(Temp)
	{
		delete Temp;
		Temp = NULL;
	}
}

static D3DXVECTOR3	GetMouse(void)
{
	POINT Pt;

	GetCursorPos(&Pt);
	ScreenToClient(g_hWnd, &Pt);

	return D3DXVECTOR3(float(Pt.x), float(Pt.y), 0.f);
}

struct DELETEMAP
{
	template<typename T>
	void operator()(T& Temp)
	{
		if(Temp.second)
		{
			delete Temp.second;
			Temp.second = NULL;
		}
	}
};

struct DELETEOBJ
{
	template<typename T>
	void operator()(T& Temp)
	{
		if(Temp)
		{
			delete Temp;
			Temp = NULL;
		}
	}
};