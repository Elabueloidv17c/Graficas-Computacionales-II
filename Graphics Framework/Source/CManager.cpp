#include "../Header/CManager.h"

CManager::CManager()
{
	m_time = 0;
#ifdef OPEN_GL
	m_lastFrame = 0;
#endif
}

#ifdef OPEN_GL
#include <Windows.h>

void CManager::Initialize(Rect dimensions)
{
	m_world = glm::mat4(1.0f);

	// Init GLFW
	glfwInit();

	//Specify the models information
	std::vector <ModelData> scene;
	scene.push_back(ModelData{ "../Models/Scene/Scene.fbx", glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), VECTOR3(0.0f, 1.0f, 0.0f)) });

	//Create window with the models especified before
	m_window.Initialize(dimensions, 0, "OpenGL", Color{ 0.0f, 0.3f, 0.6f, 1.0f }, scene);
	m_viewport.Initialize(dimensions);

	m_window.m_scene.m_pointRadius = 2.0f;
	m_window.m_scene.m_spotRadius = 3.0f;
	
	//Load lighting and color data
	m_window.m_scene.SetLightData(LightingData
	{
		Vector{0.0f, 0.0f, 0.0f},		//directional;
		1.0f,							//specularPower;
	
		Vector{0.0f, 0.3f, -3.8f},		//pointPosition;
		m_window.m_scene.m_pointRadius,
	
		Vector{0.0f, 0.0f, -10.0f},		//viewPosition;
		m_window.m_scene.m_spotRadius,
	
		Vector{10.0f, 5.0f, 2.0f},		//spotPosition;
		0.04f,							//spotAlpha;
	
		Vector{0.0f, 0.0f, 1.0f},		//spotDirection;
		0.5f,							//spotBeta;
	});
	
	m_window.m_scene.SetColorData(ColorData
	{
		Color{1.0f, 1.0f, 1.0f, 1.0f},	//Directional Light
		Color{0.0f, 1.0f, 0.0f, 1.0f},	//Point Light
		Color{0.0f, 0.0f, 1.0f, 1.0f},	//Spot Light
	
		Color{1.0f, 1.0f, 1.0f, 1.0f},
		Color{0.0f, 1.0f, 0.0f, 1.0f},
	
		1.0f,
		1.0f,
		0.2f
	});

	//Create the second render target
	//m_renderTexture.Initialize(m_window.m_size.size, Color{ 0.8f, 0.1f, 0.0f, 1.0f });
	//m_renderTexture.Unbind();

	std::vector <ShaderProgramData> shaders(4);
	shaders[0] = ShaderProgramData{ ShaderData{ "../Header/Shader/VertexShader.glsl", GL_VERTEX_SHADER, false, false },
	ShaderData{ "../Header/Shader/PixelShader.glsl", GL_FRAGMENT_SHADER, false, false } };
	shaders[1] = ShaderProgramData{ ShaderData{ "../Header/Shader/VertexShader.glsl", GL_VERTEX_SHADER, false, true },
	ShaderData{ "../Header/Shader/PixelShader.glsl", GL_FRAGMENT_SHADER, false, false } };
	shaders[2] = ShaderProgramData{ ShaderData{ "../Header/Shader/VertexShader.glsl", GL_VERTEX_SHADER, true, false },
	ShaderData{ "../Header/Shader/PixelShader.glsl", GL_FRAGMENT_SHADER, false, false } };
	shaders[3] = ShaderProgramData{ ShaderData{ "../Header/Shader/VertexShader.glsl", GL_VERTEX_SHADER, true, true },
	ShaderData{ "../Header/Shader/PixelShader.glsl", GL_FRAGMENT_SHADER, false, false } };

	m_shaderManager.Initialize(shaders);
	m_shaderManager.SetActiveShader(0);

	//Initialize the scene information user interface
	m_userInterface.Initialize();
	ImGui_ImplWin32_Init(m_window.m_pointer);
	ImGui_ImplOpenGL3_Init("#version 400 core");
}
#endif

