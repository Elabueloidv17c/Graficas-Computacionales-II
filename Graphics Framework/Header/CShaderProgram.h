#pragma once
#include "Header.h"
#include "CVertexShader.h"
#include "CPixelShader.h"

#ifdef OPEN_GL
class CCamera;
class CModel;
#endif

class CShaderProgram
{
public:

	CVertexShader						m_vertexShader;
	CPixelShader						m_pixelShader;

#ifdef DIRECT_X
	long CompileFromFile();
#endif

#ifdef OPEN_GL
	unsigned int						m_id;
#endif

	bool Initialize(ShaderData vertexData, ShaderData pixelData);
	void Erase();

	CShaderProgram();
	~CShaderProgram();
};