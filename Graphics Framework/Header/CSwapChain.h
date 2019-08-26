#pragma once
#include "Header.h"
#include "CRenderTarget.h"
#include "CDepthStencil.h"

class CDevice;

class CSwapChain
{
public:
	
	CRenderTarget m_renderTarget;
	CDepthStencil m_depthStencil;

#ifdef OPEN_GL
	unsigned int m_id;			
	bool Initialize(Color color, Size size);
#endif
#ifdef DIRECT_X
	DXGI_SWAP_CHAIN_DESC				m_description;
	IDXGISwapChain*             m_pointer;
	
	void Initialize(Color color, Size size, HWND hWindow);
#endif

	Color												m_color;

	void Resize(CDevice& device, Size size);
	void Present(unsigned int SyncInterval, unsigned int flags);

	CSwapChain();
	~CSwapChain();
};