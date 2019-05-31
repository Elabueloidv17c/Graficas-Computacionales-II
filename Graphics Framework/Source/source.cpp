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
		app.m_window.m_isInitialized = false;
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

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, INT nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

#ifdef DIRECT_X

	app.Initialize(WndProc, hInstance, "DirectX", "class", Size{ 1280, 720 }, Color{ 0.0f, 0.5f, 0.125f, 1.0f }, nCmdShow);

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
#endif //DirectX
#ifdef OPEN_GL
	app.Initialize(Rect{ 0, 0, 1280, 720 });

	while (!glfwWindowShouldClose(app.m_window.m_pointer))
	{        
		// Check if any events have been activiated
		glfwPollEvents();

		//Update and render the app
		app.Update();
		app.Render();
	}

	glfwTerminate();
#endif //OpenGL
}
