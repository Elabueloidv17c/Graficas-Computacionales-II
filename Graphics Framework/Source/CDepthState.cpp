#include "../Header/CDepthState.h"

#ifdef OPEN_GL
void CDepthState::Initialize(bool enable, unsigned int mode)
{
	m_enable = enable;
	m_mode = mode;
	glDisable(GL_DEPTH_TEST);
}
#endif

CDepthState::~CDepthState()
{
#ifdef DIRECT_X
	if (m_pointer)
	{
		m_pointer->Release();
	}
#endif
}

void CDepthState::Initialize()
{
#ifdef DIRECT_X
	ZeroMemory(&m_pointer, sizeof(m_pointer));
	ZeroMemory(&m_description, sizeof(m_description));

	m_description.DepthEnable = true;
	m_description.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	m_description.DepthFunc = D3D11_COMPARISON_LESS;
	m_description.StencilEnable = true;
	m_description.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	m_description.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	m_description.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	m_description.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	m_description.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	m_description.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	m_description.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	m_description.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	m_description.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	m_description.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
#endif
#ifdef OPEN_GL
	m_enable = true;
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
#endif
}

#ifdef DIRECT_X
void CDepthState::Initialize(bool DepthEnable, D3D11_DEPTH_WRITE_MASK DepthWriteMask, D3D11_COMPARISON_FUNC DepthFunc, 
bool StencilEnable, unsignedInt8 StencilReadMask, unsignedInt8 StencilWriteMask, D3D11_COMPARISON_FUNC FrontFaceStencilFunc,
D3D11_STENCIL_OP FrontFaceStencilDepthFailOp, D3D11_COMPARISON_FUNC	BackFaceStencilFunc, D3D11_STENCIL_OP	BackFaceStencilDepthFailOp,
D3D11_STENCIL_OP FrontFaceStencilPassOp, D3D11_STENCIL_OP BackFaceStencilPassOp, D3D11_STENCIL_OP FrontFaceStencilFailOp,
D3D11_STENCIL_OP BackFaceStencilFailOp)
{
	ZeroMemory(&m_pointer, sizeof(m_pointer));
	ZeroMemory(&m_description, sizeof(m_description));

	m_description.DepthEnable = DepthEnable;
	m_description.DepthWriteMask = DepthWriteMask;
	m_description.DepthFunc = DepthFunc;
	m_description.StencilEnable = StencilEnable;
	m_description.StencilReadMask = StencilReadMask;
	m_description.StencilWriteMask = StencilWriteMask;
	m_description.FrontFace.StencilFunc = FrontFaceStencilFunc;
	m_description.FrontFace.StencilDepthFailOp = FrontFaceStencilDepthFailOp;
	m_description.BackFace.StencilFunc = BackFaceStencilFunc;
	m_description.BackFace.StencilDepthFailOp = BackFaceStencilDepthFailOp;
	m_description.FrontFace.StencilPassOp = FrontFaceStencilPassOp;
	m_description.BackFace.StencilPassOp = BackFaceStencilPassOp;
	m_description.FrontFace.StencilFailOp = FrontFaceStencilFailOp;
	m_description.BackFace.StencilFailOp = BackFaceStencilFailOp;
}
#endif
