#include "../Header/CRenderPass.h"

CRenderPass::CRenderPass()
{
	m_clearColor = nullptr;
	m_shaderProgram = nullptr;
}

CRenderPass::~CRenderPass()
{

}

void CRenderPass::Erase()
{

}

void CRenderPass::Update()
{
}

void CRenderPass::Render(CDeviceContext& deviceContext)
{

}

void CRenderPass::Set(CDeviceContext& deviceContext)
{
}

bool CRenderPass::Resize(CDevice& device, Size size)
{
	return false;
}

void CRenderPass::Clear(CDeviceContext& deviceContext)
{

}
