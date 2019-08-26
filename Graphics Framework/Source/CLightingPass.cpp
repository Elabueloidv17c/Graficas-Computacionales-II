#include "../Header/CLightingPass.h"

bool CLightingPass::Initialize(CDevice& device, Size size, Color* clearColor, CShaderProgram* shader, 
std::vector <CRenderTarget*> renderTargets, CDepthStencil* depth, CScene* scene)
{
	m_inTextures = renderTargets;
	m_shaderProgram = shader;
	m_clearColor = clearColor;
	m_depthStencil = depth;
	m_scene = scene;

	//Create the constant buffers to pass information to the shader
#ifdef DIRECT_X
	m_modelCB.Initialize(sizeof(MATRIX4));
	m_viewCB.Initialize(sizeof(MATRIX4));
	m_projectionCB.Initialize(sizeof(MATRIX4));
	m_colorDataCB.Initialize(sizeof(ColorData));
	m_lightingDataCB.Initialize(sizeof(ColorData));
#endif
#ifdef OPEN_GL
	m_worldCB.Initialize(&m_shaderProgram->m_id, "World");
	m_modelCB.Initialize(&m_shaderProgram->m_id, "Model");
	m_viewCB.Initialize(&m_shaderProgram->m_id, "View");
	m_projectionCB.Initialize(&m_shaderProgram->m_id, "Projection");

	std::vector <std::string> names;
	names.push_back("PointColor");
	names.push_back("SpotColor");
	names.push_back("DiffuseColor");
	names.push_back("SpecularColor");
	names.push_back("AmbientColor");
	names.push_back("DiffuseIntensity");
	names.push_back("SpecularIntensity");
	names.push_back("AmbientIntensity");
	m_colorDataCB.Initialize(&m_shaderProgram->m_id, names);

	names.clear();
	names.push_back("DirectionalLight");
	names.push_back("SpecularPower");
	names.push_back("PointPosition");
	names.push_back("PointRadius");
	names.push_back("ViewPosition");
	names.push_back("SpotRadius");
	names.push_back("SpotPosition");
	names.push_back("SpotAlpha");
	names.push_back("SpotLightDirection");
	names.push_back("SpotBeta");
	m_lightingDataCB.Initialize(&m_shaderProgram->m_id, names);
	
	names.clear();
	names.push_back("Texture");
	names.push_back("NormalMap");
	m_texturesCB.Initialize(&m_shaderProgram->m_id, names);
	device.CreateConstantBuffer(m_texturesCB);
#endif

	device.CreateConstantBuffer(m_modelCB);
	device.CreateConstantBuffer(m_viewCB);
	device.CreateConstantBuffer(m_projectionCB);
	device.CreateConstantBuffer(m_colorDataCB);
	device.CreateConstantBuffer(m_lightingDataCB);

	//----------------------------------------------------------------------------------------------------------
	//Set States description.
	//----------------------------------------------------------------------------------------------------------
	m_rasterizerState.Initialize();
	device.CreateRasterizerState(m_rasterizerState);

	m_samplerState.Initialize();
	device.CreateSamplerState(m_samplerState);

	m_depthState.Initialize();
	device.CreateDepthState(m_depthState);

#ifdef DIRECT_X
	//----------------------------------------------------------------------------------------------------------
	// Setup the render target texture description.
	//----------------------------------------------------------------------------------------------------------
	HRESULT result;

	ZeroMemory(&m_renderTarget.m_texture.m_description, sizeof(m_renderTarget.m_texture.m_description));

	m_renderTarget.m_texture.m_description.Width = size.width;
	m_renderTarget.m_texture.m_description.Height = size.height;
	m_renderTarget.m_texture.m_description.MipLevels = 10;
	m_renderTarget.m_texture.m_description.ArraySize = 1;
	m_renderTarget.m_texture.m_description.Format = DXGI_FORMAT_R16G16B16A16_FLOAT; //DXGI_FORMAT_R8G8B8A8_UNORM
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

	m_depthState.Initialize(true, GL_LESS);
	device.CreateDepthState(m_depthState);

	m_color = *clearColor;
	
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glGenTextures(1, &m_renderTarget.m_texture.m_id);
	glBindTexture(GL_TEXTURE_2D, m_renderTarget.m_texture.m_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size.width, size.height, 0, GL_RGBA, GL_FLOAT, 0);
	glGenerateMipmap(GL_TEXTURE_2D);

	glGenFramebuffers(1, &m_renderTarget.m_id);
	glGenRenderbuffers(1, &m_depthStencil->m_id);
	glBindRenderbuffer(GL_RENDERBUFFER, m_depthStencil->m_id);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, size.width, size.height);

	glBindFramebuffer(GL_FRAMEBUFFER, m_renderTarget.m_id);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthStencil->m_id);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_renderTarget.m_texture.m_id, 0);

	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		return false;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
