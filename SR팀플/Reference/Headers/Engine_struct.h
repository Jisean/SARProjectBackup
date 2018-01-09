/*!
 * \file Engine_struct.h
 * \date 2016/01/08 13:48
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

#ifndef Engine_struct_h__
#define Engine_struct_h__

namespace	ENGINE
{
	typedef struct tagVertexColor
	{
		D3DXVECTOR3		vPos;
		DWORD			dwColor;

	}VTXCOL;

	const DWORD		VTXFVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;

	typedef struct tagVertexTexture
	{
		D3DXVECTOR3		vPos;
		D3DXVECTOR2		vTex;

	}VTXTEX;

	const DWORD		VTXFVF_TEX = D3DFVF_XYZ | D3DFVF_TEX1;

	typedef struct tagVertexCube
	{
		D3DXVECTOR3		vPos;
		D3DXVECTOR3		vTex;

	}VTXCUBE;

	const DWORD		VTXFVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);

	typedef	struct tagIndex16
	{
		WORD		_1, _2, _3;

	}INDEX16;

	typedef	struct tagIndex32
	{
		DWORD		_1, _2, _3;

	}INDEX32;

}

#endif // Engine_struct_h__