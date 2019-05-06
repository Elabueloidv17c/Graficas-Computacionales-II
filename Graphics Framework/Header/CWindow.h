#pragma once
#include "Header.h"
#include "CShaderProgram.h"
#include "CSwapChain.h"
#include "CCameraManager.h"
#include "CScene.h"

class CWindow
{
public:

	CCameraManager		m_camera;
	CSwapChain			m_swapChain;
	CScene				m_scene;
	Rect				m_size;

#ifdef OPEN_GL
	unsigned int		m_id;
	Color				m_clearColor;

	void Initialize(Rect dimensions, unsigned int displayMode, const char* name, Color color, std::vector <ModelData>& scene);
	void Clear();
#endif

#ifdef DIRECT_X
	HINSTANCE			m_hInstance;
	HWND				m_handle;

	std::wstring		m_name;
	std::wstring		m_className;

	bool				m_isInitialized;

	void Initialize(WNDPROC pWndProc, HINSTANCE hInstance, std::string title, std::string className, Size size, Color color, INT nCmdShow);
#endif

	CWindow() = default;
	~CWindow() = default;

	void Render(CShaderProgram& shaderProgram, CCamera& camera, CCamera& otherCamera);
	bool Resize();

	unsigned int GetId();

	Color GetClearColor();
	void SetClearColor(Color color);
};