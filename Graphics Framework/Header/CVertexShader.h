#pragma once
#include "Header.h"

class CVertexShader
{
public:
	char*																m_code;
	unsigned int												m_codeSize;
	std::vector <LayoutElement>					m_layout;

#ifdef DIRECT_X
	ID3D11InputLayout*									m_pLayout;

	LPCSTR															m_shaderModel;
	LPCSTR															m_entryPoint;
	const char*													m_filePath;

	ID3D11VertexShader*									m_pointer;
	ID3DBlob*														m_blob;

	long CreateLayout();

#endif
#ifdef OPEN_GL
	int m_type;
	int m_id;
#endif

	~CVertexShader();
	CVertexShader();

	bool Initialize(const ShaderData& data);
	bool LoadSource(const ShaderData& data);
	void Erase();
};