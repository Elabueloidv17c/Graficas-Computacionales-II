#pragma once
#include "Header.h"

#ifdef DIRECT_X
class CConstantBuffer
{
public:

	D3D11_BUFFER_DESC m_description;
	ID3D11Buffer* m_Buffer;

	void Initialize(unsigned int Size);

	CConstantBuffer();
	~CConstantBuffer();
};
#endif

