/*!
 * \file Include.h
 * \date 2016/01/05 14:41
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

#ifndef Include_h__
#define Include_h__

#include "enum.h"
#include "value.h"


extern HWND g_hWnd;
extern bool			bPisTol;
extern bool			bRifle;
extern bool			bKnife;
extern bool			bSniper;
extern bool			bStage1;
extern bool			bStage2;
extern bool			bStage3;

extern bool			bStage2Start;
extern bool			bStage2Start2;
extern bool			bStage2Start3;
extern bool			bRedBull;

extern float		fHp;
extern int			iScore;
extern int			iRifleBullet;
extern int			iPistolBullet;
extern bool			bFire;

extern bool			bLevel;
extern bool			bLevel1;
extern bool			bLevelShot;
extern bool			bLevel2;

extern int			iImageCnt;
extern int			iTimeLimit;



extern	int			g_iImgCnt;
typedef struct tagTexture
{
	LPDIRECT3DTEXTURE9		pTexture;	// 이미지 정보를 저장하기 위한 객체
	D3DXIMAGE_INFO			tImgInfo;	// 실제적인 이미지 관련 데이터를 
	// 저장하기 위한 구조체

}TEXINFO;
#define			ERR_MSG(Message)MessageBox(g_hWnd, Message, L"System Error", NULL)

class CStringCompare
{
public:
	explicit CStringCompare(const TCHAR* pKey)
		: m_pString(pKey) {}
	~CStringCompare() {}
public:
	template <typename T>
	bool operator () (T Data)
	{
		return !lstrcmp(Data.first, m_pString);
	}
private:
	const TCHAR*	m_pString;
};
#endif // Include_h__