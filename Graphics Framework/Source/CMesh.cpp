#include "../Header/CMesh.h"

CMesh::CMesh()
{
#ifdef DIRECT_X
	m_texture = nullptr;
#endif

	m_scale = 1.0f;
	m_color = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_isParty = false;
}

CMesh::~CMesh()
{
#ifdef OPEN_GL
	Erase();
#endif

#ifdef DIRECT_X
	if (m_texture)
	{
		m_texture->Release();
	}
#endif

}

void CMesh::Erase()
{
	m_transforms.clear();
	m_vertices.clear();
	m_indices.clear();

#ifdef OPEN_GL
	m_indexBuffer.Erase();
	m_vertexBuffer.Erase();
#endif
}

void CMesh::Update(float time, std::string modelPath)
{
	if (modelPath == "../Dragon/Dragon.fbx")
	{
#ifdef OPEN_GL
		m_transforms[0] = glm::rotate(m_transforms[0], glm::radians(3.0f), VECTOR3(0.0f, 1.0f, 0.0f));
#endif

#ifdef DIRECT_X
		m_transforms[0] = glm::rotate(m_transforms[0], glm::radians(0.02f), VECTOR3(0.0f, 1.0f, 0.0f));
#endif
	}

	else if (modelPath == "../Dwarf/DwarfWithEffectInstance.x")
	{
		m_transforms[0] = glm::scale(glm::mat4(1.0f), VECTOR3(abs(cosf(time))));
	}

	if (m_isParty)
	{
		m_color.r = sinf(time * 1.0f) + 1.0f;
		m_color.g = cosf(time * 3.0f) + 1.0f;
		m_color.b = sinf(time * 5.0f) + 1.0f;
	}
}

void CMesh::SetTransform(MATRIX4 transform, unsigned int index)
{
	if (index >= m_transforms.size())
	{
		std::cout << "Warning:  index out of range, transform set in index 0" << std::endl;
		m_transforms[0] = transform;
		return;
	}

	m_transforms[index] = transform;
}

void CMesh::Rotate(float speed)
{
	for (int i = 0; i < m_transforms.size(); i++)
	{
		m_transforms[i] = glm::rotate(m_transforms[i], glm::radians(cos(speed)), VECTOR3(0.0f, 1.0f, 0.0f));
	}
}

void CMesh::ChangeScale()
{
	if (m_scale >= 1.0f)
	{
		m_scale = 0.5f;
	}
	else
	{
		m_scale = 2.0f;
	}

	for (int i = 0; i < m_transforms.size(); i++)
	{
		m_transforms[i] = glm::scale(m_transforms[i], VECTOR3(m_scale));
	}
}

void CMesh::PartyMode()
{
	if (m_isParty)
	{
		m_color = { 1.0f, 1.0f, 1.0f, 1.0f };
	}

	m_isParty = !m_isParty;
}

void CMesh::AddTransform(MATRIX4 transform)
{
	m_transforms.push_back(transform);
}

#ifdef DIRECT_X
void CMesh::Render(float time, std::string modelPath)
{
	Update(time, modelPath);
}
#endif  

void CMesh::Initialize(std::string texturePath)
{
	m_numIndices = m_indices.size();
	m_numVertices = m_vertices.size();

	m_texturePath = texturePath;

#ifdef DIRECT_X
	m_vertexBuffer.Initialize(&m_vertices[0], m_vertices.size());
	m_indexBuffer.Initialize(&m_indices[0], m_indices.size());
#endif

#ifdef OPEN_GL
	m_vertexBuffer.Initialize(m_vertices);
	m_indexBuffer.Initialize(m_indices);

	m_texture.Initialize(texturePath);
#endif

}

#ifdef OPEN_GL
void CMesh::Render(unsigned int shaderProgram)
{
	m_texture.Render(shaderProgram);

	m_vertexBuffer.Bind();
	m_indexBuffer.Bind();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearDepth(1.0f);

	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, NULL);

	m_texture.EndRender();

	glBindVertexArray(0);
}
#endif