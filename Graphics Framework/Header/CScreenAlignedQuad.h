#pragma once
#include "Header.h"
#include "CVertexBuffer.h"
#include "CIndexBuffer.h"

class CRenderTarget;
class CDeviceContext;
class CDevice;

class CScreenAlignedQuad
{
public:

	CVertexBuffer								m_vertexBuffer;
	CIndexBuffer								m_indexBuffer;

	std::vector <unsigned int>	m_indices;
	std::vector <Vertex>				m_vertices;

	unsigned int								m_numIndices;
	unsigned int								m_numVertices;

	std::string									m_path;

	BrightData									m_brightData;
	BlurHData										m_blurHData;
	BlurVData										m_blurVData;
	AddBrightData								m_addBrightData;
	ToneMapData									m_toneMapData;

	bool Initialize(const char* path, CDevice& device);
	void Render(CDeviceContext& deviceContext, std::vector <CRenderTarget*>& renderTargets);

	void SetBrightData(BrightData data);
	void SetBlurHData(BlurHData data);
	void SetBlurVData(BlurVData data);
	void SetAddBrightData(AddBrightData data);
	void SetToneMapData(ToneMapData data);
	void Update(Size window);

#ifdef OPEN_GL
	void Render(unsigned int shaderProgram);
#endif

	CScreenAlignedQuad() = default;
	~CScreenAlignedQuad() = default;
};