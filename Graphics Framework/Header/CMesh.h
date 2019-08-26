#pragma once
#include "Header.h"
#include "CVertexBuffer.h"
#include "CIndexBuffer.h"
#include "CMaterial.h"

class CMesh
{
public:

	CVertexBuffer										m_vertexBuffer;
	CIndexBuffer										m_indexBuffer;

	std::vector <unsigned int>			m_indices;
	std::vector <Vertex>						m_vertices;

	unsigned int										m_numIndices;
	unsigned int										m_numVertices;

	CMaterial												m_material;

	void Erase();

	void Initialize(std::vector <std::string>& texturePath);

#ifdef OPEN_GL
	void Render(unsigned int shaderProgram);
#endif

	CMesh();
	~CMesh();
};