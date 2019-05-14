#include "../Header/CWindow.h"

#ifdef OPEN_GL
void CWindow::Initialize(Rect dimensions, unsigned int displayMode, const char* name, Color color, std::vector <ModelData>& scene)
{
	//Window
	glutInitDisplayMode(displayMode);

	glutInitWindowSize(dimensions.size.width, dimensions.size.height);
	glutInitWindowPosition(dimensions.pos.x, dimensions.pos.y);

	m_id = glutCreateWindow(name);

	m_clearColor.r = color.r;
	m_clearColor.g = color.g;
	m_clearColor.b = color.b;
	m_clearColor.a = color.a;

	m_size = dimensions;

	glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);

	//Initialize openGL
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Glew failed to initialize" << std::endl;
	}

	std::cout << "OpenGL version: \t" << (char*)glGetString(GL_VERSION) << std::endl;
	std::cout << "OpenGL renderer:\t" << (char*)glGetString(GL_RENDERER) << std::endl;

	//Scene
	m_scene.Initialize(Vector{ 0, 0, 1 }, 0.2f, scene);
	scene.clear();
	
	//Camera
	m_camera.Initialize(VECTOR3(0.0f, 400.0f, 0.0f), VECTOR3(0.0f, 0.0f, 1.0f),
	VECTOR3(0.0f, -1.0f, 0.0f), VECTOR3(1.0f, 0.0f, 0.0f), -90.0f, 0.0f, 2.0f, 0.25f);

	m_camera.AddCamera(VECTOR3(0.0f, 0.0f, -400.0f), VECTOR3(0.0f, 1.0f, 0.0f),
	VECTOR3(0.0f, 0.0f, 1.0f), VECTOR3(1.0f, 0.0f, 0.0f), 90.0f, 0.0f, 2.0f, 0.25f);

	m_camera.SetViewMatrix((45.0f / 360.0f) * 6.283185307f, m_size.size, 0.1f, 500.0f);
}

void CWindow::Render(CShaderProgram& shaderProgram, CCamera& camera, CCamera& otherCamera)
{	
	if (m_scene.m_models.size() == 0)
	{
		std::cout << "Warning: Scene not loaded in window" << std::endl;
	}
	else
	{
		for (int i = 0; i < m_scene.m_models.size(); i++)
		{
			m_scene.Render(shaderProgram, camera, otherCamera);
		}
	}
}

unsigned int CWindow::GetId()
{
	return m_id;
}

Color CWindow::GetClearColor()
{
	return m_clearColor;
}

void CWindow::SetClearColor(Color color)
{
	m_clearColor = color;
}

void CWindow::Clear()
{
	glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}
#endif

#ifdef DIRECT_X
void CWindow::Initialize(WNDPROC pWndProc, HINSTANCE hInstance, std::string title, std::string className, Size size, Color color, INT nCmdShow)
{
	m_isInitialized = true;

	m_hInstance = hInstance;

	m_size.pos = Point{ 0, 0 };
	m_size.size = size;

	m_name = CStringConverter::StringToWide(title);
	m_className = CStringConverter::StringToWide(className);

	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = pWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, (char*)IDI_TUTORIAL1);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = className.c_str();
	wcex.hIconSm = LoadIcon(wcex.hInstance, (char*)IDI_TUTORIAL1);
	RegisterClassEx(&wcex);

	m_handle = CreateWindow(className.c_str(), title.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
	size.width, size.height, NULL, NULL, hInstance, NULL);

	RECT rect = { m_size.pos.x, m_size.pos.y,  m_size.size.width,  m_size.size.height };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	m_swapChain.Initialize(color, size, m_handle);

	ShowWindow(m_handle, nCmdShow);
}
#endif

bool CWindow::Resize()
{
#ifdef OPEN_GL
	if (m_size.size.width != glutGet(GLUT_WINDOW_WIDTH) || m_size.size.height != glutGet(GLUT_WINDOW_HEIGHT))
	{
		m_size.size.width = glutGet(GLUT_WINDOW_WIDTH);
		m_size.size.height = glutGet(GLUT_WINDOW_HEIGHT);
		return true;
	}

	return false;
#endif

#ifdef DIRECT_X

	RECT windowSize;
	GetClientRect(m_handle, &windowSize);

	if(m_isInitialized && ((windowSize.right - windowSize.left) != m_size.size.width ||
	   (windowSize.bottom - windowSize.top) != m_size.size.height)) 
	{
		m_size.size.width = windowSize.right - windowSize.left;
		m_size.size.height = windowSize.bottom - windowSize.top;

		return true;
	}

	return false;
#endif
}