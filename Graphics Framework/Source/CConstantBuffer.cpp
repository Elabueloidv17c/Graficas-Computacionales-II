#include "../Header/CConstantBuffer.h"

CConstantBuffer::CConstantBuffer()
{
#ifdef DIRECT_X
	m_Buffer = nullptr;
#endif
#ifdef OPEN_GL
	m_shader = nullptr;
#endif
}

CConstantBuffer::~CConstantBuffer()
{
#ifdef DIRECT_X
	if (m_Buffer)
	{
		m_Buffer->Release();
	}
#endif
}

#ifdef DIRECT_X
void CConstantBuffer::Initialize(unsigned int Size)
{
	ZeroMemory(&m_description, sizeof(m_description));

	m_description.ByteWidth = Size;
	m_description.CPUAccessFlags = 0;
	m_description.Usage = D3D11_USAGE_DEFAULT;
	m_description.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
}
#endif

#ifdef OPEN_GL
void CConstantBuffer::Initialize(unsigned int* shader, std::vector <std::string> names)
{
	m_uniforms.resize(names.size());

	for (int i = 0; i < m_uniforms.size(); i++)
	{
		m_uniforms[i] = UniformData{ names[i], -1 };
	}

	m_shader = shader;
}

void CConstantBuffer::Initialize(unsigned int* shader, std::string name)
{
	m_uniforms.push_back(UniformData{ name, -1 });
	m_shader = shader;
}

void CConstantBuffer::Initialize(unsigned int* shader)
{
	m_shader = shader;
}
#endif
