#include "../Header/CMesh.h"

CMesh::CMesh()
{
#ifdef DIRECT_X
	m_material.m_diffuse.m_pointer = nullptr;
	m_material.m_normal.m_pointer = nullptr;
#endif
}

CMesh::~CMesh()
{
#ifdef OPEN_GL
	Erase();
#endif

#ifdef DIRECT_X
	if (m_material.m_diffuse.m_pointer)
	{
		m_material.m_diffuse.m_pointer->Release();
	}
	if (m_material.m_normal.m_pointer)
	{
		m_material.m_normal.m_pointer->Release();
	}
#endif
}

void CMesh::Erase()
{
	if (m_vertices.size())
	{
		m_vertices.clear();
	}
	if (m_indices.size())
	{
		m_indices.clear();
	}

#ifdef OPEN_GL
	m_indexBuffer.Erase();
	m_vertexBuffer.Erase();
#endif
}

void CMesh::Initialize(std::vector <std::string>& texturePaths)
{
	m_numIndices = m_indices.size();
	m_numVertices = m_vertices.size();

	m_material.Initialize(texturePaths);

#ifdef DIRECT_X
	m_vertexBuffer.Initialize(&m_vertices[0], m_vertices.size());
	m_indexBuffer.Initialize(&m_indices[0], m_indices.size());
#endif

#ifdef OPEN_GL
	m_vertexBuffer.Initialize(m_vertices);
	m_indexBuffer.Initialize(m_indices);
#endif

}

#ifdef OPEN_GL
void CMesh::Render(unsigned int shaderProgram)
{
	m_material.Render(shaderProgram);
	
	m_vertexBuffer.Bind();
	m_indexBuffer.Bind();
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearDepth(1.0f);
	
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, NULL);
	
	m_material.EndRender();
	
	glBindVertexArray(0);
}
#endif