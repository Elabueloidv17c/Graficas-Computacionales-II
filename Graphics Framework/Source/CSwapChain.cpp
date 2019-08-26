#include "../Header/CSwapChain.h"
#include "../Header/CDevice.h"

CSwapChain::CSwapChain()
{
#ifdef OPEN_GL
	m_id = 0;
#endif
}

CSwapChain::~CSwapChain()
{
#ifdef OPEN_GL
	glDeleteFramebuffers(1, &m_id);
	m_id = 0;
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

	m_renderTarget.Initialize(size);
	m_depthStencil.Initialize(size, true);
}
#endif

void CSwapChain::Resize(CDevice& device, Size size)
{
#ifdef DIRECT_X
	m_depthStencil.m_view->Release();
	m_depthStencil.m_texture.m_data->Release();
	
	m_renderTarget.m_view->Release();
	m_renderTarget.m_texture.m_data->Release();

	m_description.BufferDesc.Width = size.width;
	m_description.BufferDesc.Height = size.height;
	m_depthStencil.m_texture.m_description.Width = size.width;
	m_depthStencil.m_texture.m_description.Height = size.height;
	
	m_pointer->ResizeBuffers(0, size.width, size.height, DXGI_FORMAT_UNKNOWN, 0);

	m_pointer->GetBuffer(0, __uuidof(TextureData), (LPVOID*)&m_renderTarget.m_texture.m_data);
	device.CreateRenderTargetView(m_renderTarget, m_renderTarget.m_texture.m_data);

	device.CreateDepthStencilTexture(m_depthStencil);
	device.CreateDepthStencilView(m_depthStencil);
#endif
}

void CSwapChain::Present(unsigned int SyncInterval, unsigned int flags)
{
#ifdef DIRECT_X
	m_pointer->Present(SyncInterval, flags);
#endif
}

#ifdef OPEN_GL
bool CSwapChain::Initialize(Color color, Size size)
{
	return true;
}
#endif