#pragma once
#include "CViewportManager.h"
#include "CRenderToTexture.h"
#include "CShaderManager.h"
#include "CUserInterface.h"
#include "CDeviceContext.h"
#include "CSamplerState.h"
#include "CInputManager.h"
#include "CWindow.h"
#include "CDevice.h"
#include "CModel.h"
#include "Header.h"

class CManager
{
public:

	float					m_time;
	float					m_lastFrame;

	CRenderToTexture		m_renderTexture;
	CUserInterface			m_userInterface;
	CViewportManager		m_viewport;

	CShaderManager			m_shaderManager;

	CInputManager			m_inputHandler;
	CSamplerState			m_sampler;
	CWindow					m_window;

	CDeviceContext			m_deviceContext;
	CDevice					m_device;

	MATRIX4					m_world;


	void Initialize(WindowData data);

	CManager();
	~CManager() = default;

	void Update();
	void Resize();
	void Render();
};