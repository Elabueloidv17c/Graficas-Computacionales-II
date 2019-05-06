#pragma once
#include "Header.h"

class CTexture
{
public:
	TextureData					m_data;

#ifdef DIRECT_X
	D3D11_TEXTURE2D_DESC		m_description;
#endif

#ifdef OPEN_GL
	unsigned int				m_id;
	unsigned char				m_TextureTypeID;
	unsigned int				m_SamplerID;

	int							m_width;
	int							m_height;

	std::string					m_name;

	bool						LoadFromFile(std::string path);
#endif

public:

	CTexture();
	~CTexture();

#ifdef DIRECT_X
	void Initialize(Size size);
#endif

#ifdef OPEN_GL
	unsigned int			m_target;

	void Render(unsigned int shaderProgram);
	void EndRender();

	unsigned int		GetID();

	void				SetSampler();
	unsigned int		GetSampler();
	unsigned char		GetTextureType();
	unsigned int		GetTarget();
	bool				Initialize(std::string path);
	void				Erase();

	void				Initialize(TextureDescription data);
#endif
};