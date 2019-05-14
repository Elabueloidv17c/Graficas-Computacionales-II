#include "../Header/CManager.h"

CManager::CManager()
{
	m_time = 0;
}

#ifdef OPEN_GL
void CManager::Initialize(Rect dimensions)
{
	m_world = glm::mat4(1.0f);

	//Specify the models information
	std::vector <ModelData> scene;

	scene.push_back(ModelData{ "../Models/Batman/Batman.fbx", glm::rotate(
	glm::translate(glm::mat4(1.0f), VECTOR3(0.0f, -50.0f, 0.0f)), glm::radians(180.0f), VECTOR3(0.0f, 1.0f, 0.0f)) });

	scene.push_back(ModelData{ "../Models/Dragon/Dragon.fbx", glm::translate(glm::mat4(1.0f), VECTOR3(150.0f, 0.0f, 0.0f)) });

	scene.push_back(ModelData{ "../Models/Anubis/Anubis.fbx", glm::mat4(1.0f) });

	scene.push_back(ModelData{ "../Models/Quad/Plane.fbx", glm::rotate(glm::scale(
	glm::translate(MATRIX4(1.0f), VECTOR3(-150.0f, 0.0f, 0.0f)), VECTOR3(8.0f, 4.5f, 8.0f)), 
	glm::radians(180.0f), VECTOR3(0.0f, 1.0f, 0.0f)) });

	//Create window with the models especified before
	m_window.Initialize(dimensions, GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL, "OpenGL", Color{ 0.0f, 0.3f, 0.6f, 1.0f }, scene);
	m_viewport.Initialize(dimensions);

	//Create the second render target
	m_renderTexture.Initialize(m_window.m_size.size, Color{ 0.8f, 0.1f, 0.0f, 1.0f });
	m_renderTexture.Unbind();

	m_shaderProgram.Initialize("../Header/Shader/VertexShader.glsl", "../Header/Shader/PixelShader.glsl");

	//Initialize the scene information user interface
	m_userInterface.Initialize();
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

	//Load Shaders from file
	
	const char* shaderFile;

	if (!FAILED(m_shaderProgram.Initialize("../Header/Shader/Shaders.fx", "vs_4_0", "ps_4_0", "VS", "PS")))
	{
		//Create and set the vertex shader
		m_device.CreateVertexShader(m_shaderProgram);
		m_deviceContext.SetVertexShader(m_shaderProgram);
		
		//Create and set the pixel shader
		m_device.CreatePixelShader(m_shaderProgram);
		m_deviceContext.SetPixelShader(m_shaderProgram);

		//Create and set the shader layout
		m_device.CreateInputLayout(m_shaderProgram);
		m_deviceContext.SetInputLayout(m_shaderProgram);

		//Create the constant buffers to pass information to the shader
		m_device.CreateConstantBuffer(m_shaderProgram.m_modelCB, sizeof(MATRIX4));
		m_device.CreateConstantBuffer(m_shaderProgram.m_viewCB, sizeof(MATRIX4));
		m_device.CreateConstantBuffer(m_shaderProgram.m_projectionCB, sizeof(MATRIX4));
		m_device.CreateConstantBuffer(m_shaderProgram.m_colorDataCB, sizeof(ColorData) + sizeof(float));
		m_device.CreateConstantBuffer(m_shaderProgram.m_lightingDataCB, sizeof(VECTOR4));

		//Set The constant buffers
		m_deviceContext.SetVertexConstantBuffer(0, m_shaderProgram.m_modelCB);
		m_deviceContext.SetVertexConstantBuffer(1, m_shaderProgram.m_viewCB);
		m_deviceContext.SetVertexConstantBuffer(2, m_shaderProgram.m_projectionCB);

		//Set the vertex and pixel shaders to the constant buffers
		m_deviceContext.SetVertexConstantBuffer(3, m_shaderProgram.m_colorDataCB);
		m_deviceContext.SetPixelConstantBuffer(3, m_shaderProgram.m_colorDataCB);

		m_deviceContext.SetVertexConstantBuffer(4, m_shaderProgram.m_lightingDataCB);
		m_deviceContext.SetPixelConstantBuffer(4, m_shaderProgram.m_lightingDataCB);
	}

	//----------------------------------------------------------------------------------------------------Scene
	
	//Specify the models information
	std::vector <ModelData> scene;

	scene.push_back(ModelData{ "../Models/Batman/Batman.fbx", glm::rotate(
	glm::translate(glm::mat4(1.0f), VECTOR3(0.0f, -50.0f, 0.0f)), glm::radians(180.0f), VECTOR3(0.0f, 1.0f, 0.0f)) });

	scene.push_back(ModelData{ "../Models/Dragon/Dragon.fbx", glm::translate(glm::mat4(1.0f), VECTOR3(150.0f, 0.0f, 0.0f)) });

	scene.push_back(ModelData{ "../Models/Anubis/Anubis.fbx", glm::mat4(1.0f) });

	scene.push_back(ModelData{ "../Models/Quad/Plane.fbx", glm::rotate(glm::scale(
	glm::translate(MATRIX4(1.0f), VECTOR3(-150.0f, 0.0f, 0.0f)), VECTOR3(8.0f, 4.5f, 8.0f)),
	glm::radians(180.0f), VECTOR3(0.0f, 0.0f, 1.0f)) });

	//Load models from file
	m_window.m_scene.Initialize(Vector{ 0, 0, 1 }, 0.7f, scene, m_device);

	//----------------------------------------------------------------------------------------------------Camera

	//Initialize the main camera properties
	m_window.m_camera.Initialize(VECTOR3(0.0f, 400.0f, 0.0f), VECTOR3(0.0f, 0.0f, 1.0f),
	VECTOR3(0.0f, -1.0f, 0.0f), VECTOR3(1.0f, 0.0f, 0.0f), -90.0f, 0.0f, 2.0f, 0.25f);

	//Add secundary camera to act as security camera
	m_window.m_camera.AddCamera(VECTOR3(0.0f, 0.0f, -400.0f), VECTOR3(0.0f, 1.0f, 0.0f),
	VECTOR3(0.0f, 0.0f, 1.0f), VECTOR3(1.0f, 0.0f, 0.0f), 90.0f, 0.0f, 2.0f, 0.25f);

	//Set The view matrix´s initial position
	m_window.m_camera.SetViewMatrix((45.0f / 360.0f) * 6.283185307f, m_window.m_size.size, 0.1f, 500.0f);

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
	m_shaderProgram.Bind();
	m_shaderProgram.UpdateWorld(m_world);

	//--------------------------------------------------------------------------------------------------------------
	//Render the scene on the secure camera
	m_shaderProgram.UpdateCamera(m_window.m_camera.m_cameras[0]);
	m_renderTexture.Bind(m_window.m_size.size);
	m_renderTexture.Clear();
	m_window.Render(m_shaderProgram, m_window.m_camera.m_cameras[0], m_window.m_camera.m_cameras[1]);

	//Set the rendered texture as the screeen model texture 
	m_window.m_scene.m_models[3].m_meshes[0].m_material.m_diffuse.m_id = m_renderTexture.m_idTexture;
	
	m_renderTexture.Unbind();
	//--------------------------------------------------------------------------------------------------------------
	//Render the scene on the main camera
	m_shaderProgram.UpdateCamera(m_window.m_camera.m_cameras[1]);

	m_window.Clear();
	m_window.Render(m_shaderProgram, m_window.m_camera.m_cameras[1], m_window.m_camera.m_cameras[1]);
	
	//Render user interface to see the scene information
	m_userInterface.Initframe();
	m_userInterface.SetFrame((float)m_window.m_scene.GetNumVertices(), (float)m_window.m_scene.GetNumFaces(), 
	(float)m_window.m_scene.GetNumMeshes(), (float)m_window.m_scene.GetNumModels());
	m_userInterface.RenderFrame();

	//Present Frame
	glutSwapBuffers();
#endif

#ifdef DIRECT_X

	//--------------------------------------------------------------------------------------------------------------
	//Render the scene on the secure camera
	m_renderTexture.SetRenderTarget(m_deviceContext);
	m_renderTexture.ClearRenderTarget(m_deviceContext);
	m_deviceContext.UpdateLightingData(m_shaderProgram.m_lightingDataCB, m_window.m_scene.m_lightingData);
	m_window.m_scene.Render(m_deviceContext, m_shaderProgram, m_window.m_camera.m_cameras[0], m_window.m_camera.m_cameras[1]);

	//Set the rendered texture as the screen model texture 
	m_window.m_scene.m_models[3].m_meshes[0].m_material.m_diffuse.m_pointer = m_renderTexture.m_shaderResource;
	
	//--------------------------------------------------------------------------------------------------------------
	//Render the scene on the main camera
	m_deviceContext.SetRenderTarget(m_window.m_swapChain);
	m_deviceContext.ClearColor(m_window.m_swapChain);
	m_deviceContext.ClearDepthStencil(m_window.m_swapChain);
	m_deviceContext.UpdateLightingData(m_shaderProgram.m_lightingDataCB, m_window.m_scene.m_lightingData);
	m_window.m_scene.Render(m_deviceContext, m_shaderProgram, m_window.m_camera.m_cameras[1], m_window.m_camera.m_cameras[1]);

	//Render user interface to see the scene information
	m_userInterface.Initframe();
	m_userInterface.SetFrame((float)m_window.m_scene.GetNumVertices(), (float)m_window.m_scene.GetNumFaces(), 
	(float)m_window.m_scene.GetNumMeshes(), (float)m_window.m_scene.GetNumModels());
	m_userInterface.RenderFrame();

	//Present Frame
	m_window.m_swapChain.Present(0, 0);
#endif
}

void CManager::Update()
{
//Update the app delta time
#ifdef OPEN_GL
	m_time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
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

	//Check for resize event in the app
	Resize();

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


		//Resize the render texture
#ifdef OPEN_GL
		m_renderTexture.Initialize(m_window.m_size.size, m_renderTexture.m_color);
#endif

#ifdef DIRECT_X
		m_renderTexture.Initialize(m_device, m_window.m_size.size, m_renderTexture.m_color);

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