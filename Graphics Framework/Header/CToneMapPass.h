#pragma once
#include "CScreenAlignedQuad.h"
#include "CRenderPass.h"

class CToneMapPass : public CRenderPass
{
public:

	CScreenAlignedQuad*								m_screenAlignedQuad;
	CSwapChain*												m_backBuffer;

	CRasterizerState									m_rasterizerState;
	CSamplerState											m_samplerState;
	CDepthState												m_depthState;

	CConstantBuffer										m_dataCB;

#ifdef OPEN_GL
	CConstantBuffer										m_texturesCB;
#endif

	bool Initialize(CSwapChain* backBuffer, CDevice& device, Size size, Color* clearColor, CScreenAlignedQuad* SAQ,
	std::vector <CRenderTarget*> renderTargets, ShaderProgramData data);

	void Render(CDeviceContext& deviceContext);
	void Clear(CDeviceContext& deviceContext, CRenderTarget& renderTarget);
	void Set(CDeviceContext& deviceContext, CRenderTarget& renderTarget);
	bool Resize(CDevice& device, Size size);
	void Update();
	void Erase();

	CToneMapPass();
	~CToneMapPass();
};