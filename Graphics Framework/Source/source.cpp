#pragma once
#include "../Header/Header.h" 
#include "../Header/CManager.h"

#ifndef DIRECT_X
#include <windows.h>
#endif

CManager app;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
	{
		return false;
	}

#ifdef DIRECT_X
	unsigned char keycode = static_cast<unsigned char>(wParam);
	Point mousePosition = { LOWORD(lParam), HIWORD(lParam) };

	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_LBUTTONDOWN:
	{
		app.m_inputHandler.MouseKey(InputEvent::LeftMouse, true);
		break;
	}
	case WM_LBUTTONUP:
	{
		app.m_inputHandler.MouseKey(InputEvent::LeftMouse, false);
	}
	case WM_MOUSEMOVE:
	{
		if (app.m_inputHandler.m_events & InputEvent::LeftMouse)
		{
			app.m_window.m_camera.Rotate(mousePosition.x, mousePosition.y);
		}
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
#endif
}

#ifdef DIRECT_X
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, INT nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

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
}
#endif //DirectX

#ifdef OPEN_GL
void Render()
{
	app.Render();
}

void KeyBoardDown(unsigned char key, int x, int y)
{
	app.m_inputHandler.KeyBoardDown(key);
}

void KeyBoardUp(unsigned char key, int x, int y)
{
	app.m_inputHandler.KeyBoardUp(key);
}

void MouseKey(int button, int state, int x, int y)
{
	app.m_inputHandler.MouseKey(button, state);
}

void CameraMovement(int x, int y)
{
	if (app.m_inputHandler.m_events & InputEvent::LeftMouse)
	{
		app.m_window.m_camera.Rotate(x, y);
	}
}

void Update()
{
	glutPostRedisplay();
	app.Update();
	app.m_inputHandler.Update(app.m_window.m_camera, app.m_window.m_scene);
}

void SetCallbackFunctions()
{
	// Set render function
	glutDisplayFunc(Render);

	// Set update function
	glutIdleFunc(Update);

	// Set Mouse Movement function
	glutMotionFunc(CameraMovement);

	// Set Mouse input function
	glutMouseFunc(MouseKey);

	// Set Keyboard press function
	glutKeyboardFunc(KeyBoardDown);

	// Set keyboard release func
	glutKeyboardUpFunc(KeyBoardUp);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	app.Initialize(Rect{ 0, 0, 1280, 720 });
	SetCallbackFunctions();

	glutMainLoop();

	return 0;
}

#endif //OpenGL
