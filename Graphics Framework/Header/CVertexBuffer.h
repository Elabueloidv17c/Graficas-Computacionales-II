#pragma once
#include "Header.h"

class CVertexBuffer
{
public:

#ifdef OPEN_GL
	unsigned int				m_id;
	unsigned int				m_stride;

	void Initialize(std::vector <Vertex> data);

	unsigned int GetID();
	void Bind();
	void Unbind();

	void Erase();
#endif

#ifdef DIRECT_X
	ID3D11Buffer*				m_pointer;

	D3D11_BUFFER_DESC			m_description;
	D3D11_SUBRESOURCE_DATA		m_InitData;
	
	unsigned int				m_stride;
	unsigned int				m_offset;

	void Initialize(Vertex* data, unsigned int size);
#endif

	CVertexBuffer();
	~CVertexBuffer();
};