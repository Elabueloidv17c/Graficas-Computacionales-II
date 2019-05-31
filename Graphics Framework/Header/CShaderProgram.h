#pragma once
#include "Header.h"
#include "CVertexShader.h"
#include "CPixelShader.h"

#ifdef OPEN_GL
class CCamera;
class CModel;
#endif

#ifdef DIRECT_X
#include "CConstantBuffer.h"
#endif

class CShaderProgram
{
public:

	CVertexShader						m_vertexShader;
	CPixelShader						m_pixelShader;

#ifdef DIRECT_X
	CConstantBuffer							m_viewCB;
	CConstantBuffer							m_projectionCB;
	CConstantBuffer							m_modelCB;
	CConstantBuffer							m_colorDataCB;
	CConstantBuffer							m_lightingDataCB;

	HRESULT CompileFromFile();
#endif

#ifdef OPEN_GL
	int m_id;

	void Bind();
	void Unbind();

	//All of this methods should be inside DeviceContext class
	bool UpdateWorld(MATRIX4& data);
	bool UpdateModel(MATRIX4& transform);
	bool UpdateCamera(CCamera& camera);
	bool UpdateLight(LightingData& data);
	bool UpdateColor(ColorData& data);
#endif

	bool Initialize(ShaderData vertexData, ShaderData pixelData);
	void Erase();

	CShaderProgram();
	~CShaderProgram();
};