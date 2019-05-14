#pragma once
#include "CViewportManager.h"
#include "CRenderToTexture.h"
#include "CUserInterface.h"
#include "CSamplerState.h"
#include "CShaderProgram.h"
#include "CDeviceContext.h"
#include "CInputManager.h"
#include "CWindow.h"
#include "CDevice.h"
#include "CModel.h"
#include "Header.h"

class CManager
{
public:

	float					m_time;

	CRenderToTexture		m_renderTexture;
	CUserInterface			m_userInterface;
	CViewportManager		m_viewport;
	CShaderProgram			m_shaderProgram;
	CInputManager			m_inputHandler;
	CSamplerState			m_sampler;
	CWindow					m_window;

	CDeviceContext			m_deviceContext;
	CDevice					m_device;

	MATRIX4					m_world;

#ifdef OPEN_GL
	void Initialize(Rect dimensions);
#endif

#ifdef DIRECT_X
	void Initialize(WNDPROC pWndProc, HINSTANCE hInstance, std::string title, std::string className, Size size, Color color, INT nCmdShow);
#endif
	CManager();
	~CManager() = default;

	void Update();
	void Resize();
	void Render();
};