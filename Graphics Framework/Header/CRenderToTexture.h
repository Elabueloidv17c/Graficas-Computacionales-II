#pragma once
#include "Header.h"
#include "CTexture.h"
#include "CDevice.h"
#include "CRenderTarget.h"
#include "CDeviceContext.h"

class CRenderToTexture
{
public:

	Color							m_color;

#ifdef DIRECT_X
	CTexture						m_texture;
	CRenderTarget					m_renderTarget;

	ID3D11ShaderResourceView*		m_shaderResource;
	D3D11_SHADER_RESOURCE_VIEW_DESC m_description;

	bool Initialize(CDevice& device, Size size, Color color);

	void SetRenderTarget(CDeviceContext& deviceContext);
	void ClearRenderTarget(CDeviceContext& deviceContext);

#endif

#ifdef OPEN_GL
	unsigned int					m_idFrameBuffer;
	unsigned int					m_idTexture;
	unsigned int					m_idDepth;

	Size							m_size;

	bool Initialize(Size size, Color color);
	void Bind(Size size);
	void Unbind();
	void Clear();
#endif

	void Erase();

	CRenderToTexture();
	~CRenderToTexture();
};