#include "../Header/CSamplerState.h"


void CSamplerState::Initialize()
{
#ifdef DIRECT_X
	ZeroMemory(&m_pointer, sizeof(m_pointer));
	ZeroMemory(&m_description, sizeof(m_description));

	m_description.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	m_description.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	m_description.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	m_description.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	m_description.MipLODBias = 0.0f;
	m_description.MaxAnisotropy = 1;
	m_description.ComparisonFunc = D3D11_COMPARISON_NEVER;
	m_description.BorderColor; //Default = Color{ 1.0f, 1.0f, 1.0f, 1.0f };
	m_description.MinLOD = 0;
	m_description.MaxLOD = D3D11_FLOAT32_MAX;
#endif
#ifdef OPEN_GL
	glGenSamplers(1, &m_id);
	glSamplerParameteri(m_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glSamplerParameteri(m_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glSamplerParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glSamplerParameterf(m_id, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);
#endif
}

#ifdef DIRECT_X
void CSamplerState::Initialize(D3D11_FILTER Filter, D3D11_TEXTURE_ADDRESS_MODE AddressU, D3D11_TEXTURE_ADDRESS_MODE AddressV, 
D3D11_TEXTURE_ADDRESS_MODE AddressW, float MipLODBias, unsigned int MaxAnisotropy, D3D11_COMPARISON_FUNC ComparisonFunc, 
Color BorderColor, float MinLOD, float MaxLOD)
{
	ZeroMemory(&m_pointer, sizeof(m_pointer));
	ZeroMemory(&m_description, sizeof(m_description));

	m_description.Filter = Filter;
	m_description.AddressU = AddressU;
	m_description.AddressV = AddressV;
	m_description.AddressW = AddressW;
	m_description.MipLODBias = MipLODBias;
	m_description.MaxAnisotropy = MaxAnisotropy;
	m_description.ComparisonFunc = ComparisonFunc;
	*m_description.BorderColor = BorderColor.r;
	m_description.MinLOD = MinLOD;
	m_description.MaxLOD = MaxLOD;
}
#endif

#ifdef OPEN_GL
void CSamplerState::Initialize(unsigned int wrapS, unsigned int wrapT, unsigned int wrapMode, unsigned int magFilter, 
unsigned int filterMode, unsigned int minFilter, unsigned int minFilterMode, unsigned int anisotropy, float anisotropicLevel)
{
	glGenSamplers(1, &m_id);
	glSamplerParameteri(m_id, wrapS, wrapMode);
	glSamplerParameteri(m_id, wrapT, wrapMode);
	glSamplerParameteri(m_id, magFilter, filterMode);
	glSamplerParameteri(m_id, minFilter, minFilterMode);
	glSamplerParameterf(m_id, anisotropy, anisotropicLevel);
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
#ifdef OPEN_GL
	glDeleteSamplers(1, &m_id);
#endif
}