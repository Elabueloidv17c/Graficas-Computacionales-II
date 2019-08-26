#pragma once
#include "../Header/CDeviceContext.h"
#include "../Header/CRenderTarget.h"
#include "../Header/CShaderProgram.h"
#include "../Header/CDepthStencil.h"
#include "../Header/CSamplerState.h"
#include "../Header/CDeviceContext.h"
#include "../Header/CRasterizerState.h"
#include "../Header/CViewportManager.h"
#include "../Header/CConstantBuffer.h"
#include "../Header/CSamplerState.h"
#include "../Header/CLightingPass.h"
#include "../Header/CSwapChain.h"
#include "../Header/CCamera.h"
#include "../Header/CModel.h"

CDeviceContext::~CDeviceContext()
{
#ifdef DIRECT_X
	if (m_pointer)
	{
		m_pointer->ClearState();
	}
#endif
}

void CDeviceContext::SetRenderTarget(CRenderTarget& renderTarget, void* depthStencilView)
{
#ifdef DIRECT_X
	m_pointer->OMSetRenderTargets(1, &renderTarget.m_view, (ID3D11DepthStencilView*) depthStencilView);
#endif
#ifdef OPEN_GL
	glBindFramebuffer(GL_FRAMEBUFFER, renderTarget.m_id);
#endif
}

void CDeviceContext::UnSetRenderTarget()
{
#ifdef DIRECT_X
	ID3D11ShaderResourceView* const pSRV[3] = { NULL };
	m_pointer->PSSetShaderResources(0, 3, pSRV);
#endif
#ifdef OPEN_GL
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif
}

void CDeviceContext::Draw(unsigned int numIndices)
{
#ifdef DIRECT_X
	m_pointer->DrawIndexed(numIndices, 0, 0);
#endif
#ifdef OPEN_GL
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, NULL);
#endif
}

void CDeviceContext::SetPrimitiveTopology(unsigned int topology)
{
#ifdef DIRECT_X
	m_pointer->IASetPrimitiveTopology((D3D_PRIMITIVE_TOPOLOGY)topology);
#endif
#ifdef OPEN_GL
	glPolygonMode(GL_FRONT_AND_BACK, topology);
#endif
}

void CDeviceContext::GenerateMipMaps(CTexture& texture)
{
#ifdef DIRECT_X
	m_pointer->GenerateMips(texture.m_pointer);
#endif
#ifdef OPEN_GL
	glGenerateMipmap(GL_TEXTURE_2D);
	glGenerateTextureMipmap(texture.m_id);
#endif
}

void CDeviceContext::SetViewPort(CViewportManager& viewport)
{
#ifdef DIRECT_X
	m_pointer->RSSetViewports(1, &viewport.m_description);
#endif
}

void CDeviceContext::ClearColor(CRenderTarget& renderTarget, Color color)
{
#ifdef DIRECT_X
	m_pointer->ClearRenderTargetView(renderTarget.m_view, &color.r);
#endif
#ifdef OPEN_GL
	glClearColor(color.r, color.g, color.b, color.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#endif
}

void CDeviceContext::ClearDepthStencil(CDepthStencil& depthStencil)
{
#ifdef DIRECT_X
	m_pointer->ClearDepthStencilView(depthStencil.m_view, D3D11_CLEAR_DEPTH, 1.0f, 0);
#endif
#ifdef OPEN_GL
	glClearDepth(1.0f);
#endif
}

void CDeviceContext::SetInputLayout(CShaderProgram& shader)
{
#ifdef DIRECT_X
	m_pointer->IASetInputLayout(shader.m_vertexShader.m_pLayout);
#endif
#ifdef OPEN_GL
	shader.m_vertexShader.m_layout;
#endif
}

void CDeviceContext::SetVertexShader(CShaderProgram& shader)
{
#ifdef DIRECT_X
	m_pointer->VSSetShader(shader.m_vertexShader.m_pointer, NULL, 0);
#endif
#ifdef OPEN_GL
	glUseProgram(shader.m_id);
#endif
}

void CDeviceContext::SetPixelShader(CShaderProgram& shader)
{
#ifdef DIRECT_X
	m_pointer->PSSetShader(shader.m_pixelShader.m_pointer, NULL, 0);
#endif
#ifdef OPEN_GL
	glUseProgram(shader.m_id);
#endif
}

void CDeviceContext::SetVertexConstantBuffer(unsigned int index, CConstantBuffer& constantBuffer)
{
#ifdef DIRECT_X
	m_pointer->VSSetConstantBuffers(index, 1, &constantBuffer.m_Buffer);
#endif
}

void CDeviceContext::SetPixelConstantBuffer(unsigned int index, CConstantBuffer& constantBuffer)
{
#ifdef DIRECT_X
	m_pointer->PSSetConstantBuffers(index, 1, &constantBuffer.m_Buffer);
#endif
}

void CDeviceContext::SetSamplerState(CSamplerState& state)
{
#ifdef DIRECT_X
	m_pointer->PSSetSamplers(0, 1, &state.m_pointer);
#endif
}

void CDeviceContext::SetDepthState(CDepthState& sampler)
{
#ifdef DIRECT_X
	m_pointer->OMSetDepthStencilState(sampler.m_pointer, 0);
#endif
#ifdef OPEN_GL
	if (sampler.m_enable)
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(sampler.m_mode);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
	}
#endif
}

