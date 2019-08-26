#include "../Header/CBlurHPass.h"

bool CBlurHPass::Initialize(CDevice& device, Size size, Color* clearColor, CScreenAlignedQuad* SAQ,
	std::vector <CRenderTarget*> renderTargets, ShaderProgramData data)
{
	//----------------------------------------------------------------------------------------------------------
	//get render target textures and clear color.
	//----------------------------------------------------------------------------------------------------------
	m_inTextures = renderTargets;
	m_clearColor = clearColor;

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
	m_dataCB.Initialize(sizeof(AddBrightData));
#endif
#ifdef OPEN_GL
	std::vector <std::string> names;
	names.push_back("ViewportDimensions");
	names.push_back("LevelOfDetail");
	m_dataCB.Initialize(&m_shaderProgram->m_id, names);

	names.clear();
	names.push_back("BrightPass");
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

	//----------------------------------------------------------------------------------------------------------
	// Setup the render target texture description.
	//----------------------------------------------------------------------------------------------------------
	HRESULT result;

	ZeroMemory(&m_renderTarget.m_texture.m_description, sizeof(m_renderTarget.m_texture.m_description));

	m_renderTarget.m_texture.m_description.Width = size.width;
	m_renderTarget.m_texture.m_description.Height = size.height;
	m_renderTarget.m_texture.m_description.MipLevels = 10;
	m_renderTarget.m_texture.m_description.ArraySize = 1;
	m_renderTarget.m_texture.m_description.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	m_renderTarget.m_texture.m_description.SampleDesc.Count = 1;
	m_renderTarget.m_texture.m_description.Usage = D3D11_USAGE_DEFAULT;
	m_renderTarget.m_texture.m_description.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	m_renderTarget.m_texture.m_description.CPUAccessFlags = 0;
	m_renderTarget.m_texture.m_description.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	//----------------------------------------------------------------------------------------------------------
	// Create the render target texture.
	//----------------------------------------------------------------------------------------------------------
	result = device.m_pointer->CreateTexture2D(&m_renderTarget.m_texture.m_description, NULL, &m_renderTarget.m_texture.m_data);
	if (FAILED(result))
	{
		return false;
	}

	//----------------------------------------------------------------------------------------------------------
	// Setup the description of the render target view.
	//----------------------------------------------------------------------------------------------------------
	m_renderTarget.m_description.Format = m_renderTarget.m_texture.m_description.Format;
	m_renderTarget.m_description.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	m_renderTarget.m_description.Texture2D.MipSlice = 0;

	//----------------------------------------------------------------------------------------------------------
	// Create the render target view.
	//----------------------------------------------------------------------------------------------------------
	result = device.m_pointer->CreateRenderTargetView(m_renderTarget.m_texture.m_data, &m_renderTarget.m_description, &m_renderTarget.m_view);
	if (FAILED(result))
	{
		return false;
	}

	//----------------------------------------------------------------------------------------------------------
	// Setup the description of the shader resource view.
	//----------------------------------------------------------------------------------------------------------
	m_description.Format = m_renderTarget.m_texture.m_description.Format;
	m_description.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	m_description.Texture2D.MostDetailedMip = 0;
	m_description.Texture2D.MipLevels = 10;

	//----------------------------------------------------------------------------------------------------------
	// Create the shader resource view.
	//----------------------------------------------------------------------------------------------------------
	result = device.m_pointer->CreateShaderResourceView(m_renderTarget.m_texture.m_data, &m_description, &m_renderTarget.m_texture.m_pointer);
	if (FAILED(result))
	{
		return false;
	}
#endif
#ifdef OPEN_GL
	//----------------------------------------------------------------------------------------------------------
	//Set States description.
	//----------------------------------------------------------------------------------------------------------
	m_rasterizerState.Initialize();
	device.CreateRasterizerState(m_rasterizerState);
	
	m_samplerState.Initialize();
	device.CreateSamplerState(m_samplerState);
	
	m_depthState.Initialize(false, GL_LESS);
	device.CreateDepthState(m_depthState);

	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glGenTextures(1, &m_renderTarget.m_texture.m_id);
	glBindTexture(GL_TEXTURE_2D, m_renderTarget.m_texture.m_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.width, size.height, 0, GL_RGBA, GL_FLOAT, NULL);

	glSamplerParameterf(m_renderTarget.m_texture.m_id, GL_TEXTURE_MIN_LOD, 0);
	glSamplerParameterf(m_renderTarget.m_texture.m_id, GL_TEXTURE_MAX_LOD, 10);
	glSamplerParameterf(m_renderTarget.m_texture.m_id, GL_TEXTURE_LOD_BIAS, 0.0);
	glGenerateMipmap(GL_TEXTURE_2D);

	glGenFramebuffers(1, &m_renderTarget.m_id);
	glBindFramebuffer(GL_FRAMEBUFFER, m_renderTarget.m_id);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_renderTarget.m_texture.m_id, 0);

	GLenum DrawBuffers2[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers2);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		return false;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
