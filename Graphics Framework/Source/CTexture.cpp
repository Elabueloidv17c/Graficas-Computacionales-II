#include "../Header/CTexture.h"


CTexture::CTexture() {
#ifdef OPEN_GL
	m_id = 0;
#endif
#ifdef DIRECT_X
	m_data = nullptr;
#endif
}

CTexture::~CTexture() {
#ifdef DIRECT_X
	if (m_data) {
		m_data->Release();
	}
#endif
}

void 
CTexture::Erase() {
#ifdef OPEN_GL
	//Release the handle
	if (m_id != 0) {
		glDeleteTextures(1, &m_id);
		m_id = 0;
	}
#endif
}

bool 
CTexture::Initialize(std::string path) {
#ifdef OPEN_GL
	std::cout << "Loading texture: " << path << std::endl;
	
	if (!LoadFromFile(path)) {
		std::cout << "Error:  texture not loaded correctly" << std::endl;
		return false;
	}
	
	glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_2D, m_id);
	
	// Set the texture color clamp to the edge.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	
	// Set the texture filtering.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	// Load the image data into the texture unit.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_data);
	
	// Generate mipmaps for the texture.
	glGenerateMipmap(GL_TEXTURE_2D);
	
	std::cout << "Texture loaded correctly" << std::endl;
	
	// Release software data
	SOIL_free_image_data(m_data);
	
	m_target = GL_TEXTURE_2D;
	
	//Unbind texture buffer
	glBindTexture(GL_TEXTURE_2D, 0);
#endif
	m_name = path;
	return true;
}


#ifdef DIRECT_X
void 
CTexture::Initialize(Size size) {
	m_data = nullptr;

	ZeroMemory(&m_description, sizeof(m_description));

	m_description.Width = size.width;
	m_description.Height = size.height;
	m_description.MipLevels = 1;
	m_description.ArraySize = 1;
	m_description.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	m_description.SampleDesc.Count = 1;
	m_description.SampleDesc.Quality = 0;
	m_description.Usage = D3D11_USAGE_DEFAULT;
	m_description.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	m_description.CPUAccessFlags = 0;
	m_description.MiscFlags = 0;
}
#endif

#ifdef OPEN_GL
bool 
CTexture::LoadFromFile(std::string path) {
	m_data = SOIL_load_image(&path[0], &m_width, &m_height, NULL, SOIL_LOAD_RGBA);
	
	if (!m_data) {
		std::cout << "Error:  Texture not loaded" << std::endl;
		return false;
	}
	return true;
}

void 
CTexture::Render(unsigned int shaderProgram, bool isDiffuse)
{
	if (m_id != 0) {
		unsigned int location;
	
		if (isDiffuse) {
			location = glGetUniformLocation(shaderProgram, "Texture");
		}
		else {
			location = glGetUniformLocation(shaderProgram, "NormalMap");
		}
	
		if (location != -1) {
			glUniform1i(location, m_id);
			glActiveTexture(GL_TEXTURE0 + m_id);
			glBindTexture(m_target, m_id);
	
			if (m_SamplerID != 0) {
				glBindSampler(m_id, m_SamplerID);
			}
			else 	{
				glBindSampler(m_id, 0);
			}
		}
	}
}

void 
CTexture::EndRender() {
	glActiveTexture(GL_TEXTURE0 + m_id);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
}

void 
CTexture::SetSampler() {
	glGenSamplers(1, &m_SamplerID);
	glSamplerParameteri(m_SamplerID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glSamplerParameteri(m_SamplerID, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glSamplerParameteri(m_SamplerID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(m_SamplerID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void 
CTexture::Initialize(TextureDescription data) {
	glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_2D, m_id);
	
	glTexImage2D(GL_TEXTURE_2D, 0, data.InternalFormat, data.BufferSize.size.width, data.BufferSize.size.height, 0, data.Format, data.Type, 0);
	
	m_width = data.BufferSize.size.width;
	m_height = data.BufferSize.size.height;
	
	m_SamplerID = data.SamplerID;
	m_target = GL_TEXTURE_2D;
	
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
}

unsigned int 
CTexture::GetID() {
	return m_id;
}

unsigned int 
CTexture::GetSampler() {
	return m_SamplerID;
}

unsigned char 
CTexture::GetTextureType() {
	return m_TextureTypeID;
}

unsigned int 
CTexture::GetTarget() {
	return m_target;
}
#endif