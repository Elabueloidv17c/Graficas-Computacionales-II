#pragma once
#include "CScreenAlignedQuad.h"
#include "CRenderPass.h"

class CLuminancePass : public CRenderPass
{
public:

	CScreenAlignedQuad*								m_screenAlignedQuad;

	CRasterizerState									m_rasterizerState;
	CSamplerState											m_samplerState;
	CDepthState												m_depthState;

#ifdef OPEN_GL
	CConstantBuffer										m_texturesCB;
#endif

	bool Initialize(CDevice& device, Size size, Color* clearColor, CScreenAlignedQuad* SAQ, std::vector <CRenderTarget*> inTextures,
	ShaderProgramData data);

	void Render(CDeviceContext& deviceContext);
	void Clear(CDeviceContext& deviceContext);
	void Set(CDeviceContext& deviceContext);
	bool Resize(CDevice& device, Size size);
	void Update();
	void Erase();

	CLuminancePass();
	~CLuminancePass();
};