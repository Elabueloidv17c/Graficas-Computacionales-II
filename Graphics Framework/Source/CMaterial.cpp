#include "CMaterial.h"

#ifdef OPEN_GL
void CMaterial::Render(unsigned int shaderProgram)
{
	m_diffuse.Render(shaderProgram, true);
	m_normal.Render(shaderProgram, false);
}

void CMaterial::EndRender()
{
	m_diffuse.EndRender();
	m_normal.EndRender();
}
#endif

bool CMaterial::Initialize(std::vector <std::string>& texturePaths)
{
	m_diffuse.Initialize(texturePaths[0]);
	m_normal.Initialize(texturePaths[1]);

	return true;
}

void CMaterial::Erase()
{

}
