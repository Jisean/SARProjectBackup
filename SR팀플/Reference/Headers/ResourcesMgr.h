/*!
 * \file ResourcesMgr.h
 * \date 2016/01/13 13:21
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

#ifndef ResourcesMgr_h__
#define ResourcesMgr_h__

#include "Engine_Include.h"
#include "VIBuffer.h"

BEGIN(ENGINE)

class CComponent;
class ENGINE_DLL CResourcesMgr
{
	DECLARE_SINGLETON(CResourcesMgr)

public:
	HRESULT	AddBuffer(LPDIRECT3DDEVICE9 pDevice, RESOURCETYPE eType, 
			ENGINE::CVIBuffer::BUFFERTYPE eBufferType, 
			const wstring& wstrResourceKey, 
			const WORD& wCntX = 0, const WORD& wCntY = 0, 
			const WORD& wItv = 1);

	HRESULT	AddTexture(LPDIRECT3DDEVICE9 pDevice, RESOURCETYPE eType,
			TEXTURETYPE eTextureType, const wstring& wstrResourceKey, 
			const wstring& wstrFilePath, const WORD& wCnt);

	CComponent*	CloneResource(RESOURCETYPE eResourceType, const wstring& wstrResourceKey);

public:
	void	GetVtxInfo(RESOURCETYPE	eResourcetype, 
		const wstring& wstrResourceKey, void* pVertex);

	void	SetVtxInfo(RESOURCETYPE	eResourcetype, 
		const wstring& wstrResourceKey, void* pVertex);

public:
	void	Release(void);
	void	ResetDynamic(void);

private:
	 typedef map<wstring, CResources*>		MAPRESOURCE;
	 MAPRESOURCE							m_MapResource[RESOURCE_END];

private:
	explicit CResourcesMgr();
	~CResourcesMgr();

};
END

#endif // ResourcesMgr_h__