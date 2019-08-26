#include "../Header/CManager.h"
#include "../Header/CTexture.h"

CManager::CManager() {
	m_time = 0;
#ifdef OPEN_GL
	m_lastFrame = 0;
#endif
}

void 
CManager::Initialize(WindowData data) {
	m_world = glm::mat4(1.0f);

	//----------------------------------------------------------------------------------------------------
	//Create Window
	//----------------------------------------------------------------------------------------------------
#ifdef DIRECT_X
	m_window.Initialize(data.pWndProc, data.hInstance, data.title, data.className, data.dimensions.size, data.color, data.nCmdShow);
	m_window.m_size.size = m_window.GetWindowSize();
	m_window.m_swapChain.Initialize(data.color, m_window.m_size.size, m_window.m_handle);
#endif
#ifdef OPEN_GL
	m_window.Initialize(m_device, data.dimensions, 0, &data.title[0], data.color);
#endif

	//----------------------------------------------------------------------------------------------------
	//Create DeviceContext and SwapChain
	//----------------------------------------------------------------------------------------------------
	m_device.Initialize(m_deviceContext, m_window.m_swapChain);

	//----------------------------------------------------------------------------------------------------
	//Create Viewports
	//----------------------------------------------------------------------------------------------------
	m_viewport.Initialize(m_window.m_size);
	m_deviceContext.SetViewPort(m_viewport);

	//----------------------------------------------------------------------------------------------------
	//Camera
	//----------------------------------------------------------------------------------------------------
	m_window.m_camera.Initialize(VECTOR3(0.0f, 3.0f, -10.0f), VECTOR3(0.0f, 1.0f, 0.0f), //Main Camera
	VECTOR3(0.0f, 0.0f, 1.0f), VECTOR3(1.0f, 0.0f, 0.0f), 90.0f, 0.0f, 0.03f, 0.20f);

	m_window.m_camera.AddCamera(VECTOR3(0.0f, 15.0f, 0.0f), VECTOR3(0.0f, 0.0f, 1.0f), //Secundary Camera
	VECTOR3(0.0f, -1.0f, 0.0f), VECTOR3(1.0f, 0.0f, 0.0f), -90.0f, 0.0f, 0.03f, 0.25f);

	//Set The view matrix´s initial position
	m_window.m_camera.SetViewMatrix((60.0f / 360.0f) * 6.283185307f, m_window.m_size.size, 0.01f, 1000.0f);

	//----------------------------------------------------------------------------------------------------
	//Set triangle based render
	//----------------------------------------------------------------------------------------------------
#ifdef DIRECT_X
	m_deviceContext.SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
#endif
#ifdef OPEN_GL
	m_deviceContext.SetPrimitiveTopology(GL_FILL);
#endif

	//---------------------------------------------------------------------------------------------------
	//Shaders
	//---------------------------------------------------------------------------------------------------
	std::vector <ShaderProgramData> shaders(4);
#ifdef DIRECT_X
	shaders[0] = ShaderProgramData{ ShaderData{ "../Header/Shader/VertexShader.hlsl", "VS", "vs_5_0", false, false },
	ShaderData{ "../Header/Shader/PixelShader.hlsl", "PS", "ps_5_0", false, false } };
	shaders[1] = ShaderProgramData{ ShaderData{ "../Header/Shader/VertexShader.hlsl", "VS", "vs_5_0", false, true },
	ShaderData{ "../Header/Shader/PixelShader.hlsl", "PS", "ps_5_0", false, true } };
	shaders[2] = ShaderProgramData{ ShaderData{ "../Header/Shader/VertexShader.hlsl", "VS", "vs_5_0", true, false },
	ShaderData{ "../Header/Shader/PixelShader.hlsl", "PS", "ps_5_0", true, false } };
	shaders[3] = ShaderProgramData{ ShaderData{ "../Header/Shader/VertexShader.hlsl", "VS", "vs_5_0", true, true },
	ShaderData{ "../Header/Shader/PixelShader.hlsl", "PS", "ps_5_0", true, true } };
#endif
#ifdef OPEN_GL
	shaders[0] = ShaderProgramData{ ShaderData{ "../Header/Shader/VertexShader.glsl", GL_VERTEX_SHADER, false, false },
	ShaderData{ "../Header/Shader/PixelShader.glsl", GL_FRAGMENT_SHADER, false, false } };
	shaders[1] = ShaderProgramData{ ShaderData{ "../Header/Shader/VertexShader.glsl", GL_VERTEX_SHADER, false, true },
	ShaderData{ "../Header/Shader/PixelShader.glsl", GL_FRAGMENT_SHADER, false, true } };
	shaders[2] = ShaderProgramData{ ShaderData{ "../Header/Shader/VertexShader.glsl", GL_VERTEX_SHADER, true, false },
	ShaderData{ "../Header/Shader/PixelShader.glsl", GL_FRAGMENT_SHADER, true, false } };
	shaders[3] = ShaderProgramData{ ShaderData{ "../Header/Shader/VertexShader.glsl", GL_VERTEX_SHADER, true, true },
	ShaderData{ "../Header/Shader/PixelShader.glsl", GL_FRAGMENT_SHADER, true, true } };
#endif

	//Initialize and create all shaders
	m_shaderManager.Initialize(shaders);

	for (int i = 0; i < shaders.size(); i++) {
		//Create the vertex shader
		m_device.CreateVertexShader(*m_shaderManager.GetShaderAt(i));
		m_device.CreatePixelShader(*m_shaderManager.GetShaderAt(i));
		m_device.CreateInputLayout(*m_shaderManager.GetShaderAt(i));
	}

	//---------------------------------------------------------------------------------------------------
	//Passes
	//---------------------------------------------------------------------------------------------------
	{
		//Initialize light pass with active properties
		std::vector<CRenderTarget*> textures;
		if (!m_lightingPass.Initialize(m_device, m_window.m_size.size, &m_window.m_swapChain.m_color, m_shaderManager.GetActiveShader(),
			textures, &m_window.m_swapChain.m_depthStencil, &m_window.m_scene)) {
			return;
		}

		//Initialize SAQ to render on it with post process passes
		if (!m_screenAlignedQuad.Initialize("../Models/Quad/SAQ.obj", m_device)) {
			return;
		}

		//Set output texture from lightpass has shader resource for luminance
		textures.push_back(&m_lightingPass.m_renderTarget);

		//Luminance Pass
#ifdef DIRECT_X
		if (!m_luminancePass.Initialize(m_device, m_window.m_size.size, &m_window.m_swapChain.m_color, &m_screenAlignedQuad,
			textures, ShaderProgramData{ ShaderData{ "../Header/Shader/VertexSAQ.hlsl", "VS", "vs_5_0", false, false },
			ShaderData{ "../Header/Shader/LuminancePass.hlsl", "PS", "ps_5_0", false, false } })) {
			return;
		}
#endif
#ifdef OPEN_GL
		if (!m_luminancePass.Initialize(m_device, m_window.m_size.size, &m_window.m_swapChain.m_color, &m_screenAlignedQuad,
			textures, ShaderProgramData{ ShaderData{ "../Header/Shader/VertexSAQ.glsl", GL_VERTEX_SHADER, false, false },
			ShaderData{ "../Header/Shader/LuminancePass.glsl", GL_FRAGMENT_SHADER, false, false } })) {
			return;
		}
#endif

		//Set output texture from luminance has shader resource for bright alongside light txture
		textures.push_back(&m_luminancePass.m_renderTarget);

		//Bright pass
#ifdef DIRECT_X
		if (!m_brightPass.Initialize(m_device, m_window.m_size.size, &m_window.m_swapChain.m_color, &m_screenAlignedQuad,
			textures, ShaderProgramData{ ShaderData{ "../Header/Shader/VertexSAQ.hlsl", "VS", "vs_5_0", false, false },
			ShaderData{ "../Header/Shader/BrightPass.hlsl", "PS", "ps_5_0", false, false } })) {
			return;
		}
#endif
#ifdef OPEN_GL
		if (!m_brightPass.Initialize(m_device, m_window.m_size.size, &m_window.m_swapChain.m_color, &m_screenAlignedQuad,
			textures, ShaderProgramData{ ShaderData{ "../Header/Shader/VertexSAQ.glsl", GL_VERTEX_SHADER, false, false },
			ShaderData{ "../Header/Shader/BrightPass.glsl", GL_FRAGMENT_SHADER, false, false } }))
		{
			return;
		}
#endif
		m_screenAlignedQuad.SetBrightData(BrightData{ 0, 0.1f });

		//Set output texture from luminance has shader resource for bright alongside light txture
		textures.clear();  textures.push_back(&m_brightPass.m_renderTarget);

		//Vertical Blur pass
#ifdef DIRECT_X
		if (!m_blurVPass.Initialize(m_device, m_window.m_size.size, &m_window.m_swapChain.m_color, &m_screenAlignedQuad,
			textures, ShaderProgramData{ ShaderData{ "../Header/Shader/VertexSAQ.hlsl", "VS", "vs_5_0", false, false },
			ShaderData{ "../Header/Shader/BlurVPass.hlsl", "PS", "ps_5_0", false, false } }))
		{
			return;
		}
#endif
#ifdef OPEN_GL
		if (!m_blurVPass.Initialize(m_device, m_window.m_size.size, &m_window.m_swapChain.m_color, &m_screenAlignedQuad,
			textures, ShaderProgramData{ ShaderData{ "../Header/Shader/VertexSAQ.glsl", GL_VERTEX_SHADER, false, false },
			ShaderData{ "../Header/Shader/BlurVPass.glsl", GL_FRAGMENT_SHADER, false, false } }))
		{
			return;
		}
#endif
		m_screenAlignedQuad.SetBlurVData(BlurVData{ m_window.GetWindowSize(), 1 });

		//Horizontal Blur pass
#ifdef DIRECT_X
		if (!m_blurHPass.Initialize(m_device, m_window.m_size.size, &m_window.m_swapChain.m_color, &m_screenAlignedQuad,
			textures, ShaderProgramData{ ShaderData{ "../Header/Shader/VertexSAQ.hlsl", "VS", "vs_5_0", false, false },
			ShaderData{ "../Header/Shader/BlurHPass.hlsl", "PS", "ps_5_0", false, false } }))
		{
			return;
		}
#endif
#ifdef OPEN_GL
		if (!m_blurHPass.Initialize(m_device, m_window.m_size.size, &m_window.m_swapChain.m_color, &m_screenAlignedQuad,
			textures, ShaderProgramData{ ShaderData{ "../Header/Shader/VertexSAQ.glsl", GL_VERTEX_SHADER, false, false },
			ShaderData{ "../Header/Shader/BlurHPass.glsl", GL_FRAGMENT_SHADER, false, false } }))
		{
			return;
		}
#endif
		m_screenAlignedQuad.SetBlurHData(BlurHData{ m_window.GetWindowSize(), 1 });

		//Add Bright pass
		textures.clear();
		textures.push_back(&m_brightPass.m_renderTarget);
		textures.push_back(&m_blurHPass.m_renderTarget);
		textures.push_back(&m_blurVPass.m_renderTarget);

#ifdef DIRECT_X
		if (!m_addBrightPass.Initialize(m_device, m_window.m_size.size, &m_window.m_swapChain.m_color, &m_screenAlignedQuad,
			textures, ShaderProgramData{ ShaderData{ "../Header/Shader/VertexSAQ.hlsl", "VS", "vs_5_0", false, false },
			ShaderData{ "../Header/Shader/AddBrightPass.hlsl", "PS", "ps_5_0", false, false } }))
		{
			return;
		}
#endif
#ifdef OPEN_GL
		if (!m_addBrightPass.Initialize(m_device, m_window.m_size.size, &m_window.m_swapChain.m_color, &m_screenAlignedQuad,
			textures, ShaderProgramData{ ShaderData{ "../Header/Shader/VertexSAQ.glsl", GL_VERTEX_SHADER, false, false },
			ShaderData{ "../Header/Shader/AddBrightPass.glsl", GL_FRAGMENT_SHADER, false, false } }))
		{
			return;
		}
#endif
		m_screenAlignedQuad.SetAddBrightData(AddBrightData{ 0 });

		//Add Bright pass
		textures.clear();
		textures.push_back(&m_lightingPass.m_renderTarget);
		textures.push_back(&m_addBrightPass.m_renderTarget);

#ifdef DIRECT_X
		if (!m_toneMapPass.Initialize(&m_window.m_swapChain, m_device, m_window.m_size.size, &m_window.m_swapChain.m_color, &m_screenAlignedQuad,
			textures, ShaderProgramData{ ShaderData{ "../Header/Shader/VertexSAQ.hlsl", "VS", "vs_5_0", false, false },
			ShaderData{ "../Header/Shader/ToneMapPass.hlsl", "PS", "ps_5_0", false, false } }))
		{
			return;
		}
#endif
#ifdef OPEN_GL
		if (!m_toneMapPass.Initialize(&m_window.m_swapChain, m_device, m_window.m_size.size, &m_window.m_swapChain.m_color, &m_screenAlignedQuad,
			textures, ShaderProgramData{ ShaderData{ "../Header/Shader/VertexSAQ.glsl", GL_VERTEX_SHADER, false, false },
			ShaderData{ "../Header/Shader/ToneMapPass.glsl", GL_FRAGMENT_SHADER, false, false } }))
		{
			return;
		}
#endif
		m_screenAlignedQuad.SetToneMapData(ToneMapData{ 0.3f, 0.0f });
	}

	//----------------------------------------------------------------------------------------------------
	//Initialize VR
	//----------------------------------------------------------------------------------------------------
#ifdef STEAM_VR
	m_vrManager.Initialize(m_device, &m_window.m_swapChain.m_color, m_shaderManager.GetActiveShader(),
	&m_window.m_swapChain.m_depthStencil, &m_window.m_scene, &m_screenAlignedQuad);
#endif

	//----------------------------------------------------------------------------------------------------
	//Scene
	//----------------------------------------------------------------------------------------------------

	//Specify the models information
	std::vector <ModelData> scene;
	scene.push_back(ModelData{ "../Models/Scene/Scene.fbx",
	new MATRIX4(glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), VECTOR3(0.0f, 1.0f, 0.0f))), false });

