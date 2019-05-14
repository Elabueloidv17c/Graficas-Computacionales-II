#include "../Header/CRenderToTexture.h"

CRenderToTexture::CRenderToTexture()
{
#ifdef DIRECT_X
	m_shaderResource = nullptr;
#endif

#ifdef OPEN_GL
	m_idFrameBuffer = 0;
	m_idTexture = 0;
#endif
}

CRenderToTexture::~CRenderToTexture()
{
	Erase();
}

void CRenderToTexture::Erase()
{

}

#ifdef OPEN_GL
bool CRenderToTexture::Initialize(Size size, Color color)
{
	if (m_idFrameBuffer == 0 && m_idDepth == 0 && m_idTexture == 0)
	{
		glGenFramebuffers(1, &m_idFrameBuffer);
		glGenRenderbuffers(1, &m_idDepth);
		glGenTextures(1, &m_idTexture);

		m_color = color;
	}

	glBindTexture(GL_TEXTURE_2D, m_idTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.width, size.height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size.width, size.height, 0, GL_RGBA, GL_FLOAT, 0);
	
	glBindRenderbuffer(GL_RENDERBUFFER, m_idDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, size.width, size.height);
	
	glBindFramebuffer(GL_FRAMEBUFFER, m_idFrameBuffer);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_idDepth);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_idTexture, 0);

	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers);


	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		return false;
	}

	else
	{
		return true;
	}
}

void CRenderToTexture::Bind(Size size)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_idFrameBuffer);
	glViewport(0, 0, size.width, size.height);
}

void CRenderToTexture::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CRenderToTexture::Clear()
{
	glClearColor(m_color.r, m_color.g, m_color.b, m_color.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

#endif

#ifdef DIRECT_X
bool CRenderToTexture::Initialize(CDevice& device, Size size, Color color)
{
	m_color = color;

	HRESULT result;

	// Initialize the render target texture description.
	ZeroMemory(&m_texture.m_description, sizeof(m_texture.m_description));

	// Setup the render target texture description.
	m_texture.m_description.Width = size.width;
	m_texture.m_description.Height = size.height;
	m_texture.m_description.MipLevels = 1;
	m_texture.m_description.ArraySize = 1;
	m_texture.m_description.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	m_texture.m_description.SampleDesc.Count = 1;
	m_texture.m_description.Usage = D3D11_USAGE_DEFAULT;
	m_texture.m_description.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	m_texture.m_description.CPUAccessFlags = 0;
	m_texture.m_description.MiscFlags = 0;

	// Create the render target texture.
	result = device.m_pointer->CreateTexture2D(&m_texture.m_description, NULL, &m_texture.m_data);

	if (FAILED(result))
	{
		return false;
	}

	// Setup the description of the render target view.
	m_renderTarget.m_description.Format = m_texture.m_description.Format;
	m_renderTarget.m_description.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	m_renderTarget.m_description.Texture2D.MipSlice = 0;

	// Create the render target view.
	result = device.m_pointer->CreateRenderTargetView(m_texture.m_data, &m_renderTarget.m_description, &m_renderTarget.m_view);

	if (FAILED(result))
	{
		return false;
	}

	// Setup the description of the shader resource view.
	m_description.Format = m_texture.m_description.Format;
	m_description.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	m_description.Texture2D.MostDetailedMip = 0;
	m_description.Texture2D.MipLevels = 1;

	// Create the shader resource view.
	result = device.m_pointer->CreateShaderResourceView(m_texture.m_data, &m_description, &m_shaderResource);
	
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void CRenderToTexture::SetRenderTarget(CDeviceContext& deviceContext)
{
	deviceContext.m_pointer->OMSetRenderTargets(1, &m_renderTarget.m_view, nullptr);
}

void CRenderToTexture::ClearRenderTarget(CDeviceContext& deviceContext)
{
	deviceContext.m_pointer->ClearRenderTargetView(m_renderTarget.m_view, &m_color.r);
}
#endif