#pragma once
#include "Header.h"
#include "CTexture.h"

class CDepthStencil
{
public:
#ifdef OPEN_GL
	unsigned int m_id;

#endif

#ifdef DIRECT_X
	D3D11_DEPTH_STENCIL_VIEW_DESC				m_description;
	ID3D11DepthStencilView*             m_view;
	CTexture														m_texture;
#endif

	void Initialize(Size size, bool stencil);

	CDepthStencil();
	~CDepthStencil();
};