#ifdef STEAM_VR
	bool isLeftController = true;

	for (size_t i = 0; i < m_vrManager.m_deviceNames->size(); i++) {
		if (m_vrManager.m_deviceNames[i] == "headset") {
			scene.push_back(ModelData{ "../Models/VR/Headset.fbx", m_vrManager.m_headSetTransform, true });
		}
		if (m_vrManager.m_deviceNames[i] == "controller" && isLeftController) {
			scene.push_back(ModelData{ "../Models/VR/Controller.obj", m_vrManager.m_leftControllerTransform, false });
			isLeftController = false;
		}
		if (m_vrManager.m_deviceNames[i] == "controller" && !isLeftController) {
			scene.push_back(ModelData{ "../Models/VR/Controller.obj", m_vrManager.m_rightControllerTransform, false });
		}
	}
#endif

	//Load models from file
	m_window.m_scene.Initialize(scene, m_device);

	//----------------------------------------------------------------------------------------------------
	//Load lighting and color data
	//----------------------------------------------------------------------------------------------------

	m_window.m_scene.m_pointRadius = 2.0f;
	m_window.m_scene.m_spotRadius = 3.0f;
	m_window.m_scene.SetLightData(LightingData{
		Vector{0.0f, 0.0f, 0.0f},			//directional;
		1.0f,													//specularPower;

		Vector{0.0f, 0.3f, -3.8f},		//pointPosition;
		m_window.m_scene.m_pointRadius,

		Vector{0.0f, 0.0f, -10.0f},		//viewPosition;
		m_window.m_scene.m_spotRadius,

		Vector{10.0f, 5.0f, 2.0f},		//spotPosition;
		0.04f,												//spotAlpha;

		Vector{0.0f, 0.0f, 1.0f},			//spotDirection;
		0.5f,													//spotBeta;
		});
	m_window.m_scene.SetColorData(ColorData{
		Color{1.0f, 1.0f, 0.0f, 1.0f},	//Point Light

#ifdef DIRECT_X
		Color{0.0f, 1.0f, 0.0f, 1.0f},	//Spot Light
#endif
#ifdef OPEN_GL
		Color{0.0f, 0.0f, 1.0f, 1.0f},	//Spot Light
#endif
		Color{1.0f, 1.0f, 1.0f, 1.0f},	//Diffuse color
		Color{1.0f, 1.0f, 1.0f, 1.0f},	//Specular Color

#ifdef DIRECT_X
		Color{0.0f, 1.0f, 0.0f, 1.0f},	//Ambient Color
#endif
#ifdef OPEN_GL
		Color{0.0f, 0.0f, 1.0f, 1.0f},	//Ambient Color
#endif
		1.0f,
		1.0f,
		0.2f
		});

	//----------------------------------------------------------------------------------------------------
	//User Interface
	//----------------------------------------------------------------------------------------------------
