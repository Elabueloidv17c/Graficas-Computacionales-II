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
#ifdef OPEN_GL
	Erase();
#endif

#ifdef DIRECT_X

#endif
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

#ifdef OPEN_GL
	// Create a shader program object.
	m_id = glCreateProgram();

	// Attach the vertex and fragment shader to the program object.
	glAttachShader(m_id, m_vertexShader.GetId());
	glAttachShader(m_id, m_pixelShader.GetId());

	// Bind the shader input variables.
	glBindAttribLocation(m_id, 0, "inPosition");
	glBindAttribLocation(m_id, 1, "inTexCoord");
	glBindAttribLocation(m_id, 2, "inNormal");
	glBindAttribLocation(m_id, 3, "inTangent");

	// Link the shader program.
	glLinkProgram(m_id);

	// Check the status of the link.
	int status;
	glGetProgramiv(m_id, GL_LINK_STATUS, &status);

	if (status != 1)
	{
		std::cout << "Error: Shader program link not correct" << std::endl;
		return false;
	}
#endif

	return true;
}


void CShaderProgram::Erase()
{
#ifdef OPEN_GL
	// Detach the vertex and fragment shaders from the program.
	glDetachShader(m_id, m_vertexShader.GetId());
	glDetachShader(m_id, m_pixelShader.GetId());

	// Delete the vertex and fragment shaders.
	glDeleteShader(m_vertexShader.GetId());
	glDeleteShader(m_pixelShader.GetId());

	// Delete the shader program.
	glDeleteProgram(m_id);
#endif
}

#ifdef OPEN_GL
void CShaderProgram::Bind()
{
	glUseProgram(m_id);
}

void CShaderProgram::Unbind()
{
	glUseProgram(0);
}

bool CShaderProgram::UpdateCamera(CCamera& camera)
{
	int location;

	// View Matrix
	location = glGetUniformLocation(m_id, "View");

	if (location != -1)
	{
		glUniformMatrix4fv(location, 1, false, &camera.m_viewMatrix[0][0]);
	}

	// Projection Matrix
	location = glGetUniformLocation(m_id, "Projection");

	if (location != -1)
	{
		glUniformMatrix4fv(location, 1, false, &camera.m_projectionMatrix[0][0]);
	}

	return true;
}

bool CShaderProgram::UpdateModel(MATRIX4& transform)
{
	int location;

	// Camera Pos.
	location = glGetUniformLocation(m_id, "Model");

	if (location != -1)
	{
		glUniformMatrix4fv(location, 1, false, &transform[0][0]);
	}

	return true;
}

bool CShaderProgram::UpdateWorld(MATRIX4& data)
{
	int location;

	//Light
	location = glGetUniformLocation(m_id, "World");

	if (location != -1)
	{
		glUniformMatrix4fv(location, 1, false, &data[0][0]);
	}

	return true;
}

bool CShaderProgram::UpdateLight(LightingData& data)
{
	int location;

	//Light
	location = glGetUniformLocation(m_id, "DirectionalLight");

	if (location != -1)
	{
		glUniform3fv(location, 1, &data.directional.x);
	}

	location = glGetUniformLocation(m_id, "SpecularPower");

	if (location != -1)
	{
		glUniform1f(location, data.specularPower);
	}

	location = glGetUniformLocation(m_id, "PointPosition");

	if (location != -1)
	{
		glUniform3fv(location, 1, &data.pointPosition.x);
	}

	location = glGetUniformLocation(m_id, "PointRadius");

	if (location != -1)
	{
		glUniform1f(location, data.PointRadius);
	}

	location = glGetUniformLocation(m_id, "ViewPosition");

	if (location != -1)
	{
		glUniform3fv(location, 1, &data.viewPosition.x);
	}

	location = glGetUniformLocation(m_id, "SpotRadius");

	if (location != -1)
	{
		glUniform1f(location, data.SpotRadius);
	}

	location = glGetUniformLocation(m_id, "SpotPosition");

	if (location != -1)
	{
		glUniform3fv(location, 1, &data.spotPosition.x);
	}

	location = glGetUniformLocation(m_id, "SpotAlpha");

	if (location != -1)
	{
		glUniform1f(location, data.spotAlpha);
	}

	location = glGetUniformLocation(m_id, "SpotLightDirection");

	if (location != -1)
	{
		glUniform3fv(location, 1, &data.spotDirection.x);
	}

	location = glGetUniformLocation(m_id, "SpotBeta");

	if (location != -1)
	{
		glUniform1f(location, data.spotBeta);
	}

	return true;
}

bool CShaderProgram::UpdateColor(ColorData& data)
{
	int location;

	// Directional Light Color.
	location = glGetUniformLocation(m_id, "DirectionalColor");

	if (location != -1)
	{
		glUniform4fv(location, 1, &data.directionalColor.r);
	}

	// Point Light Color.
	location = glGetUniformLocation(m_id, "PointColor");

	if (location != -1)
	{
		glUniform4fv(location, 1, &data.pointColor.r);
	}

	// Spot Light Color.
	location = glGetUniformLocation(m_id, "SpotColor");

	if (location != -1)
	{
		glUniform4fv(location, 1, &data.spotColor.r);
	}

	// Mesh Color.
	location = glGetUniformLocation(m_id, "SpecularColor");

	if (location != -1)
	{
		glUniform4fv(location, 1, &data.specularColor.r);
	}

	// Mesh Color.
	location = glGetUniformLocation(m_id, "AmbientColor");

	if (location != -1)
	{
		glUniform4fv(location, 1, &data.ambientColor.r);
	}

	location = glGetUniformLocation(m_id, "DiffuseIntensity");

	if (location != -1)
	{
		glUniform1f(location, data.diffuseIntensity);
	}

	location = glGetUniformLocation(m_id, "SpecularIntensity");

	if (location != -1)
	{
		glUniform1f(location, data.specularIntensity);
	}

	location = glGetUniformLocation(m_id, "AmbientIntensity");

	if (location != -1)
	{
		glUniform1f(location, data.ambientIntensity);
	}

	return true;
}
#endif

#ifdef DIRECT_X
HRESULT CShaderProgram::CompileFromFile()
{
	HRESULT hr = S_OK;

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