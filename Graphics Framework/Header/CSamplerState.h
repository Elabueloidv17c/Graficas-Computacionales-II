#pragma once
#include "Header.h"


class CSamplerState
{
public:

#ifdef DIRECT_X
	ID3D11SamplerState*       m_pointer;
	D3D11_SAMPLER_DESC				m_description;

	void Initialize(D3D11_FILTER Filter, D3D11_TEXTURE_ADDRESS_MODE AddressU, D3D11_TEXTURE_ADDRESS_MODE AddressV,
		D3D11_TEXTURE_ADDRESS_MODE AddressW, float MipLODBias, unsigned int MaxAnisotropy, D3D11_COMPARISON_FUNC ComparisonFunc,
		Color BorderColor, float MinLOD, float MaxLOD);
#endif
#ifdef OPEN_GL
	unsigned int m_id;

	void Initialize(unsigned int wrapS, unsigned int wrapT, unsigned int wrapMode, unsigned int magFilter, unsigned int filterMode, 
	unsigned int minFilter, unsigned int minFilterMode, unsigned int anisotropy, float anisotropicLevel);
#endif

	CSamplerState() = default;
	~CSamplerState();

	void Initialize();
};
