#pragma once
#include "Header.h"
#include "../Header/CShaderProgram.h"

class CCamera;

class CShaderManager
{
	std::vector <CShaderProgram>	m_shaders;
	unsigned int					m_activeShader;

public:

	bool							m_isVertex;
	bool							m_isBlinn;

	bool Initialize(std::vector <ShaderProgramData> data);

	CShaderProgram* GetActiveShader();
	CShaderProgram* GetShaderAt(int index);
	void SetActiveShader(int index);
	void Update();

	CShaderManager();
	~CShaderManager() = default;
};

