#pragma once
#include "../Header/Header.h" 
#include "../Header/CManager.h"

#ifndef DIRECT_X
#include <windows.h>
#endif

CManager app;

#ifdef DIRECT_X
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
	{
		return false;
	}
	unsigned char keycode = static_cast<unsigned char>(wParam);
	Point mousePosition = { LOWORD(lParam), HIWORD(lParam) };

	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_RBUTTONDOWN:
	{
		app.m_inputHandler.MouseKey(InputEvent::RightMouse, true);
		break;
	}
	case WM_RBUTTONUP:
	{
		app.m_inputHandler.MouseKey(InputEvent::RightMouse, false);
	}
	case WM_MOUSEMOVE:
	{
		if (app.m_inputHandler.m_events & InputEvent::RightMouse)
		{
			app.m_window.m_camera.Rotate(mousePosition.x, mousePosition.y);
		}
	}
	case WM_LBUTTONDOWN:
	{
		app.m_inputHandler.MouseKey(InputEvent::LeftMouse, true);
		break;
	}
	case WM_LBUTTONUP:
	{
		app.m_inputHandler.MouseKey(InputEvent::LeftMouse, false);
	}
	case WM_KEYDOWN:
	{
		app.m_inputHandler.KeyBoardDown(wParam);
		break;
	}
	case WM_KEYUP:
	{
		app.m_inputHandler.KeyBoardUp(wParam);
	}
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
	{	
		PostQuitMessage(0);
		break;
	}
	default:
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	}
	return 0;
}
#endif

#ifdef OPEN_GL
void MouseMovement(GLFWwindow* window, double x, double y)
{
	if (app.m_inputHandler.m_events & InputEvent::RightMouse)
	{
		app.m_window.m_camera.Rotate(x, y);
	}
}

void MouseButtons(GLFWwindow* window, int button, int action, int mods)
{
	//--------------------------------------------------------------------
	//Mouse Buttons
	//--------------------------------------------------------------------

	// Left down
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		app.m_inputHandler.m_events |= InputEvent::LeftMouse;
		std::cout << "Left mouse button down" << std::endl;
	}
	//Left up
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		app.m_inputHandler.m_events &= ~(InputEvent::LeftMouse);
		std::cout << "Left mouse button up" << std::endl;
	}
	//Right down
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		app.m_inputHandler.m_events |= InputEvent::RightMouse;
		std::cout << "Right mouse button down" << std::endl;
	}
	//Right up
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{
		app.m_inputHandler.m_events &= ~(InputEvent::RightMouse);
		std::cout << "Right mouse button Up" << std::endl;
	}
	if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
	{
		app.m_inputHandler.m_events |= InputEvent::MiddleMouse;
		std::cout << "Middle mouse button down" << std::endl;
	}
	if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE)
	{
		app.m_inputHandler.m_events &= ~(InputEvent::MiddleMouse);
		std::cout << "Middle mouse button up" << std::endl;
	}
}

void KeyboardInput(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	app.m_inputHandler.ProcessInput(window, key, action);
}

void SetCallBackFunctions()
{
	glfwSetInputMode(app.m_window.m_pointer, GLFW_STICKY_KEYS, GLFW_TRUE);
	glfwSetCursorPosCallback(app.m_window.m_pointer, MouseMovement);
	glfwSetMouseButtonCallback(app.m_window.m_pointer, MouseButtons);
	glfwSetKeyCallback(app.m_window.m_pointer, KeyboardInput);
}
#endif

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, INT nCmdShow)
{
#ifdef OPEN_GL
	app.Initialize(WindowData{ "OpenGL", Rect{ 0, 0, 1280, 720 }, Color{ 0.0f, 0.125f, 0.6f, 1.0f } });
	SetCallBackFunctions();

	while (!glfwWindowShouldClose(app.m_window.m_pointer))
	{
		// Check if any events have been activiated
		glfwPollEvents();

		//Update and render the app
		app.Update();
		app.Render();
	}

	glfwTerminate();
#endif
#ifdef DIRECT_X
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	app.Initialize(WindowData{ "DirectX", Rect{ 0, 0, 1280, 720 }, Color{ 0.0f, 0.5f, 0.125f, 1.0f }, "class", WndProc, hInstance,  nCmdShow });

	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		app.Update();

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			app.Render();
		}
	}
	return (int)msg.wParam;
#endif
}