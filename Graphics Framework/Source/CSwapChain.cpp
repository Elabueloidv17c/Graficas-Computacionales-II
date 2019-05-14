#include "../Header/CSwapChain.h"


CSwapChain::CSwapChain()
{
#ifdef OPEN_GL
	m_id = 0;
#endif
}

CSwapChain::~CSwapChain()
{
#ifdef OPEN_GL
	Erase();
#endif

#ifdef DIRECT_X
	if (m_pointer)
	{
		m_pointer->Release();
	}
#endif
}

#ifdef DIRECT_X
void CSwapChain::Initialize(Color color, Size size, HWND hWindow)
{
	m_color = color;

	ZeroMemory(&m_description, sizeof(m_description));

	m_description.BufferDesc.Width = size.width;
	m_description.BufferDesc.Height = size.height;
	m_description.OutputWindow = hWindow;

	m_description.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_description.BufferCount = 1;
	m_description.BufferDesc.RefreshRate.Numerator = 60;
	m_description.BufferDesc.RefreshRate.Denominator = 1;
	m_description.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	m_description.SampleDesc.Count = 1;
	m_description.SampleDesc.Quality = 0;
	m_description.Windowed = TRUE;

	ZeroMemory(&m_pointer, sizeof(m_pointer));

	m_renderTarget = CRenderTarget();
	m_depthStencil.Initialize(size);
}

void CSwapChain::Resize(Size size)
{
	m_depthStencil.m_texture.m_data->Release();
	m_renderTarget.m_view->Release();
	m_depthStencil.m_view->Release();

	m_description.BufferDesc.Width = size.width;
	m_description.BufferDesc.Height = size.height;
	m_depthStencil.m_texture.m_description.Width = size.width;
	m_depthStencil.m_texture.m_description.Height = size.height;
	
	m_pointer->ResizeBuffers(0, size.width, size.height, DXGI_FORMAT_UNKNOWN, 0);
}

void CSwapChain::Present(UINT SyncInterval, UINT flags)
{
	m_pointer->Present(SyncInterval, flags);
}
#endif

#ifdef OPEN_GL
void CSwapChain::Erase()
{	
	m_renderTarget.Erase();
	m_depthStencil.Erase();

	glDeleteFramebuffers(1, &m_id);
	m_id = 0;
}

bool CSwapChain::Initialize(SwapChainData renderTargetData)
{
	glGenFramebuffers(1, &m_id);
	glBindFramebuffer(GL_FRAMEBUFFER, m_id);

	m_renderTarget.Initialize(renderTargetData);
	m_depthStencil.Initialize(renderTargetData.TextureDataInit.BufferSize.size, renderTargetData.stencilBuffer);

	int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Error:  SwapChain Creation Failed" << std::endl;
		return false;
	}

	m_data = renderTargetData;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

bool CSwapChain::AddRenderTarget(TextureDescription textureData)
{
	if (m_renderTarget.m_view.size() < 4)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_id);

		CTexture* NewTexture = new CTexture;
		NewTexture->Initialize(textureData);

		m_renderTarget.m_view.push_back(NewTexture);

		glFramebufferTexture2D(GL_FRAMEBUFFER, textureData.AtachmentType, GL_TEXTURE_2D, NewTexture->GetID(), 0);

		int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "Error:  Add Render Target Failed" << std::endl;
			return false;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
		return true;
	}

	else
	{
		std::cout << "Warning:  Render Target limit reached" << std::endl;
		return false;
	}
}

void CSwapChain::SetSwapChain(Rect* viewPort)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_id);

	if (viewPort)
	{
		glViewport(0, 0, (GLsizei)viewPort->size.width, (GLsizei)viewPort->size.height);
	}

	else
	{
		glViewport(0, 0, (GLsizei)m_data.TextureDataInit.BufferSize.size.width, (GLsizei)m_data.TextureDataInit.BufferSize.size.height);
	}

	GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0 , GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };

	glDrawBuffers(m_data.numTagets, DrawBuffers);
}

bool CSwapChain::Resize(Rect bufferSize)
{

	m_data.TextureDataInit.BufferSize = bufferSize;

	Erase();

	return Initialize(m_data);
}

unsigned int CSwapChain::GetID()
{
	return m_id;
}
#endif