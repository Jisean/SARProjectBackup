#pragma once

using namespace std;

typedef struct tagInfo
{
	D3DXVECTOR3		vPos;
	D3DXVECTOR3		vLook;
	D3DXVECTOR3		vDir;
	D3DXVECTOR3		vSize;

	D3DXMATRIX		matWorld;

}INFO;

typedef struct tagTexture
{
	LPDIRECT3DTEXTURE9		pTexture;	// �̹��� ������ �����ϱ� ���� ��ü
	D3DXIMAGE_INFO			tImgInfo;	// �������� �̹��� ���� �����͸� 
										// �����ϱ� ���� ����ü

}TEXINFO;

typedef struct tagTile
{
	D3DXVECTOR3		vPos;
	D3DXVECTOR3		vSize;
	BYTE			byOption;
	BYTE			byDrawID;

}TILE;

typedef struct tagUnitData
{
	wstring			wstrName;
	int				iAttack;
	int				iHp;
	BYTE			byJobIndex;
	BYTE			byItem;

}UNITDATA;

typedef struct tagTexturePath
{
	wstring			wstrObjKey;
	wstring			wstrStateKey;
	wstring			wstrPath;
	int				iCount;

	tagTexturePath(void)
	: wstrObjKey(L""), wstrStateKey(L""), wstrPath(L""), iCount(0)
	{

	}


}IMGPATH;

typedef struct tagFrame
{
	float		fFrame;
	float		fCount;
	float		fMax;

	tagFrame(){}
	tagFrame(float _fFrame, float _fCount, float _fMax)
		: fFrame(_fFrame), fCount(_fCount), fMax(_fMax)
	{

	}

}FRAME;

typedef struct tagAStarNode
{
	float			fCost;
	int				iIndex;
	tagAStarNode*	pParent;

}NODE;

typedef struct tagData
{
	int			iHp;
	int			iMp;

}DATA;