#include "../Header/CUserInterface.h"

#ifdef OPEN_GL
#include <windows.h>
#endif

CUserInterface::~CUserInterface()
{
	ImGui_ImplWin32_Shutdown();

#ifdef OPEN_GL
	ImGui_ImplOpenGL3_Shutdown();
#endif

#ifdef DIRECT_X
	ImGui_ImplDX11_Shutdown();
#endif
}

void CUserInterface::Initialize()
{
#ifdef DIRECT_X
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
#endif

#ifdef OPEN_GL

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	HWND windowHandle = FindWindow(NULL, "OpenGL");
	HDC deviceContext = GetDC(windowHandle);
	ImGui_ImplWin32_Init(windowHandle);

	ImGui_ImplOpenGL3_Init("#version 330 core");

	ImGui::StyleColorsDark();
#endif
}

void CUserInterface::Initframe()
{
#ifdef DIRECT_X
	ImGui_ImplDX11_NewFrame();
#endif

#ifdef OPEN_GL
	ImGui_ImplOpenGL3_NewFrame();
#endif
	ImGui_ImplWin32_NewFrame();
}

void CUserInterface::SetFrame(float vertices, float faces, float meshes, float models)
{
	ImGui::NewFrame();
	ImGui::Begin("Scene Information");
	ImGui::Text("     ------Performance------");
	ImGui::Text("");
	ImGui::Text("Frame Rate:			  %.1f/s", ImGui::GetIO().Framerate);
	ImGui::Text("Frame Pacing:			%.1f ms", 1000.0f / ImGui::GetIO().Framerate);
	ImGui::Text("");
	ImGui::Text("     -------Geometry-------");
	ImGui::Text("");
	ImGui::Text("Models:				 %.0f", models);
	ImGui::Text("Meshes:				 %.0f", meshes);
	ImGui::Text("Vertices:			   %.0f", vertices);
	ImGui::Text("Faces:				  %.0f", faces);
	ImGui::End();
}

void CUserInterface::RenderFrame()
{
	ImGui::Render();

#ifdef DIRECT_X
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif

#ifdef OPEN_GL
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif
}