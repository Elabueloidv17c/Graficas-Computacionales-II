#pragma once
#include "Header.h"

class CDepthState
{
public:

#ifdef DIRECT_X
	D3D11_DEPTH_STENCIL_DESC m_description;
	ID3D11DepthStencilState* m_pointer;

	void Initialize(bool DepthEnable, D3D11_DEPTH_WRITE_MASK DepthWriteMask, D3D11_COMPARISON_FUNC DepthFunc, bool StencilEnable,
		unsignedInt8 StencilReadMask, unsignedInt8 StencilWriteMask, D3D11_COMPARISON_FUNC FrontFaceStencilFunc,
		D3D11_STENCIL_OP FrontFaceStencilDepthFailOp, D3D11_COMPARISON_FUNC	BackFaceStencilFunc, D3D11_STENCIL_OP	BackFaceStencilDepthFailOp,
		D3D11_STENCIL_OP FrontFaceStencilPassOp, D3D11_STENCIL_OP BackFaceStencilPassOp, D3D11_STENCIL_OP FrontFaceStencilFailOp,
		D3D11_STENCIL_OP BackFaceStencilFailOp);
#endif
#ifdef OPEN_GL
	unsigned int m_id;
	unsigned int m_mode;
	bool m_enable;

	void Initialize(bool enable, unsigned int mode);
#endif

	CDepthState() = default;
	~CDepthState();

	void Initialize();
};