void CDeviceContext::SetRasterizerState(CRasterizerState& state)
{
#ifdef DIRECT_X
	m_pointer->RSSetState(state.m_pointer);
#endif
}

void CDeviceContext::SetMesh(CMesh& mesh, CLightingPass& pass)
{
#ifdef DIRECT_X
	m_pointer->IASetVertexBuffers(0, 1, &mesh.m_vertexBuffer.m_pointer, &mesh.m_vertexBuffer.m_stride, &mesh.m_vertexBuffer.m_offset);
	m_pointer->IASetIndexBuffer(mesh.m_indexBuffer.m_pointer, DXGI_FORMAT_R32_UINT, 0);

	m_pointer->PSSetShaderResources(0, 1, &mesh.m_material.m_diffuse.m_pointer);

	//If the mesh has a normalMap, load it
	if (mesh.m_material.m_normal.m_pointer != nullptr) 
	{
		m_pointer->PSSetShaderResources(1, 1, &mesh.m_material.m_normal.m_pointer);
	}
#endif
#ifdef OPEN_GL
	//DiffuseMap
	glUniform1i(pass.m_texturesCB.m_uniforms[0].location, mesh.m_material.m_diffuse.m_id);
	glActiveTexture(GL_TEXTURE0 + mesh.m_material.m_diffuse.m_id);
	glBindTexture(GL_TEXTURE_2D, mesh.m_material.m_diffuse.m_id);

	if (pass.m_samplerState.m_id != 0) {
		glBindSampler(mesh.m_material.m_diffuse.m_id, pass.m_samplerState.m_id);
	}
	else {
		glBindSampler(mesh.m_material.m_diffuse.m_id, 0);
	}

	//NormalMap
	glUniform1i(pass.m_texturesCB.m_uniforms[1].location, mesh.m_material.m_normal.m_id);
	glActiveTexture(GL_TEXTURE0 + mesh.m_material.m_normal.m_id);
	glBindTexture(GL_TEXTURE_2D, mesh.m_material.m_normal.m_id);

	if (pass.m_samplerState.m_id != 0) {
		glBindSampler(mesh.m_material.m_normal.m_id, pass.m_samplerState.m_id);
	}
	else {
		glBindSampler(mesh.m_material.m_normal.m_id, 0);
	}

	mesh.m_vertexBuffer.Bind();
	mesh.m_indexBuffer.Bind();
#endif
}

void CDeviceContext::SetScreenAlignedQuad(CScreenAlignedQuad& quad, std::vector <CRenderTarget*>& renderTargets)
{
#ifdef DIRECT_X
	m_pointer->IASetVertexBuffers(0, 1, &quad.m_vertexBuffer.m_pointer, &quad.m_vertexBuffer.m_stride, &quad.m_vertexBuffer.m_offset);
	m_pointer->IASetIndexBuffer(quad.m_indexBuffer.m_pointer, DXGI_FORMAT_R32_UINT, 0);

	for (int i = 0; i < renderTargets.size(); i++)
	{
		m_pointer->PSSetShaderResources(i, 1, &renderTargets[i]->m_texture.m_pointer);
	}
#endif
#ifdef OPEN_GL
	quad.m_vertexBuffer.Bind();
	quad.m_indexBuffer.Bind();
#endif
}

void CDeviceContext::UpdateViewMatrix(CConstantBuffer& constantBuffer, CCamera& camera)
{
#ifdef DIRECT_X
		XMMATRIX view = XMMATRIX(&camera.m_viewMatrix[0][0][0]);
		m_pointer->UpdateSubresource(constantBuffer.m_Buffer, 0, NULL, &XMMatrixTranspose(view), 0, 0);
#endif
#ifdef OPEN_GL
		glUniformMatrix4fv(constantBuffer.m_uniforms[0].location, 1, false, &camera.m_viewMatrix[0][0][0]);
#endif
}

void CDeviceContext::UpdateProjectionMatrix(CConstantBuffer& constantBuffer, CCamera& camera)
{
#ifdef DIRECT_X
		XMMATRIX projection = XMMATRIX(&camera.m_projectionMatrix[0][0][0]);
		m_pointer->UpdateSubresource(constantBuffer.m_Buffer, 0, NULL, &XMMatrixTranspose(projection), 0, 0);
#endif
#ifdef OPEN_GL
		glUniformMatrix4fv(constantBuffer.m_uniforms[0].location, 1, false, &camera.m_projectionMatrix[0][0][0]);
#endif
}

void CDeviceContext::UpdateModelMatrix(CConstantBuffer& constantBuffer, CModel& model, unsigned int index)
{
#ifdef DIRECT_X
		XMMATRIX modelMatrix = XMMATRIX(&model.m_transforms[0][index][0][0]);
		m_pointer->UpdateSubresource(constantBuffer.m_Buffer, 0, NULL, &XMMatrixTranspose(modelMatrix), 0, 0);
#endif
#ifdef OPEN_GL
		glUniformMatrix4fv(constantBuffer.m_uniforms[0].location, 1, false, &model.m_transforms[0][index][0][0]);
#endif
}

