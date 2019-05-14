#include "../Header/CViewportManager.h"


CViewportManager::CViewportManager()
{
#ifdef OPEN_GL
	m_activeViewPort = 0;
#endif
}

CViewportManager::~CViewportManager()
{
#ifdef OPEN_GL
	Erase();
#endif
}

void CViewportManager::Initialize(Rect dimensions)
{

#ifdef OPEN_GL
	if (m_vieports.size() == 0)
	{
		m_activeViewPort = 0;
		m_vieports.push_back(dimensions);
	}

	glViewport(dimensions.pos.x, dimensions.pos.y, dimensions.size.width, dimensions.size.height);
#endif

#ifdef DIRECT_X
	m_description.Width = (FLOAT)dimensions.size.width;
	m_description.Height = (FLOAT)dimensions.size.height;
	m_description.TopLeftX = dimensions.pos.x;
	m_description.TopLeftY = dimensions.pos.y;

	m_description.MinDepth = 0.0f;
	m_description.MaxDepth = 1.0f;
#endif
}

void CViewportManager::Resize(Rect dimensions)
{
#ifdef OPEN_GL
	glViewport(dimensions.pos.x, dimensions.pos.y, dimensions.size.width, dimensions.size.height);
	m_vieports[m_activeViewPort] = dimensions;
#endif

#ifdef DIRECT_X
	m_description.Width = (FLOAT)dimensions.size.width;
	m_description.Height = (FLOAT)dimensions.size.height;
	m_description.TopLeftX = dimensions.pos.x;
	m_description.TopLeftY = dimensions.pos.y;

	m_description.MinDepth = 0.0f;
	m_description.MaxDepth = 1.0f;
#endif
}

#ifdef OPEN_GL
void CViewportManager::Erase()
{
	m_vieports.clear();
	m_activeViewPort = 0;
}

void CViewportManager::Add(Rect viewPort)
{
	m_activeViewPort++;
	m_vieports.push_back(viewPort);
}

void CViewportManager::SetActive(unsigned short index, Rect dimensions)
{
	if (index >= m_vieports.size())
	{
		std::cout << "Warning:  Index out of range, can't change active viewport" << std::endl;
		return;
	}

	m_activeViewPort = index;
	Resize(dimensions);
}

unsigned int CViewportManager::GetActive()
{
	return m_activeViewPort;
}
#endif