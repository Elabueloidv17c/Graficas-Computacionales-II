#pragma once
#include "Header.h"

class CViewportManager
{
public:

#ifdef DIRECT_X
	D3D11_VIEWPORT m_description;
#endif

#ifdef OPEN_GL
	std::vector <Rect> m_vieports;
	unsigned short m_activeViewPort;

	void Erase();
	void Add(Rect viewPort);

	void SetActive(unsigned short index, Rect dimensions);
	unsigned int GetActive();
#endif

	void Initialize(Rect dimensions);
	void Resize(Rect dimensions);

	CViewportManager();
	~CViewportManager();
};