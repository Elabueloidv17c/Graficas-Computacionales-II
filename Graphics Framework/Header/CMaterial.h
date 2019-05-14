#pragma once
#include "Header.h"
#include "../Header/CTexture.h"

class CMaterial
{
public:

	CTexture					m_diffuse;
	CTexture					m_normal;

	bool Initialize(std::vector <std::string>& texturePaths);
	void Erase();
	
	void Render(unsigned int shaderProgram);
	void EndRender();

	CMaterial() = default;
	~CMaterial() = default;
};

