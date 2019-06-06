#include "../Header/CIndexBuffer.h"


CIndexBuffer::CIndexBuffer()
{
#ifdef OPEN_GL
	m_id = 0;
#endif

#ifdef DIRECT_X
	m_pointer = nullptr;
#endif
}

CIndexBuffer::~CIndexBuffer()
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


void CIndexBuffer::Initialize(unsigned int* data, unsigned int size)
{
#ifdef DIRECT_X
	ZeroMemory(&m_description, sizeof(m_description));
	m_description.Usage = D3D11_USAGE_DEFAULT;
	m_description.ByteWidth = sizeof(unsigned int) * size;
	m_description.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_description.CPUAccessFlags = 0;

	ZeroMemory(&m_InitData, sizeof(m_InitData));

	m_InitData.pSysMem = data;
#endif
#ifdef OPEN_GL
	glGenBuffers(1, &m_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(unsigned int), data, GL_STATIC_DRAW);

	Unbind();
#endif
}

#ifdef OPEN_GL
void CIndexBuffer::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}

void CIndexBuffer::Unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void CIndexBuffer::Erase()
{
	m_id = 0;
}

unsigned int CIndexBuffer::GetID()
{
	return m_id;
}
#endif
