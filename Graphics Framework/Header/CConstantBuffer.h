#pragma once
#include "Header.h"

class CConstantBuffer
{
public:

#ifdef DIRECT_X
	D3D11_BUFFER_DESC m_description;
	ID3D11Buffer* m_Buffer;

	void Initialize(unsigned int Size);
#endif
#ifdef OPEN_GL

	std::vector <UniformData>			m_uniforms;
	unsigned int*									m_shader;

	void Initialize(unsigned int* shader, std::vector <std::string> names);
	void Initialize(unsigned int* shader, std::string name);
	void Initialize(unsigned int* shader);
#endif

	CConstantBuffer();
	~CConstantBuffer();
};