void CDeviceContext::UpdateColorData(CConstantBuffer& constantBuffer, ColorData& data)
{
#ifdef DIRECT_X
		m_pointer->UpdateSubresource(constantBuffer.m_Buffer, 0, NULL, &data, 0, 0);
#endif
#ifdef OPEN_GL
	glUniform4fv(constantBuffer.m_uniforms[0].location, 1, &data.pointColor.r);
	glUniform4fv(constantBuffer.m_uniforms[1].location, 1, &data.spotColor.r);
	glUniform4fv(constantBuffer.m_uniforms[2].location, 1, &data.diffuseColor.r);
	glUniform4fv(constantBuffer.m_uniforms[3].location, 1, &data.specularColor.r);
	glUniform4fv(constantBuffer.m_uniforms[4].location, 1, &data.ambientColor.r);
	glUniform1f	(constantBuffer.m_uniforms[5].location, data.diffuseIntensity);
	glUniform1f	(constantBuffer.m_uniforms[6].location, data.specularIntensity);
	glUniform1f	(constantBuffer.m_uniforms[7].location, data.ambientIntensity);
#endif
}

void CDeviceContext::UpdateLightingData(CConstantBuffer& constantBuffer, LightingData& data)
{
#ifdef DIRECT_X
		m_pointer->UpdateSubresource(constantBuffer.m_Buffer, 0, NULL, &data, 0, 0);
#endif
#ifdef OPEN_GL
	glUniform3fv(constantBuffer.m_uniforms[0].location, 1, &data.directional.x);
	glUniform1f(constantBuffer.m_uniforms[1].location, data.specularPower);
	glUniform3fv(constantBuffer.m_uniforms[2].location, 1, &data.pointPosition.x);
	glUniform1f(constantBuffer.m_uniforms[3].location, data.PointRadius);
	glUniform3fv(constantBuffer.m_uniforms[4].location, 1, &data.viewPosition.x);
	glUniform1f(constantBuffer.m_uniforms[5].location, data.SpotRadius);
	glUniform3fv(constantBuffer.m_uniforms[6].location, 1, &data.spotPosition.x);
	glUniform1f(constantBuffer.m_uniforms[7].location, data.spotAlpha);
	glUniform3fv(constantBuffer.m_uniforms[8].location, 1, &data.spotDirection.x);
	glUniform1f(constantBuffer.m_uniforms[9].location, data.spotBeta);
#endif
}

void CDeviceContext::UpdateBrightData(CConstantBuffer& constantBuffer, BrightData data)
{
#ifdef DIRECT_X
	m_pointer->UpdateSubresource(constantBuffer.m_Buffer, 0, NULL, &data, 0, 0);
#endif
#ifdef OPEN_GL
	glUniform1i(constantBuffer.m_uniforms[0].location, data.LevelOfDetail);
	glUniform1f(constantBuffer.m_uniforms[1].location, data.BloomThreshold);
#endif
}

void CDeviceContext::UpdateBlurHData(CConstantBuffer& constantBuffer, BlurHData data)
{
#ifdef DIRECT_X
	m_pointer->UpdateSubresource(constantBuffer.m_Buffer, 0, NULL, &data, 0, 0);
#endif
#ifdef OPEN_GL
	glUniform2f(constantBuffer.m_uniforms[0].location, 1, data.ViewportDimensions.width);
	glUniform1i(constantBuffer.m_uniforms[1].location, data.LevelOfDetail);
#endif
}

void CDeviceContext::UpdateBlurVData(CConstantBuffer& constantBuffer, BlurVData data)
{
#ifdef DIRECT_X
	m_pointer->UpdateSubresource(constantBuffer.m_Buffer, 0, NULL, &data, 0, 0);
#endif
#ifdef OPEN_GL
	glUniform2f(constantBuffer.m_uniforms[0].location, 1, data.ViewportDimensions.width);
	glUniform1i(constantBuffer.m_uniforms[1].location, data.LevelOfDetail);
#endif
}

void CDeviceContext::UpdateAddBrightData(CConstantBuffer& constantBuffer, AddBrightData data)
{
#ifdef DIRECT_X
	m_pointer->UpdateSubresource(constantBuffer.m_Buffer, 0, NULL, &data, 0, 0);
#endif
#ifdef OPEN_GL
	glUniform1i(constantBuffer.m_uniforms[0].location, data.LevelOfDetail);
#endif
}

void CDeviceContext::UpdateToneMapData(CConstantBuffer& constantBuffer, ToneMapData data)
{
#ifdef DIRECT_X
	m_pointer->UpdateSubresource(constantBuffer.m_Buffer, 0, NULL, &data, 0, 0);
#endif
#ifdef OPEN_GL
	glUniform1f(constantBuffer.m_uniforms[0].location, data.Exposure);
	glUniform1f(constantBuffer.m_uniforms[1].location, data.BloomMultiplier);
#endif
}
