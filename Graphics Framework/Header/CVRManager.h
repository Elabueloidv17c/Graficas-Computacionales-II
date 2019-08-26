#pragma once
#include "CLightingPass.h"
#include "CToneMapPass.h"
#include "CBrightPass.h"
#include "CBlurHPass.h"
#include "CLuminancePass.h"
#include "CAddBrightPass.h"
#include "CBlurVPass.h"
#include "CFlipPass.h"
#include "CDevice.h"
#include "Header.h"
#include "CDeviceContext.h"

class CVRManager {
public:

	CLightingPass							m_lightingPass;
	CLuminancePass						m_luminancePass;
	CBrightPass								m_brightPass;
	CBlurHPass								m_blurHPass;
	CBlurVPass								m_blurVPass;
	CAddBrightPass						m_addBrightPass;
	CToneMapPass							m_toneMapPass;

	vr::IVRSystem*						m_context;
	vr::IVRCompositor*				m_renderer;
	vr::IVRRenderModels*			m_renderModels;

	Rect											m_screenSize;

	vr::Texture_t							m_leftEyeTexture;
	CRenderTarget							m_leftEyeRender;
	CCamera										m_leftEye;

	vr::Texture_t							m_rightEyeTexture;
	CRenderTarget							m_rightEyeRender;
	CCamera										m_rightEye;

	MATRIX4*									m_headSetTransform;
	MATRIX4*									m_leftControllerTransform;
	MATRIX4*									m_rightControllerTransform;

	std::string								m_deviceNames[vr::k_unMaxTrackedDeviceCount];
	vr::TrackedDevicePose_t		m_devicePoses[vr::k_unMaxTrackedDeviceCount];

	void Render();


	void DrawScene(CDeviceContext& deviceContext, vr::EVREye eye);
	bool CreateRenderTarget(vr::EVREye eye, CDevice& device);
	void ProcessEvent(const vr::VREvent_t& event);
	void HandleEvents();

	static void SetProjectionMatrix(vr::HmdMatrix44_t origin, MATRIX4& destination);
	static void SetViewMatrix(vr::HmdMatrix34_t origin, MATRIX4& destination);
	static void SetMatrix(vr::HmdMatrix34_t origin, MATRIX4& destination);

	void Initialize(CDevice& device, Color* clearColor, CShaderProgram* shader, CDepthStencil* depth, 
	CScene* scene, CScreenAlignedQuad* SAQ);
	void CreateRenderTargets(CDevice& device, Color* clearColor, CShaderProgram* shader, CDepthStencil* depth, CScene* scene,
	CScreenAlignedQuad* SAQ);

	void GetDevicesTransform();
	void Update();

	bool CreateContext();
	bool CreateCompositor();
	
	bool CheckConnection();
	bool BindToRuntime();

	std::string GetTrackedDeviceTypeString(vr::ETrackedDeviceClass device);
	std::string GetTrackedDeviceName(vr::TrackedDeviceIndex_t unDevice, vr::TrackedDeviceProperty prop);

	CVRManager();
	~CVRManager();
};