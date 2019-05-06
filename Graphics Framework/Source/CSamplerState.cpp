#include "../Header/CSamplerState.h"

#ifdef DIRECT_X
void CSamplerState::Initialize()
{
	ZeroMemory(&m_pointer, sizeof(m_pointer));
	ZeroMemory(&m_description, sizeof(m_description));

	m_description.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	m_description.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	m_description.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	m_description.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	m_description.ComparisonFunc = D3D11_COMPARISON_NEVER;
	m_description.MinLOD = 0;
	m_description.MaxLOD = D3D11_FLOAT32_MAX;
}
#endif

CSamplerState::~CSamplerState()
{
#ifdef DIRECT_X
	if (m_pointer)
	{
		m_pointer->Release();
	}
#endif
}