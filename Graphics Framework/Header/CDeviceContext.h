#pragma once
#include "Header.h"

class CRenderTarget;
class CShaderProgram;
class CDepthStencil;
class CSamplerState;
class CDeviceContext;
class CViewportManager;
class CConstantBuffer;
class CSamplerState;
class CSwapChain;
class CCamera;
class CModel;
class CMesh;

class CDeviceContext
{
public:

#ifdef OPEN_GL

#endif

#ifdef DIRECT_X
	ID3D11DeviceContext*                m_pointer;

	void SetViewPort(CViewportManager& viewport);
	void ClearDepthStencil(CSwapChain& swapchain);
	void SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY topology);

	void SetRenderTarget(CSwapChain& swapchain);
	void ClearColor(CSwapChain& swapchain);
	
	void SetInputLayout(CShaderProgram& shader);
	void SetVertexShader(CShaderProgram& shader);
	void SetPixelShader(CShaderProgram& shader);

	void SetVertexConstantBuffer(unsigned int index, CConstantBuffer& constantBuffer);
	void SetPixelConstantBuffer(unsigned int index, CConstantBuffer& constantBuffer);
	void SetSampler(CSamplerState& sampler);

	void UpdateViewMatrix(CConstantBuffer& constantBuffer, CCamera& camera);
	void UpdateProjectionMatrix(CConstantBuffer& constantBuffer, CCamera& camera);
	void UpdateModelMatrix(CConstantBuffer& constantBuffer, CModel& model, unsigned int index);
	void UpdateColorData(CConstantBuffer& constantBuffer, ColorData& data);
	void UpdateLightingData(CConstantBuffer& constantBuffer, LightingData& data);

	void SetMesh(CMesh& mesh);
	void Draw(unsigned int numIndices);


#endif

	CDeviceContext() = default;
	~CDeviceContext();
};