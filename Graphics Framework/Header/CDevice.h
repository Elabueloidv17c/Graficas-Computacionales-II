#pragma once
#include "Header.h"

#include "CRenderTarget.h"
#include "CDepthStencil.h"
#include "CSamplerState.h"
#include "CDeviceContext.h"
#include "CShaderProgram.h"
#include "CVertexBuffer.h"
#include "CIndexBuffer.h"
#include "CSwapChain.h"
#include "CMesh.h"

class CDevice
{
public:

#ifdef OPEN_GL

#endif

#ifdef DIRECT_X
	ID3D11Device*								m_pointer;
	D3D_DRIVER_TYPE								m_driverType;

	HRESULT Initialize(CDeviceContext& context, CSwapChain& swapchain);

	void CreateRenderTargetView(CRenderTarget& renderTarget, ID3D11Texture2D* pBackBuffer);
	void CreateDepthStencilTexture(CDepthStencil& depthStencil);
	void CreateDepthStencilView(CDepthStencil& depthStencil);
	void CreateSamplerState(CSamplerState& sampler);

	void CreateVertexShader(CShaderProgram& shader);
	void CreateInputLayout(CShaderProgram& shader);
	void CreatePixelShader(CShaderProgram& shader);

	void CreateConstantBuffer(CConstantBuffer& buffer, unsigned int size);

	void CreateTexture(CMesh& mesh);
	void CreateBuffers(CMesh& mesh);

#endif

	CDevice();
	~CDevice();
};