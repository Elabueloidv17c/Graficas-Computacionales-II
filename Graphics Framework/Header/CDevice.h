#pragma once
#include "Header.h"
#include "CScreenAlignedQuad.h"
#include "CRasterizerState.h"
#include "CRasterizerState.h"
#include "CConstantBuffer.h"
#include "CDeviceContext.h"
#include "CShaderProgram.h"
#include "CVertexBuffer.h"
#include "CSamplerState.h"
#include "CDepthStencil.h"
#include "CRenderTarget.h"
#include "CIndexBuffer.h"
#include "CDepthState.h"
#include "CSwapChain.h"
#include "CMesh.h"

class CDevice
{
public:

#ifdef DIRECT_X
	ID3D11Device*								m_pointer;
	D3D_DRIVER_TYPE							m_driverType;
#endif

	long Initialize(CDeviceContext& context, CSwapChain& swapchain);

	void CreateRenderTargetView(CRenderTarget& renderTarget, void* pBackBuffer);
	void CreateDepthStencilTexture(CDepthStencil& depthStencil);
	void CreateDepthStencilView(CDepthStencil& depthStencil);

	void CreateVertexShader(CShaderProgram& shader);
	void CreateInputLayout(CShaderProgram& shader);
	void CreatePixelShader(CShaderProgram& shader);

	void CreateConstantBuffer(CConstantBuffer& buffer);

	void CreateBuffers(CScreenAlignedQuad& quad);
	void CreateBuffers(CMesh& mesh);
	void CreateTexture(CMesh& mesh);

	void CreateRasterizerState(CRasterizerState& state);
	void CreateSamplerState(CSamplerState& state);
	void CreateDepthState(CDepthState& state);

	CDevice() = default;
	~CDevice();
};