#pragma once
#include "Header.h"

#ifdef OPEN_GL
#include "CShader.h"
#include "CCamera.h"
#include "CMesh.h"
#endif

#ifdef DIRECT_X
#include "CConstantBuffer.h"
#endif

class CShaderProgram
{
public:

#ifdef DIRECT_X
	const char*								m_filePath;
	unsigned int							m_layoutSize;

	ID3D11InputLayout*						m_pLayout;
	std::vector <D3D11_INPUT_ELEMENT_DESC>	m_layout;

	ID3D11VertexShader*						m_pVertex;
	ID3DBlob*								m_pVertexBlob;
	LPCSTR									m_vertexEntryPoint;
	LPCSTR									m_VertexShaderModel;

	ID3D11PixelShader*						m_pPixel;
	ID3DBlob*								m_pPixelBlob;
	LPCSTR									m_pixelEntryPoint;
	LPCSTR									m_pixelShaderModel;

	CConstantBuffer							m_viewCB;
	CConstantBuffer							m_projectionCB;
	CConstantBuffer							m_worldCB;
	CConstantBuffer							m_colorCB;
	CConstantBuffer							m_lightingDataCB;

	HRESULT CompileFromFile();
	HRESULT CreateLayout();
	HRESULT Initialize(const char* fileName, LPCSTR vertexShaderModel, LPCSTR pixelShaderModel, LPCSTR vertexEntryPoint, LPCSTR pixelEntryPoint);

#endif

#ifdef OPEN_GL
	CShader m_VertexShader;					
	CShader m_FragmentShader;				

	int m_id;								

	void Erase();																
	bool Initialize(std::string vertexShaderPath, std::string pixelShaderPath);

	void Bind();
	void Unbind();

	bool UpdateMesh(CMesh& mesh, int index);
	bool UpdateCamera(CCamera camera);
	bool UpdateLight(LightingData data);
#endif

	CShaderProgram();
	~CShaderProgram();
};