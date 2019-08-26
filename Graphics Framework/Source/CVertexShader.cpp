#include "../Header/CVertexShader.h"

CVertexShader::CVertexShader()
{
	m_code = nullptr;

#ifdef OPEN_GL
	m_id = 0;
#endif
}

CVertexShader::~CVertexShader()
{
#ifdef DIRECT_X
	if (m_pointer)
	{
		m_pointer->Release();
	}
	if (m_blob)
	{
		m_blob->Release();
	}
#endif
}

void CVertexShader::Erase()
{
	if (m_code)
	{
		delete[] m_code;
		m_code = nullptr;
	}
}

bool CVertexShader::Initialize(const ShaderData& data)
{
	// Load the shader source file into a text buffer.
	if (!LoadSource(data))
	{
		return false;
	}

#ifdef OPEN_GL
	m_type = data.typeFlag;
#endif

#ifdef DIRECT_X
	m_entryPoint = data.entryPoint;
	m_shaderModel = data.shaderModel;
	m_filePath = data.filePath;

	bool result = D3DCompile(m_code, m_codeSize, nullptr, nullptr, nullptr, m_entryPoint, m_shaderModel, 
	D3DCOMPILE_ENABLE_STRICTNESS, 0, &m_blob, nullptr);

	if (result)
	{
		MessageBox(NULL, "Vertex shader not compiled correctly", "Error", MB_OK);

		return result;
	}

	CreateLayout();

#endif

	return true;
}

bool CVertexShader::LoadSource(const ShaderData& data)
{
	std::ifstream file;

	// Open the shader source file.
	file.open(data.filePath);

	// If it could not open the file then exit.
	if (file.fail())
	{
		std::cout << "Error: Shader source code not loaded" << std::endl;
		return false;
	}

	// Initialize the size of the file.
	int fileSize = 0;

	// Read the first element of the file.
	char input;
	file.get(input);

	// Count the number of elements in the text file.
	while (!file.eof())
	{
		fileSize++;
		file.get(input);
	}

	// Close the file for now.
	file.close();

	if (!fileSize)
	{
		std::cout << "Error: Shader data not initialized correctly" << std::endl;
		return false;
	}

	// Initialize the buffer to read the shader source file into.
	std::string defines = "";

#ifdef OPEN_GL
	defines += "#version 400 core\n\n";
#endif
#ifdef POINT_LIGHT
	defines += "#define POINT_LIGHT\n";
#endif
#ifdef SPOT_LIGHT
	defines += "#define SPOT_LIGHT\n\n";
#endif

	if (!data.isBlinn)
	{
		defines += "#define BLINN_PHONG\n";
	}
	if (!data.isVertex)
	{
		defines += "#define PIXEL_LIGHT\n";
	}

	m_codeSize = fileSize + defines.length() + 1;
	m_code = new char[m_codeSize];

	for (int i = 0; i < defines.length(); i++)
	{
		m_code[i] = defines[i];
	}

	// Open the shader source file again.
	file.open(data.filePath);

	// Read the shader text file into the buffer as a block.
	file.read(m_code + defines.length(), fileSize);

	// Close the file.
	file.close();

	// Null terminate the buffer.
	m_code[m_codeSize - 1] = '\0';

	return true;
}

#ifdef DIRECT_X
long CVertexShader::CreateLayout()
{
	// Reflect shader info
	ID3D11ShaderReflection* pVertexShaderReflection = nullptr;

	if (FAILED(D3DReflect(m_blob->GetBufferPointer(), m_blob->GetBufferSize(),
		IID_ID3D11ShaderReflection, (void**)&pVertexShaderReflection)))
	{
		return S_FALSE;
	}

	// Get shader info
	D3D11_SHADER_DESC shaderDescription;
	pVertexShaderReflection->GetDesc(&shaderDescription);

	m_layout.resize(shaderDescription.InputParameters);

	for (int i = 0; i < m_layout.size(); i++)
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
		m_layout[i] = elementDescription;
	}

	return S_OK;
}
#endif