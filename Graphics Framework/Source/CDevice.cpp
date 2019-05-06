#pragma once
#include "../Header/CDevice.h"

CDevice::CDevice()
{

}

CDevice::~CDevice()
{
#ifdef DIRECT_X
	if (m_pointer)
	{
		m_pointer->Release();
	}
#endif
}

#ifdef OPEN_GL

#endif

#ifdef DIRECT_X
HRESULT CDevice::Initialize(CDeviceContext& context, CSwapChain& swapchain)
{
	HRESULT result;

	UINT createDeviceFlags = 0;

#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] = {
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};

	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};

	D3D_FEATURE_LEVEL featureLevel = featureLevels[0];

	for (UINT driverTypeIndex = 0; driverTypeIndex < ARRAYSIZE(driverTypes); driverTypeIndex++)
	{
		m_driverType = driverTypes[driverTypeIndex];

		result = D3D11CreateDeviceAndSwapChain(NULL, driverTypes[driverTypeIndex], NULL, createDeviceFlags, featureLevels, 
		ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &swapchain.m_description, &swapchain.m_pointer, &m_pointer, &featureLevel, 
		&context.m_pointer);

		if (SUCCEEDED(result))
		{
			ID3D11Texture2D* pBackBuffer = NULL;
			swapchain.m_pointer->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			CreateRenderTargetView(swapchain.m_renderTarget, pBackBuffer);
			pBackBuffer->Release();

			CreateDepthStencilTexture(swapchain.m_depthStencil);
			CreateDepthStencilView(swapchain.m_depthStencil);

			break;
		}
	}

	return result;
}

void CDevice::CreateRenderTargetView(CRenderTarget& renderTarget, ID3D11Texture2D* pBackBuffer)
{
	m_pointer->CreateRenderTargetView(pBackBuffer, NULL, &renderTarget.m_view);
}

void CDevice::CreateDepthStencilTexture(CDepthStencil& depthStencil)
{
	m_pointer->CreateTexture2D(&depthStencil.m_texture.m_description, NULL, &depthStencil.m_texture.m_data);
}

void CDevice::CreateDepthStencilView(CDepthStencil& depthStencil)
{
	m_pointer->CreateDepthStencilView(depthStencil.m_texture.m_data, &depthStencil.m_description, &depthStencil.m_view);
}

void CDevice::CreateSamplerState(CSamplerState& sampler)
{
	m_pointer->CreateSamplerState(&sampler.m_description, &sampler.m_pointer);
}

void CDevice::CreateInputLayout(CShaderProgram& shader)
{
	m_pointer->CreateInputLayout(&shader.m_layout[0], shader.m_layoutSize, shader.m_pVertexBlob->GetBufferPointer(), 
	shader.m_pVertexBlob->GetBufferSize(), &shader.m_pLayout);
}

void CDevice::CreateVertexShader(CShaderProgram& shader)
{
	m_pointer->CreateVertexShader(shader.m_pVertexBlob->GetBufferPointer(), shader.m_pVertexBlob->GetBufferSize(), 
	NULL, &shader.m_pVertex);
}

void CDevice::CreatePixelShader(CShaderProgram& shader)
{
	m_pointer->CreatePixelShader(shader.m_pPixelBlob->GetBufferPointer(), shader.m_pPixelBlob->GetBufferSize(), 
	NULL, &shader.m_pPixel);
}

void CDevice::CreateConstantBuffer(CConstantBuffer& buffer, unsigned int size)
{
	buffer.Initialize(size);
	m_pointer->CreateBuffer(&buffer.m_description, NULL, &buffer.m_Buffer);
}

void CDevice::CreateTexture(CMesh& mesh)
{
	D3DX11CreateShaderResourceViewFromFile(m_pointer, &mesh.m_texturePath[0], NULL, NULL, &mesh.m_texture, NULL);
}

void CDevice::CreateBuffers(CMesh& mesh)
{
	m_pointer->CreateBuffer(&mesh.m_vertexBuffer.m_description, &mesh.m_vertexBuffer.m_InitData, &mesh.m_vertexBuffer.m_pointer);
	m_pointer->CreateBuffer(&mesh.m_indexBuffer.m_description, &mesh.m_indexBuffer.m_InitData, &mesh.m_indexBuffer.m_pointer);
}
#endif