#endif
	return true;
}

void CBlurHPass::Render(CDeviceContext& deviceContext)
{
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
#endif
	m_screenAlignedQuad->Render(deviceContext, m_inTextures);
}

void CBlurHPass::Clear(CDeviceContext& deviceContext)
{
	deviceContext.ClearColor(m_renderTarget, *m_clearColor);
}

void CBlurHPass::Set(CDeviceContext& deviceContext)
{
	//Generate mipmaps
	deviceContext.GenerateMipMaps(m_renderTarget.m_texture);

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
	deviceContext.UpdateBlurHData(m_dataCB, m_screenAlignedQuad->m_blurHData);

	//Set Render Target
	deviceContext.SetRenderTarget(m_renderTarget, nullptr);
}

bool CBlurHPass::Resize(CDevice& device, Size size)
{
#ifdef DIRECT_X
	HRESULT result;

	m_renderTarget.m_view->Release();
	m_renderTarget.m_texture.m_data->Release();
	m_renderTarget.m_texture.m_pointer->Release();

	m_renderTarget.m_texture.m_description.Width = size.width;
	m_renderTarget.m_texture.m_description.Height = size.height;

	// Create the render target texture.
	result = device.m_pointer->CreateTexture2D(&m_renderTarget.m_texture.m_description, NULL, &m_renderTarget.m_texture.m_data);

	if (FAILED(result))
	{
		return false;
	}

	// Setup the description of the render target view.
	m_renderTarget.m_description.Format = m_renderTarget.m_texture.m_description.Format;

	// Create the render target view.
	result = device.m_pointer->CreateRenderTargetView(m_renderTarget.m_texture.m_data, &m_renderTarget.m_description, &m_renderTarget.m_view);

	if (FAILED(result))
	{
		return false;
	}

	// Setup the description of the shader resource view.
	m_description.Format = m_renderTarget.m_texture.m_description.Format;


	// Create the shader resource view.
	result = device.m_pointer->CreateShaderResourceView(m_renderTarget.m_texture.m_data, &m_description, &m_renderTarget.m_texture.m_pointer);

	if (FAILED(result))
	{
		return false;
	}

#endif
#ifdef OPEN_GL
	glDeleteTextures(1, &m_renderTarget.m_texture.m_id);
	glDeleteFramebuffers(1, &m_renderTarget.m_id);

	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glGenTextures(1, &m_renderTarget.m_texture.m_id);
	glBindTexture(GL_TEXTURE_2D, m_renderTarget.m_texture.m_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.width, size.height, 0, GL_RGBA, GL_FLOAT, NULL);
	glGenerateMipmap(GL_TEXTURE_2D);

	glSamplerParameterf(m_renderTarget.m_texture.m_id, GL_TEXTURE_MIN_LOD, 0);
	glSamplerParameterf(m_renderTarget.m_texture.m_id, GL_TEXTURE_MAX_LOD, 10);
	glSamplerParameterf(m_renderTarget.m_texture.m_id, GL_TEXTURE_LOD_BIAS, 0.0);

	glGenFramebuffers(1, &m_renderTarget.m_id);
	glBindFramebuffer(GL_FRAMEBUFFER, m_renderTarget.m_id);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_renderTarget.m_texture.m_id, 0);

	GLenum DrawBuffers2[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers2);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		return false;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
#endif
	return true;
}

void CBlurHPass::Update()
{

}

void CBlurHPass::Erase()
{

}

CBlurHPass::CBlurHPass()
{
#ifdef DIRECT_X
	m_shaderProgram = nullptr;
	m_clearColor = nullptr;
#endif
}

CBlurHPass::~CBlurHPass()
{
#ifdef DIRECT_X
	if (m_shaderProgram)
	{
		delete m_shaderProgram;
	}
	m_shaderProgram = nullptr;
#endif
}
