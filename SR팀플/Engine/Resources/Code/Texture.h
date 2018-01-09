/*!
 * \file Texture.h
 * \date 2016/01/14 14:35
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

#ifndef Texture_h__
#define Texture_h__

#include "Resources.h"

BEGIN(ENGINE)

class ENGINE_DLL CTexture
	: public CResources
{
private:
	/*LPDIRECT3DTEXTURE9
	LPDIRECT3DCUBETEXTURE9*/
	typedef	vector<IDirect3DBaseTexture9*>			VECTEXTURE;
	VECTEXTURE										m_vecTexture;

	DWORD											m_dwContainerSize;

private:
	HRESULT	LoadTexture(TEXTURETYPE	eTexType, const wstring& wstrFilePath, const WORD& wCnt);

public:
	static CTexture* Create(LPDIRECT3DDEVICE9 pDevice, TEXTURETYPE	eTexType, 
		const wstring& wstrFilePath, const WORD& wCnt);

public:
	
	void	Render(const DWORD& iIndex);
	virtual CResources* CloneResource(void);

	void Release(void);

public:
	static	int		m_iImgCnt;

private:
	explicit CTexture(LPDIRECT3DDEVICE9 pDevice);
public:
	virtual ~CTexture(void);
};

END
#endif // Texture_h__