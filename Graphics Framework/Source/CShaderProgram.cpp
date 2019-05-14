#include "../Header/CShaderProgram.h"
#include "../Header/CCamera.h"
#include "../Header/CModel.h"

CShaderProgram::CShaderProgram()
{
#ifdef OPEN_GL
	m_id = 0;
#endif

#ifdef DIRECT_X
	m_filePath = nullptr;
	m_pLayout = nullptr;
	m_pVertex = nullptr;
	m_pVertexBlob = nullptr;

	m_pPixel = nullptr;
	m_pPixelBlob = nullptr;
#endif
}

CShaderProgram::~CShaderProgram()
{
#ifdef OPEN_GL
	Erase();
#endif

#ifdef DIRECT_X
	if (m_pVertexBlob)
	{
		m_pVertexBlob->Release();
	}
	if(m_pPixelBlob)
	{
		m_pPixelBlob->Release();
	}
	if (m_pLayout)
	{
		m_pLayout->Release();
	}
	if (m_pVertex)
	{
		m_pVertex->Release();
	}
	if (m_pPixel)
	{
		m_pPixel->Release();
	}
#endif
}

#ifdef OPEN_GL
bool CShaderProgram::Initialize(std::string vertexShaderPath, std::string pixelShaderPath)
{
	// Compile shaders
	if (!m_VertexShader.Initialize(vertexShaderPath, GL_VERTEX_SHADER))
	{
		std::cout << "Error:  Vertex shader not compiled correctly" << std::endl;
		return false;
	}

	if (!m_FragmentShader.Initialize(pixelShaderPath, GL_FRAGMENT_SHADER))
	{
		std::cout << "Error:  Fragment shader not compiled correctly" << std::endl;
		return false;
	}

	// Create a shader program object.
	m_id = glCreateProgram();

	// Attach the vertex and fragment shader to the program object.
	glAttachShader(m_id, m_VertexShader.GetId());
	glAttachShader(m_id, m_FragmentShader.GetId());

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

	return true;
}

void CShaderProgram::Erase()
{
	// Detach the vertex and fragment shaders from the program.
	glDetachShader(m_id, m_VertexShader.GetId());
	glDetachShader(m_id, m_FragmentShader.GetId());

	// Delete the vertex and fragment shaders.
	glDeleteShader(m_VertexShader.GetId());
	glDeleteShader(m_FragmentShader.GetId());

	// Delete the shader program.
	glDeleteProgram(m_id);
}

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
		glUniform1f(location, data.specularIntensity);
	}

	return true;
}

bool CShaderProgram::UpdateColor(ColorData& data)
{
	int location;

	// Diffuse Color.
	location = glGetUniformLocation(m_id, "DiffuseColor");

	if (location != -1)
	{
		glUniform4fv(location, 1, &data.diffuseColor.r);
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
HRESULT CShaderProgram::Initialize(const char* fileName, LPCSTR vertexShaderModel, LPCSTR pixelShaderModel, LPCSTR vertexEntryPoint, LPCSTR pixelEntryPoint)
{
	m_VertexShaderModel = vertexShaderModel;
	m_vertexEntryPoint = vertexEntryPoint;
	m_pixelShaderModel = pixelShaderModel;
	m_pixelEntryPoint = pixelEntryPoint;

	m_filePath = fileName;

	m_layoutSize = 4;
	m_layout.resize(m_layoutSize);

	m_layout[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	m_layout[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	m_layout[2] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	m_layout[3] = { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 };

	if (FAILED(CompileFromFile()))
	{
		return S_FALSE;
	}
	else
	{
		return S_OK;
	}
}

HRESULT CShaderProgram::CompileFromFile()
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

	ID3DBlob* pErrorBlob;

	hr = D3DX11CompileFromFile(m_filePath, NULL, NULL, m_vertexEntryPoint, m_VertexShaderModel, dwShaderFlags, 0, NULL, &m_pVertexBlob, &pErrorBlob, NULL);

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

	hr = D3DX11CompileFromFile(m_filePath, NULL, NULL, m_pixelEntryPoint, m_pixelShaderModel, dwShaderFlags, 0, NULL, &m_pPixelBlob, &pErrorBlob, NULL);

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

#ifdef D3DReflect
HRESULT CShaderProgram::CreateLayout()
{
	// Reflect shader info
	ID3D11ShaderReflection* pVertexShaderReflection = nullptr;

	if (FAILED(D3DReflect(m_pVertexBlob->GetBufferPointer(), m_pVertexBlob->GetBufferSize(), IID_ID3D11ShaderReflection,
	   (void**)&pVertexShaderReflection)))
	{
		return S_FALSE;
	}

	// Get shader info
	D3D11_SHADER_DESC shaderDesc;
	pVertexShaderReflection->GetDesc(&shaderDesc);
	
	for (int i = 0; i < shaderDesc.InputParameters; i++)
	{
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
		pVertexShaderReflection->GetInputParameterDesc(i, &paramDesc);
	
		// fill out input element desc
		D3D11_INPUT_ELEMENT_DESC elementDescription;

		elementDescription.SemanticName = paramDesc.SemanticName;
		elementDescription.SemanticIndex = paramDesc.SemanticIndex;
		elementDescription.InputSlot = 0;
		elementDescription.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		elementDescription.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		elementDescription.InstanceDataStepRate = 0;
		
		// determine DXGI format
		if (paramDesc.Mask == 1)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
			{
				elementDescription.Format = DXGI_FORMAT_R32_UINT;
			}
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
			{
				elementDescription.Format = DXGI_FORMAT_R32_SINT;
			}
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
			{
				elementDescription.Format = DXGI_FORMAT_R32_FLOAT;
			}
		}
		else if (paramDesc.Mask <= 3)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
			{
				elementDescription.Format = DXGI_FORMAT_R32G32_UINT;
			}
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
			{
				elementDescription.Format = DXGI_FORMAT_R32G32_SINT;
			}
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
			{
				elementDescription.Format = DXGI_FORMAT_R32G32_FLOAT;
			}
		}
		else if (paramDesc.Mask <= 7)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
			{
				elementDescription.Format = DXGI_FORMAT_R32G32B32_UINT;
			}
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
			{
				elementDescription.Format = DXGI_FORMAT_R32G32B32_SINT;
			}
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
			{
				elementDescription.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			}
		}
		else if (paramDesc.Mask <= 15)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
			{
				elementDescription.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			}
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
			{
				elementDescription.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			}
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
			{
				elementDescription.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			}
		}

		//save element desc
		m_layout.push_back(elementDescription);
	}

	return S_OK;
}
#endif