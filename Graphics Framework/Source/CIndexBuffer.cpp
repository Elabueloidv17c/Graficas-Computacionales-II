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

#ifdef OPEN_GL
void CIndexBuffer::Initialize(std::vector<unsigned int> data)
{
	glGenBuffers(1, &m_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(unsigned int), &data[0], GL_STATIC_DRAW);

	Unbind();
}
#endif

#ifdef DIRECT_X
void CIndexBuffer::Initialize(unsigned int* data, unsigned int size)
{
	ZeroMemory(&m_description, sizeof(m_description));
	m_description.Usage = D3D11_USAGE_DEFAULT;
	m_description.ByteWidth = sizeof(unsigned int) * size;
	m_description.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_description.CPUAccessFlags = 0;

	ZeroMemory(&m_InitData, sizeof(m_InitData));

	m_InitData.pSysMem = data;
}
#endif

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
