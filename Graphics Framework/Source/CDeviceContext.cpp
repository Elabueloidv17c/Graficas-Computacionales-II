#pragma once
#include "../Header/CDeviceContext.h"

#ifdef OPEN_GL

#endif

#ifdef DIRECT_X
void CDeviceContext::SetRenderTarget(CSwapChain& swapchain)
{
	m_pointer->OMSetRenderTargets(1, &swapchain.m_renderTarget.m_view, swapchain.m_depthStencil.m_view);
}

void CDeviceContext::SetViewPort(CViewportManager& viewport)
{
	m_pointer->RSSetViewports(1, &viewport.m_description);
}

void CDeviceContext::ClearColor(CSwapChain& swapchain)
{
	m_pointer->ClearRenderTargetView(swapchain.m_renderTarget.m_view, &swapchain.m_color.r);
}

void CDeviceContext::ClearDepthStencil(CSwapChain& swapchain)
{
	m_pointer->ClearDepthStencilView(swapchain.m_depthStencil.m_view, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void CDeviceContext::SetInputLayout(CShaderProgram& shader)
{
	m_pointer->IASetInputLayout(shader.m_pLayout);
}

void CDeviceContext::SetVertexShader(CShaderProgram& shader)
{
	m_pointer->VSSetShader(shader.m_pVertex, NULL, 0);
}

void CDeviceContext::SetPixelShader(CShaderProgram& shader)
{
	m_pointer->PSSetShader(shader.m_pPixel, NULL, 0);
}

void CDeviceContext::SetVertexConstantBuffer(unsigned int index, CConstantBuffer& constantBuffer)
{
	m_pointer->VSSetConstantBuffers(index, 1, &constantBuffer.m_Buffer);
}

void CDeviceContext::SetPixelConstantBuffer(unsigned int index, CConstantBuffer& constantBuffer)
{
	m_pointer->PSSetConstantBuffers(index, 1, &constantBuffer.m_Buffer);
}

void CDeviceContext::SetSampler(CSamplerState& sampler)
{
	m_pointer->PSSetSamplers(0, 1, &sampler.m_pointer);
}

void CDeviceContext::SetMesh(CMesh& mesh)
{
	m_pointer->IASetVertexBuffers(0, 1, &mesh.m_vertexBuffer.m_pointer, &mesh.m_vertexBuffer.m_stride, &mesh.m_vertexBuffer.m_offset);
	m_pointer->IASetIndexBuffer(mesh.m_indexBuffer.m_pointer, DXGI_FORMAT_R32_UINT, 0);
	m_pointer->PSSetShaderResources(0, 1, &mesh.m_texture);
}

void CDeviceContext::UpdateViewMatrix(CConstantBuffer& constantBuffer, CCamera& camera)
{
	XMMATRIX view = XMMATRIX(&camera.m_viewMatrix[0][0]);
	m_pointer->UpdateSubresource(constantBuffer.m_Buffer, 0, NULL, &XMMatrixTranspose(view), 0, 0);
}

void CDeviceContext::UpdateProjectionMatrix(CConstantBuffer& constantBuffer, CCamera& camera)
{
	XMMATRIX projection = XMMATRIX(&camera.m_projectionMatrix[0][0]);
	m_pointer->UpdateSubresource(constantBuffer.m_Buffer, 0, NULL, &XMMatrixTranspose(projection), 0, 0);
}

void CDeviceContext::UpdateWorldMatrix(CConstantBuffer& constantBuffer, CMesh& mesh, unsigned int index)
{
	XMMATRIX model = XMMATRIX(&mesh.m_transforms[index][0][0]);
	m_pointer->UpdateSubresource(constantBuffer.m_Buffer, 0, NULL, &XMMatrixTranspose(model), 0, 0);
}

void CDeviceContext::UpdateMeshColor(CConstantBuffer& constantBuffer, CMesh& mesh)
{
	m_pointer->UpdateSubresource(constantBuffer.m_Buffer, 0, NULL, &mesh.m_color, 0, 0);
}

void CDeviceContext::UpdateLighting(CConstantBuffer& constantBuffer, Vector& direction)
{
	m_pointer->UpdateSubresource(constantBuffer.m_Buffer, 0, NULL, &direction, 0, 0);
}

void CDeviceContext::SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY topology)
{
	m_pointer->IASetPrimitiveTopology(topology);
}

void CDeviceContext::Draw(unsigned int numIndices)
{
	m_pointer->DrawIndexed(numIndices, 0, 0);
}
#endif

CDeviceContext::~CDeviceContext()
{
#ifdef DIRECT_X
	if (m_pointer)
	{
		m_pointer->ClearState();
	}
#endif
}