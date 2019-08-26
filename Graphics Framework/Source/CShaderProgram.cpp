#include "../Header/CShaderProgram.h"
#include "../Header/CCamera.h"
#include "../Header/CModel.h"

CShaderProgram::CShaderProgram()
{
#ifdef OPEN_GL
	m_id = 0;
#endif
}

CShaderProgram::~CShaderProgram()
{
	Erase();
}

bool CShaderProgram::Initialize(ShaderData vertexData, ShaderData pixelData)
{
	// Compile shaders
	if (!m_vertexShader.Initialize(vertexData))
	{
		std::cout << "Error:  Vertex shader not compiled correctly" << std::endl;
		return false;
	}

	if (!m_pixelShader.Initialize(pixelData))
	{
		std::cout << "Error:  Pixel shader not compiled correctly" << std::endl;
		return false;
	}

	return true;
}

void CShaderProgram::Erase()
{
#ifdef OPEN_GL
	// Detach the vertex and fragment shaders from the program.
	glDetachShader(m_id, m_vertexShader.m_id);
	glDetachShader(m_id, m_pixelShader.m_id);

	// Delete the vertex and fragment shaders.
	glDeleteShader(m_vertexShader.m_id);
	glDeleteShader(m_pixelShader.m_id);

	// Delete the shader program.
	glDeleteProgram(m_id);
#endif
}

#ifdef DIRECT_X
long CShaderProgram::CompileFromFile()
{
	long hr = S_OK;

	DWORD dwShaderFlags = D3DXSHADER_DEBUG;

	ID3DBlob* pErrorBlob;

	hr = D3DX11CompileFromFile(m_vertexShader.m_filePath, NULL, NULL, m_vertexShader.m_entryPoint, 
	m_vertexShader.m_shaderModel, dwShaderFlags, 0, NULL, &m_vertexShader.m_blob, &pErrorBlob, NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "The FX file cannot be compiled for vertex. Please run this executable from the directory that contains the FX file.", "Error", MB_OK);

		if (pErrorBlob != NULL)
		{
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		}

		if (pErrorBlob) pErrorBlob->Release();
		{
			return hr;
		}
	}

	hr = D3DX11CompileFromFile(m_pixelShader.m_filePath, NULL, NULL, m_pixelShader.m_entryPoint,
	m_pixelShader.m_shaderModel, dwShaderFlags, 0, NULL, &m_pixelShader.m_blob, &pErrorBlob, NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "The FX file cannot be compiled for pixel. Please run this executable from the directory that contains the FX file.", "Error", MB_OK);

		if (pErrorBlob != NULL)
		{
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		}

		if (pErrorBlob) pErrorBlob->Release();
		{
			return hr;
		}
	}

	return S_OK;
}
#endif