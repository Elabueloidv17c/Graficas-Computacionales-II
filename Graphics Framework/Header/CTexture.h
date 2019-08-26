#pragma once
#include "Header.h"

class CTexture
{
public:
	TextureData					m_data;
	std::string					m_name;

	int									m_width;
	int									m_height;

#ifdef DIRECT_X
	D3D11_TEXTURE2D_DESC		m_description;
	ID3D11ShaderResourceView*	m_pointer;
#endif
#ifdef OPEN_GL
	unsigned int				m_id;
	unsigned char				m_TextureTypeID;
	unsigned int				m_SamplerID;

	bool						LoadFromFile(std::string path);
#endif

public:

	CTexture();
	~CTexture();

	bool				Initialize(std::string path);
	void				Erase();

#ifdef DIRECT_X
	void Initialize(Size size);
#endif

#ifdef OPEN_GL
	unsigned int		m_target;

	void				Initialize(TextureDescription data);

	void				Render(unsigned int shaderProgram, bool isDiffuse);
	void				EndRender();

	unsigned int		GetID();

	void				SetSampler();
	unsigned int		GetSampler();
	unsigned char		GetTextureType();
	unsigned int		GetTarget();

#endif
};