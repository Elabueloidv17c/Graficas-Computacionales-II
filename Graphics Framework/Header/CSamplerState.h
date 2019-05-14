#pragma once
#include "Header.h"


class CSamplerState
{
public:

#ifdef OPEN_GL

#endif

#ifdef DIRECT_X
	ID3D11SamplerState*             m_pointer;
	D3D11_SAMPLER_DESC				m_description;
#endif

	CSamplerState() = default;
	~CSamplerState();
	void Initialize();
};