#ifdef DIRECT_X
void CManager::Initialize(WNDPROC pWndProc, HINSTANCE hInstance, std::string title, std::string className, Size size, Color color, INT nCmdShow)
{
	//Create Window
	m_window.Initialize(pWndProc, hInstance, title, className, size, color, nCmdShow);

	//Create DeviceContext, SwapChain and render target
	m_device.Initialize(m_deviceContext, m_window.m_swapChain);
	m_deviceContext.SetRenderTarget(m_window.m_swapChain);
	m_renderTexture.Initialize(m_device, size, Color{0.8f, 0.1f, 0.0f, 1.0f});

	//Create Samplers
	m_sampler.Initialize();
	m_device.CreateSamplerState(m_sampler);
	m_deviceContext.SetSampler(m_sampler);

	//Create Viewports
	m_viewport.Initialize(m_window.m_size);
	m_deviceContext.SetViewPort(m_viewport);

	//Set triangle based render
	m_deviceContext.SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//---------------------------------------------------------------------------------------------------Shaders


	//LCreate the information to load the shaders
	std::vector <ShaderProgramData> shaders(4);
	shaders[0] = ShaderProgramData{ ShaderData{ "../Header/Shader/VertexShader.hlsl", "VS", "vs_4_0", false, false },
	ShaderData{ "../Header/Shader/PixelShader.hlsl", "PS", "ps_4_0", false, false } };
	shaders[1] = ShaderProgramData{ ShaderData{ "../Header/Shader/VertexShader.hlsl", "VS", "vs_4_0", false, true },
	ShaderData{ "../Header/Shader/PixelShader.hlsl", "PS", "ps_4_0", false, true } };
	shaders[2] = ShaderProgramData{ ShaderData{ "../Header/Shader/VertexShader.hlsl", "VS", "vs_4_0", true, false },
	ShaderData{ "../Header/Shader/PixelShader.hlsl", "PS", "ps_4_0", true, false } };
	shaders[3] = ShaderProgramData{ ShaderData{ "../Header/Shader/VertexShader.hlsl", "VS", "vs_4_0", true, true },
	ShaderData{ "../Header/Shader/PixelShader.hlsl", "PS", "ps_4_0", true, true } };
	m_shaderManager.SetActiveShader(0);

	//Load all shaders
	m_shaderManager.Initialize(shaders);

	//Initialize each shader
	for (int i = 0; i < shaders.size(); i++)
	{
		//Create the vertex shader
		m_device.CreateVertexShader(*m_shaderManager.GetShaderAt(i));
		
		//Create the pixel shader
		m_device.CreatePixelShader(*m_shaderManager.GetShaderAt(i));

		//Create the shader layout
		m_device.CreateInputLayout(*m_shaderManager.GetShaderAt(i));

		//Create the constant buffers to pass information to the shader
		m_device.CreateConstantBuffer(m_shaderManager.GetShaderAt(i)->m_modelCB, sizeof(MATRIX4));
		m_device.CreateConstantBuffer(m_shaderManager.GetShaderAt(i)->m_viewCB, sizeof(MATRIX4));
		m_device.CreateConstantBuffer(m_shaderManager.GetShaderAt(i)->m_projectionCB, sizeof(MATRIX4));
		m_device.CreateConstantBuffer(m_shaderManager.GetShaderAt(i)->m_colorDataCB, sizeof(ColorData) + sizeof(float));
		m_device.CreateConstantBuffer(m_shaderManager.GetShaderAt(i)->m_lightingDataCB, sizeof(LightingData));

	}

	//Set active shader properties
	m_deviceContext.SetVertexShader(*m_shaderManager.GetActiveShader());
	m_deviceContext.SetPixelShader(*m_shaderManager.GetActiveShader());
	m_deviceContext.SetInputLayout(*m_shaderManager.GetActiveShader());

	//Set The constant buffers
	m_deviceContext.SetVertexConstantBuffer(0, m_shaderManager.GetActiveShader()->m_modelCB);
	m_deviceContext.SetVertexConstantBuffer(1, m_shaderManager.GetActiveShader()->m_viewCB);
	m_deviceContext.SetVertexConstantBuffer(2, m_shaderManager.GetActiveShader()->m_projectionCB);

	//Set the vertex and pixel shaders to the constant buffers
	m_deviceContext.SetVertexConstantBuffer(3, m_shaderManager.GetActiveShader()->m_colorDataCB);
	m_deviceContext.SetPixelConstantBuffer(3, m_shaderManager.GetActiveShader()->m_colorDataCB);

	m_deviceContext.SetVertexConstantBuffer(4, m_shaderManager.GetActiveShader()->m_lightingDataCB);
	m_deviceContext.SetPixelConstantBuffer(4, m_shaderManager.GetActiveShader()->m_lightingDataCB);

	//----------------------------------------------------------------------------------------------------Scene
	
	//Specify the models information
	std::vector <ModelData> scene;
	scene.push_back(ModelData{ "../Models/Scene/Scene.fbx", glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), VECTOR3(0.0f, 1.0f, 0.0f)) });

	//Load models from file
	m_window.m_scene.Initialize(scene, m_device);

	m_window.m_scene.m_pointRadius = 2.0f;
	m_window.m_scene.m_spotRadius = 3.0f;

	//Load lighting and color data
	m_window.m_scene.SetLightData(LightingData
	{
		Vector{0.0f, 0.0f, 0.0f},		//directional;
		1.0f,							//specularPower;
		
		Vector{0.0f, 0.3f, -3.8f},		//pointPosition;
		m_window.m_scene.m_pointRadius,
		
		Vector{0.0f, 0.0f, -10.0f},		//viewPosition;
		m_window.m_scene.m_spotRadius,
		
		Vector{10.0f, 5.0f, 2.0f},		//spotPosition;
		0.04f,							//spotAlpha;
		
		Vector{0.0f, 0.0f, 1.0f},		//spotDirection;
		0.5f,							//spotBeta;
	});


	m_window.m_scene.SetColorData(ColorData
		{
			Color{1.0f, 1.0f, 1.0f, 1.0f},	//Directional Light
			Color{0.0f, 1.0f, 0.0f, 1.0f},	//Point Light
			Color{0.0f, 0.0f, 1.0f, 1.0f},	//Spot Light

			Color{1.0f, 1.0f, 1.0f, 1.0f},
			Color{0.0f, 1.0f, 0.0f, 1.0f},

			1.0f,
			1.0f,
			0.2f
		});

	//----------------------------------------------------------------------------------------------------Camera

	//Add secundary camera
	m_window.m_camera.Initialize(VECTOR3(0.0f, 15.0f, 0.0f), VECTOR3(0.0f, 0.0f, 1.0f),
	VECTOR3(0.0f, -1.0f, 0.0f), VECTOR3(1.0f, 0.0f, 0.0f), -90.0f, 0.0f, 0.003f, 0.25f);

	//Initialize the main camera properties
	m_window.m_camera.AddCamera(VECTOR3(0.0f, 3.0f, -10.0f), VECTOR3(0.0f, 1.0f, 0.0f),
	VECTOR3(0.0f, 0.0f, 1.0f), VECTOR3(1.0f, 0.0f, 0.0f), 90.0f, 0.0f, 0.003f, 0.20f);

	//Set The view matrix´s initial position
	m_window.m_camera.SetViewMatrix((60.0f / 360.0f) * 6.283185307f, m_window.m_size.size, 0.1f, 1000.0f);

	//----------------------------------------------------------------------------------------------------User Interface

	//Create  the user interface using the ImGui Library
	m_userInterface.Initialize();
	ImGui_ImplWin32_Init(m_window.m_handle);
	ImGui_ImplDX11_Init(m_device.m_pointer, m_deviceContext.m_pointer);
}
#endif

