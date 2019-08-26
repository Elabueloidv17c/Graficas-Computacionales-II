#include "../Header/CWindow.h"

#ifdef OPEN_GL
void CWindow::Initialize(CDevice& device, Rect dimensions, unsigned int displayMode, const char* name, Color color)
{
	glfwInit();

	m_swapChain.m_color = color;
	m_clearColor.r = color.r;
	m_clearColor.g = color.g;
	m_clearColor.b = color.b;
	m_clearColor.a = color.a;

	//Create window
	m_pointer = glfwCreateWindow((const int)dimensions.size.width, (const int)dimensions.size.height, name, nullptr, nullptr);
	m_size = dimensions;

	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	//Check if the windows was created correctly
	if (!m_pointer)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return;
	}

	//Create deviceContext
	glfwMakeContextCurrent(m_pointer);

	//Set frame buffer
	//m_size.pos.x = 0; m_size.pos.y = 0;
	//glfwGetFramebufferSize(m_pointer, (int*)&m_size.size.width, (int*)&m_size.size.height);

	//Initialize openGL
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Glew failed to initialize" << std::endl;
	}

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	
	std::cout << "OpenGL version: \t" << (char*)glGetString(GL_VERSION) << std::endl;
	std::cout << "OpenGL renderer:\t" << (char*)glGetString(GL_RENDERER) << std::endl;
	
	glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
}
#endif

#ifdef DIRECT_X
void CWindow::Initialize(WNDPROC pWndProc, HINSTANCE hInstance, std::string title, std::string className, Size size, Color color, INT nCmdShow)
{
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

	ShowWindow(m_handle, nCmdShow);
}
#endif

void CWindow::Render(CDeviceContext& deviceContext, CLightingPass& pass, CCamera& camera)
{

}

Size CWindow::GetWindowSize()
{
#ifdef DIRECT_X
	RECT windowSize;
	GetClientRect(m_handle, &windowSize);

	return Size{ (float)(windowSize.right - windowSize.left), (float)(windowSize.bottom - windowSize.top) };
#endif
#ifdef OPEN_GL
	int width;
	int height;
	glfwGetFramebufferSize(m_pointer, &width, &height);
	Size windowSize;
	windowSize.width = width;
	windowSize.height = height;

	return windowSize;
#endif
}

bool CWindow::Resize()
{
#ifdef DIRECT_X

	RECT windowSize;
	GetClientRect(m_handle, &windowSize);

	if(GetWindowSize().height != m_size.size.height || GetWindowSize().width != m_size.size.width)
	{
		m_size.size = GetWindowSize();
		return true;
	}

	return false;
#endif
#ifdef OPEN_GL
	Point windowSize;
	glfwGetWindowSize(m_pointer, &windowSize.x, &windowSize.y);

	if (m_size.size.width != windowSize.x || m_size.size.height != windowSize.y)
	{
		m_size.size.width = windowSize.x;
		m_size.size.height = windowSize.y;
		return true;
}

	return false;
#endif

}

CWindow::CWindow()
{
#ifdef OPEN_GL
	m_pointer = nullptr;
#endif
}

CWindow::~CWindow()
{
#ifdef OPEN_GL
	glfwDestroyWindow(m_pointer);
#endif

}