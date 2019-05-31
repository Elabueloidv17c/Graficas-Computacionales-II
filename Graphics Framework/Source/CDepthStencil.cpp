#include "../Header/CDepthStencil.h"

CDepthStencil::CDepthStencil()
{
#ifdef OPEN_GL
	m_id = 0;
#endif
}

CDepthStencil::~CDepthStencil()
{
#ifdef OPEN_GL
	Erase();
#endif

#ifdef DIRECT_X
	if (m_view)
	{
		m_view->Release();
	}
#endif
}

#ifdef OPEN_GL
void CDepthStencil::Initialize(Size size, bool stencil)
{
	glGenRenderbuffersEXT(1, &m_id);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_id);
	
	if (stencil)
	{
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH24_STENCIL8, size.width, size.height);
	}
	
	else
	{
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, size.width, size.height);
	}
	
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_id);
}

void CDepthStencil::Erase()
{
#ifdef OPEN_GL
	glDeleteRenderbuffers(1, &m_id);
	m_id = 0;
#endif

#ifdef DIRECT_X

#endif
}

unsigned int CDepthStencil::GetID()
{
	return m_id;
}
#endif

#ifdef DIRECT_X
void CDepthStencil::Initialize(Size size)
{
	m_texture.Initialize(size);

	ZeroMemory(&m_description, sizeof(m_description));

	m_description.Format = m_texture.m_description.Format;
	m_description.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	m_description.Texture2D.MipSlice = 0;

	ZeroMemory(&m_view, sizeof(m_view));
}
#endif