#ifdef DIRECT_X
	m_userInterface.Initialize(&m_window.m_scene, &m_screenAlignedQuad);
	ImGui_ImplWin32_Init(m_window.m_handle);//ImGui Library Initialization
	ImGui_ImplDX11_Init(m_device.m_pointer, m_deviceContext.m_pointer);
#endif
#ifdef OPEN_GL
	//Initialize the scene information user interface
	m_userInterface.Initialize(&m_window.m_scene, &m_screenAlignedQuad);
	ImGui_ImplGlfw_InitForOpenGL(m_window.m_pointer, true);
	ImGui_ImplOpenGL3_Init("#version 400 core");
#endif
}

void 
CManager::Render() {

	DrawScene();

	//-----------------------------------------------------------------------------------------------------------------------------------
	//User interface
	//-----------------------------------------------------------------------------------------------------------------------------------
	m_userInterface.Initframe();
	m_userInterface.SetFrame(m_shaderManager.m_isVertex, m_shaderManager.m_isBlinn, m_window.m_scene.m_isSpotOn, 
	m_window.m_scene.m_isSpotChanged, m_window.m_scene.m_isPointOn, m_window.m_scene.m_isPointChanged);
	
#ifdef STEAM_VR
	if (m_vrManager.m_context) {
#ifdef DIRECT_X
		m_userInterface.SetHeadsetEye(m_vrManager.m_leftEyeRender.m_texture);
#endif
#ifdef OPEN_GL
		m_userInterface.SetHeadsetEye(m_vrManager.m_leftEyeRender.m_texture);
		//m_userInterface.SetHeadsetEye(m_vrManager.m_flip.m_texture);
#endif
	}
#endif

	m_userInterface.RenderFrame();
	
	//Present Frame
#ifdef DIRECT_X
	m_window.m_swapChain.Present(0, 0);
#endif
#ifdef OPEN_GL
	glfwSwapBuffers(m_window.m_pointer);
#endif

	//-----------------------------------------------------------------------------------------------------------------------------------
	//VR
	//-----------------------------------------------------------------------------------------------------------------------------------
#ifdef STEAM_VR
	if (m_vrManager.m_context) {
		m_window.m_swapChain.Resize(m_device, m_vrManager.m_screenSize.size);
		m_viewport.Resize(m_vrManager.m_screenSize);
		m_deviceContext.SetViewPort(m_viewport);
		m_screenAlignedQuad.m_blurHData.ViewportDimensions = m_vrManager.m_screenSize.size;
		m_screenAlignedQuad.m_blurVData.ViewportDimensions = m_vrManager.m_screenSize.size;

		m_vrManager.DrawScene(m_deviceContext, vr::EVREye::Eye_Left);
		m_vrManager.DrawScene(m_deviceContext, vr::EVREye::Eye_Right);
	}

	m_vrManager.Render();

	m_window.m_swapChain.Resize(m_device, m_window.GetWindowSize());
	m_viewport.Resize(m_window.m_size);
	m_deviceContext.SetViewPort(m_viewport);
	m_screenAlignedQuad.m_blurHData.ViewportDimensions = m_window.GetWindowSize();
	m_screenAlignedQuad.m_blurVData.ViewportDimensions = m_window.GetWindowSize();
#endif
}

