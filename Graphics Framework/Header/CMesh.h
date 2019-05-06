#pragma once
#include "Header.h"
#include "CVertexBuffer.h"
#include "CIndexBuffer.h"
#include "CTexture.h"

class CMesh
{
public:

	bool										m_isRotating;
	bool										m_isParty;
	float										m_scale;
	Color										m_color;
	std::vector <MATRIX4>						m_transforms;

	CVertexBuffer								m_vertexBuffer;
	CIndexBuffer								m_indexBuffer;

	std::string									m_texturePath;

	std::vector <unsigned int>					m_indices;
	std::vector <Vertex>						m_vertices;

	unsigned int								m_numIndices;
	unsigned int								m_numVertices;

#ifdef DIRECT_X
	ID3D11ShaderResourceView*					m_texture;
#endif

#ifdef OPEN_GL
	CTexture									m_texture;
#endif

	void Update(float time, std::string modelPath);
	void Erase();

	void SetTransform(MATRIX4 position, unsigned int index);
	void Rotate(float speed);
	void ChangeScale();
	void PartyMode();

	void AddTransform(MATRIX4 position);

#ifdef DIRECT_X
	void Initialize(std::string texturePath);
	void Render(float time, std::string modelPath);
#endif

#ifdef OPEN_GL
	void Initialize(std::string texturePath);
	void Render(unsigned int shaderProgram);
#endif

	CMesh();
	~CMesh();
};