#pragma once
#include "Header.h"

class CScreenAlignedQuad;
class CRasterizerState;
class CViewportManager;
class CConstantBuffer;
class CDeviceContext;
class CShaderProgram;
class CRenderTarget;
class CSamplerState;
class CSamplerState;
class CDepthStencil;
class CLightingPass;
class CDepthState;
class CSwapChain;
class CTexture;
class CCamera;
class CModel;
class CMesh;

class CDeviceContext
{
public:

#ifdef DIRECT_X
	ID3D11DeviceContext*                m_pointer;
#endif

	void GenerateMipMaps(CTexture& texture);

	void SetViewPort(CViewportManager& viewport);
	void ClearDepthStencil(CDepthStencil& depthStencil);

	void SetPrimitiveTopology(unsigned int topology);
	void SetRenderTarget(CRenderTarget& renderTarget, void* depthStencil);

	void UnSetRenderTarget();

	void ClearColor(CRenderTarget& renderTarget, Color color);
	
	void SetInputLayout(CShaderProgram& shader);
	void SetVertexShader(CShaderProgram& shader);
	void SetPixelShader(CShaderProgram& shader);

	void SetRasterizerState(CRasterizerState& state);
	void SetSamplerState(CSamplerState& sampler);
	void SetDepthState(CDepthState& sampler);

	void SetVertexConstantBuffer(unsigned int index, CConstantBuffer& constantBuffer);
	void SetPixelConstantBuffer(unsigned int index, CConstantBuffer& constantBuffer);
	
	void UpdateViewMatrix(CConstantBuffer& constantBuffer, CCamera& camera);
	void UpdateProjectionMatrix(CConstantBuffer& constantBuffer, CCamera& camera);
	void UpdateModelMatrix(CConstantBuffer& constantBuffer, CModel& model, unsigned int index);
	
	void UpdateColorData(CConstantBuffer& constantBuffer, ColorData& data);
	void UpdateLightingData(CConstantBuffer& constantBuffer, LightingData& data);

	void UpdateBrightData(CConstantBuffer& constantBuffer, BrightData data);
	void UpdateBlurHData(CConstantBuffer& constantBuffer, BlurHData data);
	void UpdateBlurVData(CConstantBuffer& constantBuffer, BlurVData data);
	void UpdateAddBrightData(CConstantBuffer& constantBuffer, AddBrightData data);
	void UpdateToneMapData(CConstantBuffer& constantBuffer, ToneMapData data);

	void SetScreenAlignedQuad(CScreenAlignedQuad& quad, std::vector <CRenderTarget*>& renderTargets);
	void SetMesh(CMesh& mesh, CLightingPass& pass);
	
	void Draw(unsigned int numIndices);

	CDeviceContext() = default;
	~CDeviceContext();
};