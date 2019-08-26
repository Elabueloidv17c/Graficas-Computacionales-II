#include "../Header/CToneMapPass.h"

bool CToneMapPass::Initialize(CSwapChain* backBuffer, CDevice& device, Size size, Color* clearColor, CScreenAlignedQuad* SAQ,
	std::vector <CRenderTarget*> renderTargets, ShaderProgramData data)
{
	//----------------------------------------------------------------------------------------------------------
	//get render target textures and clear color.
	//----------------------------------------------------------------------------------------------------------
	m_inTextures = renderTargets;
	m_clearColor = clearColor;

	m_backBuffer = backBuffer;
	//----------------------------------------------------------------------------------------------------------
	//Set Screen Aligned Quad reference.
	//----------------------------------------------------------------------------------------------------------
	m_screenAlignedQuad = SAQ;

	//----------------------------------------------------------------------------------------------------------
	//Create pass shaders
	//----------------------------------------------------------------------------------------------------------
	m_shaderProgram = new CShaderProgram();

	if (!m_shaderProgram->Initialize(data.vertex, data.pixel))
	{
		return false;
	}

	device.CreateVertexShader(*m_shaderProgram);
	device.CreatePixelShader(*m_shaderProgram);
	device.CreateInputLayout(*m_shaderProgram);

#ifdef DIRECT_X
	m_dataCB.Initialize(sizeof(ToneMapData));
#endif
#ifdef OPEN_GL
	std::vector <std::string> names;
	names.push_back("Exposure");
	names.push_back("BloomMultiplier");
	m_dataCB.Initialize(&m_shaderProgram->m_id, names);

	names.clear();
	names.push_back("LightingPass");
	names.push_back("AddBrightPass");
	m_texturesCB.Initialize(&m_shaderProgram->m_id, names);
	device.CreateConstantBuffer(m_texturesCB);
#endif

	//Create the constant buffers to pass information to the shader
	device.CreateConstantBuffer(m_dataCB);

#ifdef DIRECT_X
	
	//----------------------------------------------------------------------------------------------------------
	//Set States description.
	//----------------------------------------------------------------------------------------------------------
	m_rasterizerState.Initialize(D3D11_FILL_SOLID, D3D11_CULL_NONE, false, 0, 0.0f, 0.0f, false, false, false, false);
	device.CreateRasterizerState(m_rasterizerState);

	m_samplerState.Initialize(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP,
	D3D11_TEXTURE_ADDRESS_WRAP, 0.0f, 1, D3D11_COMPARISON_NEVER, Color{ 1.0f, 1.0f, 1.0f, 1.0f }, 0, D3D11_FLOAT32_MAX);
	device.CreateSamplerState(m_samplerState);

	m_depthState.Initialize(false, D3D11_DEPTH_WRITE_MASK_ALL, D3D11_COMPARISON_LESS, false, D3D11_DEFAULT_STENCIL_READ_MASK,
	D3D11_DEFAULT_STENCIL_WRITE_MASK, D3D11_COMPARISON_ALWAYS, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS, D3D11_STENCIL_OP_KEEP,
	D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP);
	device.CreateDepthState(m_depthState);
#endif
#ifdef OPEN_GL
	//----------------------------------------------------------------------------------------------------------
	//Set States description.
	//----------------------------------------------------------------------------------------------------------
	m_rasterizerState.Initialize();
	device.CreateRasterizerState(m_rasterizerState);
	
	m_samplerState.Initialize();
	device.CreateSamplerState(m_samplerState);
	
	m_depthState.Initialize();
	device.CreateDepthState(m_depthState);
#endif

	return true;
}

void 
CToneMapPass::Render(CDeviceContext& deviceContext) {
#ifdef OPEN_GL
	glUniform1i(m_texturesCB.m_uniforms[0].location, m_inTextures[0]->m_texture.m_id);
	glActiveTexture(GL_TEXTURE0 + m_inTextures[0]->m_texture.m_id);
	glBindTexture(GL_TEXTURE_2D, m_inTextures[0]->m_texture.m_id);

	if (m_samplerState.m_id != 0) {
		glBindSampler(m_inTextures[0]->m_texture.m_id, m_samplerState.m_id);
	}
	else {
		glBindSampler(m_inTextures[0]->m_texture.m_id, 0);
	}

	glUniform1i(m_texturesCB.m_uniforms[1].location, m_inTextures[1]->m_texture.m_id);
	glActiveTexture(GL_TEXTURE0 + m_inTextures[1]->m_texture.m_id);
	glBindTexture(GL_TEXTURE_2D, m_inTextures[1]->m_texture.m_id);

	if (m_samplerState.m_id != 0) {
		glBindSampler(m_inTextures[1]->m_texture.m_id, m_samplerState.m_id);
	}
	else {
		glBindSampler(m_inTextures[1]->m_texture.m_id, 0);
	}
#endif

	m_screenAlignedQuad->Render(deviceContext, m_inTextures);
}

void CToneMapPass::Clear(CDeviceContext& deviceContext, CRenderTarget& renderTarget)
{
	deviceContext.ClearColor(renderTarget, *m_clearColor);
	if (m_backBuffer) {
		deviceContext.ClearDepthStencil(m_backBuffer->m_depthStencil);
	}
}

void 
CToneMapPass::Set(CDeviceContext& deviceContext, CRenderTarget& renderTarget) {
	//Set Pipeline States
	deviceContext.SetSamplerState(m_samplerState);
	deviceContext.SetRasterizerState(m_rasterizerState);
	deviceContext.SetDepthState(m_depthState);

	//Set Shaders
	deviceContext.SetVertexShader(*m_shaderProgram);
	deviceContext.SetPixelShader(*m_shaderProgram);
	deviceContext.SetInputLayout(*m_shaderProgram);

	//Set the vertex and pixel shaders to the constant buffers
	deviceContext.SetPixelConstantBuffer(0, m_dataCB);
	deviceContext.UpdateToneMapData(m_dataCB, m_screenAlignedQuad->m_toneMapData);

	//Set Render Target
	deviceContext.SetRenderTarget(renderTarget, nullptr);
}

bool CToneMapPass::Resize(CDevice& device, Size size)
{
#ifdef DIRECT_X
	if (m_backBuffer) {
		m_backBuffer->Resize(device, size);
	}
#endif

	return true;
}

void CToneMapPass::Update()
{

}

void CToneMapPass::Erase()
{

}

CToneMapPass::CToneMapPass()
{
#ifdef DIRECT_X
	m_shaderProgram = nullptr;
	m_clearColor = nullptr;
#endif
}

CToneMapPass::~CToneMapPass()
{
#ifdef DIRECT_X
	if (m_shaderProgram)
	{
		delete m_shaderProgram;
	}
	m_shaderProgram = nullptr;
#endif
}
