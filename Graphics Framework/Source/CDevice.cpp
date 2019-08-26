#pragma once
#include "../Header/CDevice.h"

CDevice::~CDevice()
{
#ifdef DIRECT_X
	if (m_pointer)
	{
		m_pointer->Release();
	}
#endif
}

long CDevice::Initialize(CDeviceContext& context, CSwapChain& swapchain)
{
	long result;

#ifdef DIRECT_X

	UINT createDeviceFlags = 0;

#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] = {
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};

	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};

	D3D_FEATURE_LEVEL featureLevel = featureLevels[0];

	for (UINT driverTypeIndex = 0; driverTypeIndex < ARRAYSIZE(driverTypes); driverTypeIndex++)
	{
		m_driverType = driverTypes[driverTypeIndex];

		result = D3D11CreateDeviceAndSwapChain(NULL, driverTypes[driverTypeIndex], NULL, createDeviceFlags, featureLevels, 
		ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &swapchain.m_description, &swapchain.m_pointer, &m_pointer, &featureLevel, 
		&context.m_pointer);

		if (SUCCEEDED(result))
		{
			swapchain.m_pointer->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)& swapchain.m_renderTarget.m_texture.m_data);
			CreateRenderTargetView(swapchain.m_renderTarget, swapchain.m_renderTarget.m_texture.m_data);

			CreateDepthStencilTexture(swapchain.m_depthStencil);
			CreateDepthStencilView(swapchain.m_depthStencil);

			break;
		}
	}
#endif
#ifdef OPEN_GL
	result = 1;
#endif
	return result;
}

void CDevice::CreateRenderTargetView(CRenderTarget& renderTarget, void* pBackBuffer)
{
#ifdef DIRECT_X
	m_pointer->CreateRenderTargetView((ID3D11Texture2D*)pBackBuffer, NULL, &renderTarget.m_view);
#endif
}

void CDevice::CreateDepthStencilTexture(CDepthStencil& depthStencil)
{
#ifdef DIRECT_X
	m_pointer->CreateTexture2D(&depthStencil.m_texture.m_description, NULL, &depthStencil.m_texture.m_data);
#endif
}

void CDevice::CreateDepthStencilView(CDepthStencil& depthStencil)
{
#ifdef DIRECT_X
	m_pointer->CreateDepthStencilView(depthStencil.m_texture.m_data, &depthStencil.m_description, &depthStencil.m_view);
#endif
}

void CDevice::CreateSamplerState(CSamplerState& state)
{
#ifdef DIRECT_X
	m_pointer->CreateSamplerState(&state.m_description, &state.m_pointer);
#endif
}

void CDevice::CreateDepthState(CDepthState& state)
{
#ifdef DIRECT_X
	m_pointer->CreateDepthStencilState(&state.m_description, &state.m_pointer);
#endif
}

void CDevice::CreateInputLayout(CShaderProgram& shader)
{
#ifdef DIRECT_X
	m_pointer->CreateInputLayout(&shader.m_vertexShader.m_layout[0], shader.m_vertexShader.m_layout.size(), 
	shader.m_vertexShader.m_blob->GetBufferPointer(), shader.m_vertexShader.m_blob->GetBufferSize(), 
	&shader.m_vertexShader.m_pLayout);
#endif
#ifdef OPEN_GL
	// Create a shader program object.
	shader.m_id = glCreateProgram();

	// Attach the vertex and fragment shader to the program object.
	glAttachShader(shader.m_id, shader.m_vertexShader.m_id);
	glAttachShader(shader.m_id, shader.m_pixelShader.m_id);


	// Link the shader program.
	glLinkProgram(shader.m_id);

	// Check the status of the link.
	int status;
	glGetProgramiv(shader.m_id, GL_LINK_STATUS, &status);

	if (status != 1)
	{
		std::cout << "Error: Shader program link not correct" << std::endl;
	}

	const int nameSize = 50;
	unsigned int type, index;
	int  size, count;
	char name[nameSize];

	glGetProgramiv(shader.m_id, GL_ACTIVE_ATTRIBUTES, &count);
	shader.m_vertexShader.m_layout.resize(count);

	for (unsigned int i = 0; i < count; i++)
	{
		glGetActiveAttrib(shader.m_id, i, nameSize, nullptr, &size, &type, &name[0]);
		index = glGetAttribLocation(shader.m_id, &name[0]);
		shader.m_vertexShader.m_layout[index].Name = name;
		glBindAttribLocation(shader.m_id, index, &shader.m_vertexShader.m_layout[i].Name[0]);
	}
#endif
}

