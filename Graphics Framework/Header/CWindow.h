#pragma once
#include "Header.h"
#include "CShaderProgram.h"
#include "CSwapChain.h"
#include "CCameraManager.h"
#include "CScene.h"
#include "CModel.h"
#include "CMesh.h"

class CWindow
{
public:

	CCameraManager	m_camera;
	CSwapChain			m_swapChain;
	CScene					m_scene;
	Rect						m_size;
	Color						m_clearColor;

#ifdef OPEN_GL
	GLFWwindow*			m_pointer;

	void Initialize(CDevice& device, Rect dimensions, unsigned int displayMode, const char* name, Color color);
#endif

#ifdef DIRECT_X
	HINSTANCE				m_hInstance;
	HWND						m_handle;

	std::wstring		m_name;
	std::wstring		m_className;

	void Initialize(WNDPROC pWndProc, HINSTANCE hInstance, std::string title, std::string className, Size size, Color color, INT nCmdShow);
#endif

	CWindow();
	~CWindow();

	void Render(CDeviceContext& deviceContext, CLightingPass& pass, CCamera& camera);
	Size GetWindowSize();
	bool Resize();
};