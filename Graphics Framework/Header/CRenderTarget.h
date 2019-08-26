#pragma once
#include "Header.h"
#include "CTexture.h"

class CRenderTarget
{
public:

#ifdef DIRECT_X
	D3D11_RENDER_TARGET_VIEW_DESC		m_description;
	ID3D11RenderTargetView*					m_view;
#endif 
#ifdef OPEN_GL
	unsigned int										m_id;
#endif

	CTexture												m_texture;

	void Initialize(Size size);

	CRenderTarget();
	~CRenderTarget();
};