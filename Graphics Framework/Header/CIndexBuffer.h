#pragma once
#include "Header.h"

class CIndexBuffer
{
public:

#ifdef OPEN_GL
	unsigned int m_id;													

	void Initialize(std::vector<unsigned int> data);

	unsigned int GetID();
	void Bind();
	void Unbind();
	void Erase();
	
#endif

#ifdef DIRECT_X
	ID3D11Buffer*				m_pointer;
	unsigned int				m_stride;

	D3D11_BUFFER_DESC			m_description;
	D3D11_SUBRESOURCE_DATA		m_InitData;

	unsigned int				m_offset;

	void Initialize(unsigned int* data, unsigned int size);
#endif


	CIndexBuffer();
	~CIndexBuffer();
};