void CDevice::CreateVertexShader(CShaderProgram& shader)
{
#ifdef DIRECT_X
	m_pointer->CreateVertexShader(shader.m_vertexShader.m_blob->GetBufferPointer(), 
	shader.m_vertexShader.m_blob->GetBufferSize(),NULL, &shader.m_vertexShader.m_pointer);
#endif
#ifdef OPEN_GL
	shader.m_vertexShader.m_id = glCreateShader(shader.m_vertexShader.m_type);
	glShaderSource(shader.m_vertexShader.m_id, 1, &shader.m_vertexShader.m_code, NULL);
	glCompileShader(shader.m_vertexShader.m_id);

	int status = 0;
	glGetShaderiv(shader.m_vertexShader.m_id, GL_COMPILE_STATUS, &status);

	if (status != 1)
	{
		std::cout << "VertexShader failed to compile";
	}
#endif
}

void CDevice::CreatePixelShader(CShaderProgram& shader)
{
#ifdef DIRECT_X
	m_pointer->CreatePixelShader(shader.m_pixelShader.m_blob->GetBufferPointer(), shader.m_pixelShader.m_blob->GetBufferSize(),
	NULL, &shader.m_pixelShader.m_pointer);
#endif
#ifdef OPEN_GL
	shader.m_pixelShader.m_id = glCreateShader(shader.m_pixelShader.m_type);
	glShaderSource(shader.m_pixelShader.m_id, 1, &shader.m_pixelShader.m_code, NULL);
	glCompileShader(shader.m_pixelShader.m_id);

	int status = 0;
	glGetShaderiv(shader.m_pixelShader.m_id, GL_COMPILE_STATUS, &status);

	if (status != 1)
	{
		std::cout << "PixelShader failed to compile";
	}
#endif
}

void CDevice::CreateConstantBuffer(CConstantBuffer& buffer)
{
#ifdef DIRECT_X
	m_pointer->CreateBuffer(&buffer.m_description, NULL, &buffer.m_Buffer);
#endif
#ifdef OPEN_GL

	if (!buffer.m_uniforms.size())
	{
		const int nameSize = 50;
		unsigned int type, index;
		int  size, count;
		char name[nameSize];

		glGetProgramiv(*buffer.m_shader, GL_ACTIVE_UNIFORMS, &count);
		buffer.m_uniforms.resize(count);

		for (int i = 0; i < count; i++)
		{
			glGetActiveUniform(*buffer.m_shader, i, nameSize, nullptr, &size, &type, &name[0]);
			index = glGetUniformLocation(*buffer.m_shader, &name[0]);
			buffer.m_uniforms[index].name = name;
			buffer.m_uniforms[index].location = index;
		}

		return;
	}

	for (int i = 0; i < buffer.m_uniforms.size(); i++)
	{
		buffer.m_uniforms[i].location = glGetUniformLocation(*buffer.m_shader, &buffer.m_uniforms[i].name[0]);
	}
#endif
}

void CDevice::CreateTexture(CMesh& mesh)
{
#ifdef DIRECT_X
	D3DX11CreateShaderResourceViewFromFile(m_pointer, &mesh.m_material.m_diffuse.m_name[0],
	NULL, NULL, &mesh.m_material.m_diffuse.m_pointer, NULL);

	D3DX11CreateShaderResourceViewFromFile(m_pointer, &mesh.m_material.m_normal.m_name[0], 
	NULL, NULL, &mesh.m_material.m_normal.m_pointer, NULL);
#endif
}

void CDevice::CreateRasterizerState(CRasterizerState& state) 
{
#ifdef DIRECT_X
	m_pointer->CreateRasterizerState(&state.m_description, &state.m_pointer);
#endif
}

void CDevice::CreateBuffers(CMesh& mesh)
{
#ifdef DIRECT_X
	m_pointer->CreateBuffer(&mesh.m_vertexBuffer.m_description, &mesh.m_vertexBuffer.m_InitData, &mesh.m_vertexBuffer.m_pointer);
	m_pointer->CreateBuffer(&mesh.m_indexBuffer.m_description, &mesh.m_indexBuffer.m_InitData, &mesh.m_indexBuffer.m_pointer);
#endif
}

void CDevice::CreateBuffers(CScreenAlignedQuad& quad)
{
#ifdef DIRECT_X
	m_pointer->CreateBuffer(&quad.m_vertexBuffer.m_description, &quad.m_vertexBuffer.m_InitData, &quad.m_vertexBuffer.m_pointer);
	m_pointer->CreateBuffer(&quad.m_indexBuffer.m_description, &quad.m_indexBuffer.m_InitData, &quad.m_indexBuffer.m_pointer);
#endif
}