void 
CManager::DrawScene() {
	//-----------------------------------------------------------------------------------------------------------------------------------
	//Light Pass
	//-----------------------------------------------------------------------------------------------------------------------------------
	m_deviceContext.UnSetRenderTarget();
	m_lightingPass.Set(m_deviceContext);
	m_lightingPass.Clear(m_deviceContext);
	m_lightingPass.Render(m_deviceContext, m_window.m_camera.GetActiveCamera(), false);
	m_deviceContext.UnSetRenderTarget();

	//-----------------------------------------------------------------------------------------------------------------------------------
	//Luminance Pass
	//-----------------------------------------------------------------------------------------------------------------------------------
	m_luminancePass.Set(m_deviceContext);
	m_luminancePass.Clear(m_deviceContext);
	m_luminancePass.Render(m_deviceContext);
	m_deviceContext.UnSetRenderTarget();

	//-----------------------------------------------------------------------------------------------------------------------------------
	//Bright Pass
	//-----------------------------------------------------------------------------------------------------------------------------------
	m_brightPass.Set(m_deviceContext);
	m_brightPass.Clear(m_deviceContext);
	m_brightPass.Render(m_deviceContext);
	m_deviceContext.UnSetRenderTarget();

	//-----------------------------------------------------------------------------------------------------------------------------------
	//Vertical Blur Pass
	//-----------------------------------------------------------------------------------------------------------------------------------
	m_blurVPass.Set(m_deviceContext);
	m_blurVPass.Clear(m_deviceContext);
	m_blurVPass.Render(m_deviceContext);
	m_deviceContext.UnSetRenderTarget();

	//-----------------------------------------------------------------------------------------------------------------------------------
	//Horizontal Blur Pass
	//-----------------------------------------------------------------------------------------------------------------------------------
	m_blurHPass.Set(m_deviceContext);
	m_blurHPass.Clear(m_deviceContext);
	m_blurHPass.Render(m_deviceContext);
	m_deviceContext.UnSetRenderTarget();

	//-----------------------------------------------------------------------------------------------------------------------------------
	//Add Bright Pass
	//-----------------------------------------------------------------------------------------------------------------------------------
	m_addBrightPass.Set(m_deviceContext);
	m_addBrightPass.Clear(m_deviceContext);
	m_addBrightPass.Render(m_deviceContext);
	m_deviceContext.UnSetRenderTarget();

	//-----------------------------------------------------------------------------------------------------------------------------------
	//Tone Map Pass
	//-----------------------------------------------------------------------------------------------------------------------------------
	m_toneMapPass.Set(m_deviceContext, m_window.m_swapChain.m_renderTarget);
	m_toneMapPass.Clear(m_deviceContext, m_window.m_swapChain.m_renderTarget);
	m_toneMapPass.Render(m_deviceContext);
}

