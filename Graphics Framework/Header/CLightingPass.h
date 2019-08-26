#pragma once
#include "CConstantBuffer.h"
#include "CRenderPass.h"
#include "CSwapChain.h"
#include "CCamera.h"
#include "CScene.h"

class CLightingPass : public CRenderPass
{
public:

	CScene*									m_scene;
	CDepthStencil*					m_depthStencil;

	CRasterizerState				m_rasterizerState;
	CSamplerState						m_samplerState;
	CDepthState							m_depthState;

	CConstantBuffer					m_worldCB;
	CConstantBuffer					m_modelCB;
	CConstantBuffer					m_viewCB;
	CConstantBuffer					m_projectionCB;
	CConstantBuffer					m_colorDataCB;
	CConstantBuffer					m_lightingDataCB;
	CConstantBuffer					m_texturesCB;

#ifdef OPEN_GL
	Color										m_color;
	Size										m_size;
#endif

	bool Initialize(CDevice& device, Size size, Color* clearColor, CShaderProgram* shader, std::vector <CRenderTarget*> renderTargets,
	CDepthStencil* depth, CScene* scene);

	void Render(CDeviceContext& deviceContext, CCamera* camera, bool isVR);
	void Clear(CDeviceContext& deviceContext);
	void Set(CDeviceContext& deviceContext);
	bool Resize(CDevice& device, Size size);
	void Update(CCamera* camera);
	void Erase();

	void SetShaderProgram(CShaderProgram* shader);

	CLightingPass();
	~CLightingPass();
};