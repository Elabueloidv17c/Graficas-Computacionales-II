#pragma once
#include "Header.h"

class CRasterizerState
{
public:

#ifdef DIRECT_X
	D3D11_RASTERIZER_DESC m_description;
	ID3D11RasterizerState* m_pointer;

	void Initialize(D3D11_FILL_MODE FillMode, D3D11_CULL_MODE CullMode, bool FrontCounterClockwise, int DepthBias,
	float DepthBiasClamp, float SlopeScaledDepthBias, bool  DepthClipEnable, bool ScissorEnable, bool MultisampleEnable,
	bool AntialiasedLineEnable);
#endif
#ifdef OPEN_GL
	unsigned int m_id;
#endif

	void Initialize();
};