#pragma once
#include "Header.h"
#include "CTexture.h"

class CRenderTarget
{
public:
#ifdef OPEN_GL
	std::vector <CTexture*> m_view;
#endif

#ifdef DIRECT_X
	ID3D11RenderTargetView* m_view;
	D3D11_RENDER_TARGET_VIEW_DESC	m_description;
#endif 

	CRenderTarget();
	~CRenderTarget();

#ifdef OPEN_GL
	void Initialize(SwapChainData renderTargetData);
	void Erase();
#endif
};