#include "../Header/CConstantBuffer.h"

#ifdef DIRECT_X
CConstantBuffer::CConstantBuffer()
{
	m_Buffer = nullptr;
}

CConstantBuffer::~CConstantBuffer()
{
	if (m_Buffer)
	{
		m_Buffer->Release();
	}
}

void CConstantBuffer::Initialize(unsigned int Size)
{
	ZeroMemory(&m_description, sizeof(m_description));

	m_description.ByteWidth = Size;
	m_description.CPUAccessFlags = 0;
	m_description.Usage = D3D11_USAGE_DEFAULT;
	m_description.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
}
#endif