void
CManager::Update() {
//Update the app delta time
#ifdef OPEN_GL
	m_time = glfwGetTime();
#endif
#ifdef DIRECT_X
	m_time = 0.0f;

	if (m_device.m_driverType == D3D_DRIVER_TYPE_REFERENCE)
	{
		m_time += (float)XM_PI * 0.0125f;
	}
	else
	{
		static DWORD dwTimeStart = 0;
		DWORD dwTimeCur = GetTickCount();

		if (dwTimeStart == 0)
		{
			dwTimeStart = dwTimeCur;
		}

		m_time = (dwTimeCur - dwTimeStart) / 1000.0f;
	}
#endif

#ifdef STEAM_VR
	m_vrManager.Update();
#endif

	//Check for resize event in the app
	Resize();

	//Check for keyboard or mouse inputs
	m_inputHandler.Update(m_window.m_camera, m_window.m_scene, m_time);
	//m_screenAlignedQuad.Update(m_window.GetWindowSize());

	//Update the model's properties
	m_window.m_scene.Update(m_time);

	//Update shaders
	m_shaderManager.Update();

	//--------------------------------------------------------------------------
	//Update Passes
	//--------------------------------------------------------------------------

	//Lighting
	m_lightingPass.SetShaderProgram(m_shaderManager.GetActiveShader());
	m_lightingPass.Update(m_window.m_camera.GetActiveCamera());

	m_lightingPass.Update(m_window.m_camera.GetActiveCamera());
}

void 
CManager::Resize() {
	//Check if the window was resized in the current frame
	if (m_window.Resize()) {
		if (m_window.m_size.size.height == 0 && m_window.m_size.size.width == 0) {
			return;
		}

		//Update viewport size and position
		m_viewport.Resize(m_window.m_size);
		m_deviceContext.SetViewPort(m_viewport);

		//Resize the camera matrices
		m_window.m_camera.Resize(m_window.m_size.size);

		m_window.m_swapChain.Resize(m_device, m_window.m_size.size);

		//Passes
		m_lightingPass.Resize(m_device, m_window.m_size.size);
		m_luminancePass.Resize(m_device, m_window.m_size.size);
		m_brightPass.Resize(m_device, m_window.m_size.size);
		m_blurHPass.Resize(m_device, m_window.m_size.size);
		m_blurVPass.Resize(m_device, m_window.m_size.size);
		m_addBrightPass.Resize(m_device, m_window.m_size.size);
		m_toneMapPass.Resize(m_device, m_window.m_size.size);
	}
}