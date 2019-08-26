#include "../Header/CRenderTarget.h"

void CRenderTarget::Initialize(Size size)
{
#ifdef DIRECT_X
	m_texture.Initialize(size);

	ZeroMemory(&m_description, sizeof(m_description));
	ZeroMemory(&m_view, sizeof(m_view));
#endif
#ifdef OPEN_GL
	m_id = 0;
#endif
}

CRenderTarget::CRenderTarget()
{
#ifdef DIRECT_X
	m_view = nullptr;
#endif
#ifdef OPEN_GL
	m_id = 0;
#endif
}

CRenderTarget::~CRenderTarget()
{
#ifdef DIRECT_X
	if (m_view)
	{
		m_view->Release();
	}
#endif
}