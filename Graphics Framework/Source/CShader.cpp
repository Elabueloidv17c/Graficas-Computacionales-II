#include "../Header/CShader.h"

CShader::CShader()
{
	m_code = nullptr;
	m_id = 0;
}

CShader::~CShader()
{
#ifdef DIRECT_X

#endif
}

void CShader::Erase()
{
	delete[] m_code;
	m_code = nullptr;
}


#ifdef OPEN_GL
bool CShader::Initialize(const std::string path, int typeFlag)
{
	// Load the shader source file into a text buffer.
	if (!LoadSource(&path[0]))
	{
		return false;
	}

	// Create a shader object.
	m_id = glCreateShader(typeFlag);

	// Copy the shader source code strings into the shader objects.
	glShaderSource(m_id, 1, &m_code, NULL);

	// Compile the shaders.
	glCompileShader(m_id);

	// Check to see if the shader compiled successfully.
	int status = 0;
	glGetShaderiv(m_id, GL_COMPILE_STATUS, &status);

	if (status != 1)
	{
		return false;
	}

	return true;
}

bool CShader::LoadSource(const char* path)
{
	std::ifstream file;
	int fileSize;
	char input;

	// Open the shader source file.
	file.open(path);

	// If it could not open the file then exit.
	if (file.fail())
	{
		std::cout << "Error: Shader source code not loaded" << std::endl;
		return false;
	}

	// Initialize the size of the file.
	fileSize = 0;

	// Read the first element of the file.
	file.get(input);

	// Count the number of elements in the text file.
	while (!file.eof())
	{
		fileSize++;
		file.get(input);
	}

	// Close the file for now.
	file.close();

	// Initialize the buffer to read the shader source file into.

	m_code = new char[fileSize + 1];

	if (!m_code)
	{
		std::cout << "Error: Shader data not initialized correctly" << std::endl;
		return false;
	}

	// Open the shader source file again.
	file.open(path);

	// Read the shader text file into the buffer as a block.
	file.read(m_code, fileSize);

	// Close the file.
	file.close();

	// Null terminate the buffer.
	m_code[fileSize] = '\0';

	return true;
}

int CShader::GetId()
{
	return m_id;
}
#endif