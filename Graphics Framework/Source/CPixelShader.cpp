#include "../Header/CPixelShader.h"

CPixelShader::CPixelShader()
{
	m_code = nullptr;

#ifdef OPEN_GL
	m_id = 0;
#endif
}

CPixelShader::~CPixelShader()
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

void CPixelShader::Erase()
{
	if (m_code)
	{
		delete[] m_code;
		m_code = nullptr;
	}
}


bool CPixelShader::Initialize(const ShaderData& data)
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
		MessageBox(NULL, "Pixel shader not compiled correctly", "Error", MB_OK);

		return result;
	}
#endif

	return true;
}

bool CPixelShader::LoadSource(const ShaderData& data)
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