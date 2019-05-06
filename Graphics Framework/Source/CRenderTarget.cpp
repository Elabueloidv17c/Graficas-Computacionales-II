#include "../Header/CRenderTarget.h"

CRenderTarget::CRenderTarget()
{
#ifdef DIRECT_X
	m_view = nullptr;
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

#ifdef OPEN_GL
void CRenderTarget::Initialize(SwapChainData renderTargetData)
{
	if (renderTargetData.numTagets > 4)
	{
		std::cout << "Can't initialize a Render target with that number of render textures, default set to 4" << std::endl;
		renderTargetData.numTagets = 4;
	}

	for (int i = 0; i < renderTargetData.numTagets; i++)
	{
		if (renderTargetData.targetID[i] == 0u)
		{
			CTexture* NewTexture = new CTexture;

			NewTexture->Initialize(renderTargetData.TextureDataInit);
			m_view.push_back(NewTexture);

			glFramebufferTexture2D(GL_FRAMEBUFFER, renderTargetData.TextureDataInit.AtachmentType + i, GL_TEXTURE_2D, NewTexture->GetID(), 0);
		}

		else
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, renderTargetData.TextureDataInit.AtachmentType + i, GL_TEXTURE_2D, renderTargetData.targetID[i], 0);
		}
	}
}

void CRenderTarget::Erase()
{
	for (unsigned int i = 0; i < m_view.size(); i++)
	{
		m_view[i]->Erase();
		delete m_view[i];
	}

	m_view.clear();
}
#endif