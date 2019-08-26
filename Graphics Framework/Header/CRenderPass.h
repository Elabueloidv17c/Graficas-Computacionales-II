#pragma once
#include "Header.h"
#include "CDevice.h"
#include "CDepthState.h"
#include "CSamplerState.h"
#include "CRenderTarget.h"
#include "CDeviceContext.h"
#include "CShaderProgram.h"
#include "CRasterizerState.h"

class CRenderPass
{
public:

	CShaderProgram*										m_shaderProgram;
	Color*														m_clearColor;

	CRenderTarget											m_renderTarget;
	std::vector <CRenderTarget*>			m_inTextures;

	CRasterizerState									m_rasterizerState;
	CSamplerState											m_samplerState;
	CDepthState												m_depthState;

#ifdef DIRECT_X
	D3D11_SHADER_RESOURCE_VIEW_DESC		m_description;
#endif

	virtual bool Resize(CDevice& device, Size size);
	virtual void Clear(CDeviceContext& deviceContext);
	virtual void Update();
	virtual void Render(CDeviceContext& deviceContext);
	virtual void Set(CDeviceContext& deviceContext);
	virtual void Erase();


	CRenderPass();
	virtual ~CRenderPass();
};