#pragma once
#include "CScreenAlignedQuad.h"
#include "CRasterizerState.h"
#include "CViewportManager.h"
#include "CLuminancePass.h"
#include "CAddBrightPass.h"
#include "CShaderManager.h"
#include "CUserInterface.h"
#include "CDeviceContext.h"
#include "CSamplerState.h"
#include "CLightingPass.h"
#include "CInputManager.h"
#include "CToneMapPass.h"
#include "CBrightPass.h"
#include "CBlurHPass.h"
#include "CBlurVPass.h"
#include "CVRManager.h"
#include "CWindow.h"
#include "CDevice.h"
#include "CModel.h"
#include "Header.h"

class CManager
{
public:

	float									m_time;
	float									m_lastFrame;

	CLightingPass					m_lightingPass;
	CLuminancePass				m_luminancePass;
	CBrightPass						m_brightPass;
	CBlurHPass						m_blurHPass;
	CBlurVPass						m_blurVPass;
	CAddBrightPass				m_addBrightPass;
	CToneMapPass					m_toneMapPass;

	CVRManager						m_vrManager;
	CUserInterface				m_userInterface;
	CViewportManager			m_viewport;

	CShaderManager				m_shaderManager;

	CInputManager					m_inputHandler;

	CWindow								m_window;
	CScreenAlignedQuad		m_screenAlignedQuad;

	CDeviceContext				m_deviceContext;
	CDevice								m_device;

	MATRIX4								m_world;


	void Initialize(WindowData data);

	CManager();
	~CManager() = default;

	void DrawScene();
	void Update();
	void Resize();
	void Render();
};