void CManager::Render()
{
#ifdef OPEN_GL

	//Set Active Shader
	m_shaderManager.GetActiveShader()->Bind();
	m_shaderManager.GetActiveShader()->UpdateWorld(m_world);
	m_shaderManager.GetActiveShader()->UpdateCamera(*m_window.m_camera.GetActiveCamera());

	//Lighting Update
	m_window.m_scene.m_lightingData.viewPosition.x = m_window.m_camera.GetActiveCamera()->m_position.x;
	m_window.m_scene.m_lightingData.viewPosition.y = m_window.m_camera.GetActiveCamera()->m_position.y;
	m_window.m_scene.m_lightingData.viewPosition.z = m_window.m_camera.GetActiveCamera()->m_position.z;

	m_window.m_scene.m_lightingData.spotDirection.x = m_window.m_camera.GetActiveCamera()->m_front.x;
	m_window.m_scene.m_lightingData.spotDirection.y = m_window.m_camera.GetActiveCamera()->m_front.y;
	m_window.m_scene.m_lightingData.spotDirection.z = m_window.m_camera.GetActiveCamera()->m_front.z;

	m_window.Clear();
	m_window.Render(*m_shaderManager.GetActiveShader(), *m_window.m_camera.GetActiveCamera(), *m_window.m_camera.GetSecundaryCamera());

	//Render user interface to see the scene information
	m_userInterface.SetFrame((float)m_window.m_scene.GetNumVertices(), (float)m_window.m_scene.GetNumFaces(),
	(float)m_window.m_scene.GetNumMeshes(), (float)m_window.m_scene.GetNumModels(), m_shaderManager.m_isVertex,
	m_shaderManager.m_isBlinn, m_window.m_scene.m_lightingData.SpotRadius, m_window.m_scene.m_lightingData.spotAlpha,
	m_window.m_scene.m_lightingData.spotBeta, m_window.m_scene.m_lightingData.PointRadius,
	m_window.m_scene.m_colorData.directionalColor, m_window.m_scene.m_colorData.pointColor,
	m_window.m_scene.m_colorData.spotColor);
	
	m_userInterface.RenderFrame();

	//Present Frame
	glfwSwapBuffers(m_window.m_pointer);
#endif

#ifdef DIRECT_X

	//Lighting Update
	m_window.m_scene.m_lightingData.viewPosition.x = m_window.m_camera.GetActiveCamera()->m_position.x;
	m_window.m_scene.m_lightingData.viewPosition.y = m_window.m_camera.GetActiveCamera()->m_position.y;
	m_window.m_scene.m_lightingData.viewPosition.z = m_window.m_camera.GetActiveCamera()->m_position.z;

	m_window.m_scene.m_lightingData.spotDirection.x = m_window.m_camera.GetActiveCamera()->m_front.x;
	m_window.m_scene.m_lightingData.spotDirection.y = m_window.m_camera.GetActiveCamera()->m_front.y;
	m_window.m_scene.m_lightingData.spotDirection.z = m_window.m_camera.GetActiveCamera()->m_front.z;

	//Shader Update

	//Set active shader properties
	m_deviceContext.SetVertexShader(*m_shaderManager.GetActiveShader());
	m_deviceContext.SetPixelShader(*m_shaderManager.GetActiveShader());
	m_deviceContext.SetInputLayout(*m_shaderManager.GetActiveShader());

	//Set The constant buffers
	m_deviceContext.SetVertexConstantBuffer(0, m_shaderManager.GetActiveShader()->m_modelCB);
	m_deviceContext.SetVertexConstantBuffer(1, m_shaderManager.GetActiveShader()->m_viewCB);
	m_deviceContext.SetVertexConstantBuffer(2, m_shaderManager.GetActiveShader()->m_projectionCB);

	//Set the vertex and pixel shaders to the constant buffers
	m_deviceContext.SetVertexConstantBuffer(3, m_shaderManager.GetActiveShader()->m_colorDataCB);
	m_deviceContext.SetPixelConstantBuffer(3, m_shaderManager.GetActiveShader()->m_colorDataCB);

	m_deviceContext.SetVertexConstantBuffer(4, m_shaderManager.GetActiveShader()->m_lightingDataCB);
	m_deviceContext.SetPixelConstantBuffer(4, m_shaderManager.GetActiveShader()->m_lightingDataCB);

	m_deviceContext.UpdateLightingData(
	m_shaderManager.GetActiveShader()->m_lightingDataCB,
	m_window.m_scene.m_lightingData
	);

	
	//Render the scene on the secure camera
	//m_renderTexture.SetRenderTarget(m_deviceContext);
	//m_renderTexture.ClearRenderTarget(m_deviceContext);
	//m_window.m_scene.Render(m_deviceContext, m_shaderProgram, m_window.m_camera.m_cameras[0], m_window.m_camera.m_cameras[1]);
	//Set the rendered texture as the screen model texture 
	//m_window.m_scene.m_models[3].m_meshes[0].m_material.m_diffuse.m_pointer = m_renderTexture.m_shaderResource;
	
	//Render the scene on the main camera
	m_deviceContext.SetRenderTarget(m_window.m_swapChain);
	m_deviceContext.ClearColor(m_window.m_swapChain);
	m_deviceContext.ClearDepthStencil(m_window.m_swapChain);
	m_window.m_scene.Render(m_deviceContext, *m_shaderManager.GetActiveShader(), *m_window.m_camera.GetActiveCamera(), 
	*m_window.m_camera.GetSecundaryCamera());

	//Render user interface to see the scene information
	m_userInterface.Initframe();
	m_userInterface.SetFrame((float)m_window.m_scene.GetNumVertices(), (float)m_window.m_scene.GetNumFaces(), 
	(float)m_window.m_scene.GetNumMeshes(), (float)m_window.m_scene.GetNumModels(), m_shaderManager.m_isVertex, 
	m_shaderManager.m_isBlinn, m_window.m_scene.m_lightingData.SpotRadius, m_window.m_scene.m_lightingData.spotAlpha, 
	m_window.m_scene.m_lightingData.spotBeta, m_window.m_scene.m_lightingData.PointRadius,
	m_window.m_scene.m_colorData.directionalColor, m_window.m_scene.m_colorData.pointColor,
	m_window.m_scene.m_colorData.spotColor);

	m_userInterface.RenderFrame();

	//Present Frame
	m_window.m_swapChain.Present(0, 0);
#endif
}

