#pragma once
#include "Header.h"

#include "CRenderTarget.h"
#include "CShaderProgram.h"
#include "CDepthStencil.h"
#include "CSamplerState.h"
#include "CDeviceContext.h"
#include "CViewportManager.h"
#include "CConstantBuffer.h"
#include "CSamplerState.h"
#include "CSwapChain.h"
#include "CCamera.h"
#include "CMesh.h"

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
	void UpdateModelMatrix(CConstantBuffer& constantBuffer, CMesh& mesh, unsigned int index);
	void UpdateMeshColor(CConstantBuffer& constantBuffer, CMesh& mesh);
	void UpdateLighting(CConstantBuffer& constantBuffer, LightingData& data);

	void SetMesh(CMesh& mesh);
	void Draw(unsigned int numIndices);


#endif

	CDeviceContext() = default;
	~CDeviceContext();
};