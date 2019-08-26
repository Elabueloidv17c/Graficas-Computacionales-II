#include "..\Header\CRasterizerState.h"

void CRasterizerState::Initialize()
{
#ifdef DIRECT_X
	ZeroMemory(&m_pointer, sizeof(m_pointer));
	ZeroMemory(&m_description, sizeof(m_description));


	m_description.FillMode = D3D11_FILL_SOLID;
	m_description.CullMode = D3D11_CULL_BACK;
	m_description.FrontCounterClockwise = false;
	m_description.DepthBias = 0;
	m_description.SlopeScaledDepthBias = 0.0f;
	m_description.DepthBiasClamp = 0.0f;
	m_description.DepthClipEnable = true;
	m_description.ScissorEnable = false;
	m_description.MultisampleEnable = false;
	m_description.AntialiasedLineEnable = false;
#endif
}

#ifdef DIRECT_X
void CRasterizerState::Initialize(D3D11_FILL_MODE FillMode, D3D11_CULL_MODE CullMode, bool FrontCounterClockwise, int DepthBias,
float DepthBiasClamp, float SlopeScaledDepthBias, bool  DepthClipEnable, bool ScissorEnable, bool MultisampleEnable,
bool AntialiasedLineEnable)
{
	ZeroMemory(&m_pointer, sizeof(m_pointer));
	ZeroMemory(&m_description, sizeof(m_description));


	m_description.FillMode = FillMode;
	m_description.CullMode = CullMode;
	m_description.FrontCounterClockwise = FrontCounterClockwise;
	m_description.DepthBias = DepthBias;
	m_description.DepthBiasClamp = DepthBiasClamp;
	m_description.SlopeScaledDepthBias = SlopeScaledDepthBias;
	m_description.DepthClipEnable = DepthClipEnable;
	m_description.ScissorEnable = ScissorEnable;
	m_description.MultisampleEnable = MultisampleEnable;
	m_description.AntialiasedLineEnable = AntialiasedLineEnable;
}
#endif