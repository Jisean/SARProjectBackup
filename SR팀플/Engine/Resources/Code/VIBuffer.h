/*!
 * \file VIBuffer.h
 * \date 2016/01/08 13:26
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

#ifndef VIBuffer_h__
#define VIBuffer_h__

#include "Engine_Include.h"
#include "Resources.h"

BEGIN(ENGINE)

class ENGINE_DLL CVIBuffer : public CResources
{ 
public:
	enum BUFFERTYPE	{ BUFFER_TRICOL, BUFFER_RCCOL, BUFFER_TERRAIN,
		BUFFER_RCTEX, TERRAIN_TEX, BUFFER_CUBETEX, BUFFER_RCUI, BUFFER_RCTARGET  };


protected:
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;

protected:
	DWORD						m_dwVtxSize;
	DWORD						m_dwVtxCount;
	DWORD						m_dwVtxFVF;
	DWORD						m_dwTriCnt;
	DWORD						m_dwIdxSize;
	D3DFORMAT					m_IdxFmt;

public:
	void	GetVtxInfo(void* pVertex);
	void	SetVtxInfo(void* pVertex);


public:
	virtual HRESULT		CreateBuffer(void);
	virtual CResources* CloneResource(void);

public:
	virtual void	Render();
	void	Release(void);

protected:
	explicit CVIBuffer(LPDIRECT3DDEVICE9 pDevice);
public:
	virtual ~CVIBuffer(void);

};

END

#endif // VIBuffer_h__