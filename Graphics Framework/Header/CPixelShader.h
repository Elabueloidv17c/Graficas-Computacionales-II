#pragma once
#include "Header.h"

class CPixelShader
{
public:
	char*									m_code;
	unsigned int							m_codeSize;

#ifdef DIRECT_X
	LPCSTR									m_shaderModel;
	LPCSTR									m_entryPoint;
	const char*								m_filePath;

	ID3D11PixelShader*						m_pointer;
	ID3DBlob*								m_blob;
#endif
#ifdef OPEN_GL
	int m_id;
	int GetId();
#endif


	~CPixelShader();
	CPixelShader();

	bool Initialize(const ShaderData& data);
	bool LoadSource(const ShaderData& data);
	void Erase();
};