#endif
	return true;
}

void 
CLightingPass::Render(CDeviceContext& deviceContext, CCamera* camera, bool isVR) {
	m_scene->Render(deviceContext, *this, *camera, isVR);
}

void 
CLightingPass::Set(CDeviceContext& deviceContext) {
	//Set Pipeline States
	deviceContext.SetSamplerState(m_samplerState);
	deviceContext.SetRasterizerState(m_rasterizerState);
	deviceContext.SetDepthState(m_depthState);

	//Generate mipmaps
	deviceContext.GenerateMipMaps(m_renderTarget.m_texture);

	//Set Shaders
	deviceContext.SetVertexShader(*m_shaderProgram);
	deviceContext.SetPixelShader(*m_shaderProgram);
	deviceContext.SetInputLayout(*m_shaderProgram);

	//Set The constant buffers
	deviceContext.SetVertexConstantBuffer(0, m_modelCB);
	deviceContext.SetVertexConstantBuffer(1, m_viewCB);
	deviceContext.SetVertexConstantBuffer(2, m_projectionCB);

	//Set the vertex and pixel shaders to the constant buffers
	deviceContext.SetVertexConstantBuffer(3, m_colorDataCB);
	deviceContext.SetPixelConstantBuffer(3, m_colorDataCB);
	deviceContext.SetVertexConstantBuffer(4, m_lightingDataCB);
	deviceContext.SetPixelConstantBuffer(4, m_lightingDataCB);
	deviceContext.UpdateLightingData(m_lightingDataCB, m_scene->m_lightingData);

	//Set Render Target
#ifdef DIRECT_X
	deviceContext.SetRenderTarget(m_renderTarget, m_depthStencil->m_view);
#endif
#ifdef OPEN_GL
	deviceContext.SetRenderTarget(m_renderTarget, m_depthStencil);
#endif
}

bool CLightingPass::Resize(CDevice& device, Size size)
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
	glDeleteFramebuffers(1, &m_renderTarget.m_id);
	glDeleteTextures(1, &m_renderTarget.m_texture.m_id);
	glDeleteRenderbuffers(1, &m_depthStencil->m_id);

	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glGenTextures(1, &m_renderTarget.m_texture.m_id);
	glBindTexture(GL_TEXTURE_2D, m_renderTarget.m_texture.m_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size.width, size.height, 0, GL_RGBA, GL_FLOAT, 0);
	glGenerateMipmap(GL_TEXTURE_2D);

	glSamplerParameterf(m_renderTarget.m_texture.m_id, GL_TEXTURE_MIN_LOD, 0);
	glSamplerParameterf(m_renderTarget.m_texture.m_id, GL_TEXTURE_MAX_LOD, 10);
	glSamplerParameterf(m_renderTarget.m_texture.m_id, GL_TEXTURE_LOD_BIAS, 0.0);

	glGenFramebuffers(1, &m_renderTarget.m_id);
	glGenRenderbuffers(1, &m_depthStencil->m_id);
	glBindRenderbuffer(GL_RENDERBUFFER, m_depthStencil->m_id);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, size.width, size.height);

	glBindFramebuffer(GL_FRAMEBUFFER, m_renderTarget.m_id);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthStencil->m_id);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_renderTarget.m_texture.m_id, 0);

	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers);

	glBindTexture(GL_TEXTURE_2D, 0);
#endif
	return true;
}

void CLightingPass::Clear(CDeviceContext& deviceContext)
{
	deviceContext.ClearColor(m_renderTarget, *m_clearColor);
	deviceContext.ClearDepthStencil(*m_depthStencil);
}

void CLightingPass::Update(CCamera* camera) {
	m_scene->m_lightingData.viewPosition.x =  camera->m_position.x;
	m_scene->m_lightingData.viewPosition.y =  camera->m_position.y;
	m_scene->m_lightingData.viewPosition.z =  camera->m_position.z;
	m_scene->m_lightingData.spotDirection.x = camera->m_front.x;
	m_scene->m_lightingData.spotDirection.y = camera->m_front.y;
	m_scene->m_lightingData.spotDirection.z = camera->m_front.z;
}

void CLightingPass::Erase()
{

}


void CLightingPass::SetShaderProgram(CShaderProgram* shader)
{
	m_shaderProgram = shader;
}

CLightingPass::CLightingPass()
{
	m_scene = nullptr;
	m_depthStencil = nullptr;
}

CLightingPass::~CLightingPass()
{
#ifdef OPEN_GL
	glDeleteFramebuffers(1, &m_renderTarget.m_id);
	glDeleteTextures(1, &m_renderTarget.m_texture.m_id);
#endif
}