void CManager::Update()
{
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

	m_window.m_scene.RotateLight(m_time);

	//Check for resize event in the app
	Resize();

	//Update shders
	m_shaderManager.Update();

	//Check for keyboard or mouse inputs
	m_inputHandler.Update(m_window.m_camera, m_window.m_scene, m_time);

	//Update the model's properties
	m_window.m_scene.Update(m_time);
}

void CManager::Resize()
{
	//Check if the window was resized in the current frame
	if (m_window.Resize())
	{
		//Update viewport size and position
		m_viewport.Resize(m_window.m_size);


#ifdef DIRECT_X
		//m_renderTexture.Initialize(m_device, m_window.m_size.size, m_renderTexture.m_color);

		m_window.m_swapChain.Resize(m_window.m_size.size);

		ID3D11Texture2D* pBackBuffer = NULL;
		m_window.m_swapChain.m_pointer->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		m_device.CreateRenderTargetView(m_window.m_swapChain.m_renderTarget, pBackBuffer);
		pBackBuffer->Release();

		m_device.CreateDepthStencilTexture(m_window.m_swapChain.m_depthStencil);
		m_device.CreateDepthStencilView(m_window.m_swapChain.m_depthStencil);
		m_deviceContext.SetRenderTarget(m_window.m_swapChain);
		m_deviceContext.SetViewPort(m_viewport);
#endif

		//Resize the camera matrices
		m_window.m_camera.Resize(m_window.m_size.size);
	}
}