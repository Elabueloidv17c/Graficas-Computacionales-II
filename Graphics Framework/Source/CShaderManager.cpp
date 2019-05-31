#include "CShaderManager.h"

CShaderManager::CShaderManager()
{
	m_activeShader = 0;
}

bool CShaderManager::Initialize(std::vector <ShaderProgramData> data)
{
	m_shaders.resize(data.size());

	for (int i = 0; i < data.size(); i++)
	{
		m_shaders[i].Initialize(data[i].vertex, data[i].pixel);
	}

	return true;
}

CShaderProgram* CShaderManager::GetActiveShader()
{
	return &m_shaders[m_activeShader];
}

CShaderProgram * CShaderManager::GetShaderAt(int index)
{
	return &m_shaders[index];
}

void CShaderManager::SetActiveShader(int index)
{
	m_activeShader = index;
}


void CShaderManager::Update()
{
	if (m_isVertex && m_isBlinn)
	{
		m_activeShader = 3;
	}
	else if (m_isVertex && !m_isBlinn)
	{
		m_activeShader = 2;
	}
	else if (!m_isVertex && m_isBlinn)
	{
		m_activeShader = 1;
	}
	else if(!m_isVertex && !m_isBlinn)
	{
		m_activeShader = 0;
	}
}
