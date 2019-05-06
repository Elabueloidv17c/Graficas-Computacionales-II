#pragma once
#include "Header.h"
#include "CRenderTarget.h"
#include "CDepthStencil.h"

class CSwapChain
{
public:
	
	CRenderTarget m_renderTarget;
	CDepthStencil m_depthStencil;

#ifdef OPEN_GL
	SwapChainData m_data;
	unsigned int m_id;
									
	bool Initialize(SwapChainData renderTargetData);
	bool AddRenderTarget(TextureDescription textureData);
	void SetSwapChain(Rect* viewPort);						
	bool Resize(Rect bufferSize);					
	void Erase();												

	unsigned int GetID();
#endif

#ifdef DIRECT_X
	DXGI_SWAP_CHAIN_DESC				m_description;
	IDXGISwapChain*                     m_pointer;
	Color								m_color;

	void Initialize(Color color, Size size, HWND hWindow);
	void Resize(Size size);
	void Present(UINT SyncInterval, UINT flags);
#endif

	CSwapChain();
	~CSwapChain();
};