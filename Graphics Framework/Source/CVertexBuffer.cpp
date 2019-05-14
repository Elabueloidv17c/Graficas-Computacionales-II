#include "../Header/CVertexBuffer.h"

CVertexBuffer::CVertexBuffer()
{
#ifdef OPEN_GL
	m_id = 0;
#endif

#ifdef DIRECT_X
	m_pointer = nullptr;
#endif

}

CVertexBuffer::~CVertexBuffer()
{
#ifdef OPEN_GL
	Erase();
#endif

#ifdef DIRECT_X
	if (m_pointer)
	{
		m_pointer->Release();
	}
#endif
}

#ifdef OPEN_GL
void CVertexBuffer::Initialize(std::vector <Vertex> data)
{
	m_stride = sizeof(Vertex);

	glGenBuffers(1, &m_id);
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * data.size(), &data[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	// Vertex position.
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, m_stride, 0);

	// Texture coordinates.
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, m_stride, (const void*)(3 * sizeof(float)));

	// Normal Direction.
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, m_stride, (const void*)(5 * sizeof(float)));

	// Tangent Direction.
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, m_stride, (const void*)(8 * sizeof(float)));
}
#endif

#ifdef DIRECT_X
void CVertexBuffer::Initialize(Vertex* data, unsigned int size)
{
	ZeroMemory(&m_description, sizeof(m_description));
	m_description.Usage = D3D11_USAGE_DEFAULT;
	m_description.ByteWidth = sizeof(Vertex) * size;
	m_description.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_description.CPUAccessFlags = 0;

	ZeroMemory(&m_InitData, sizeof(m_InitData));

	m_InitData.pSysMem = data;

	m_stride = sizeof(Vertex);
	m_offset = 0;
}
#endif

#ifdef OPEN_GL
void CVertexBuffer::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, m_stride, 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, m_stride, (const void*)(3 * sizeof(float)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, m_stride, (const void*)(5 * sizeof(float)));
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, m_stride, (const void*)(8 * sizeof(float)));
}

void CVertexBuffer::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void CVertexBuffer::Erase()
{
	m_id = 0;
	m_stride = 0;
}

unsigned int CVertexBuffer::GetID()
{
	return m_id;
}
#endif