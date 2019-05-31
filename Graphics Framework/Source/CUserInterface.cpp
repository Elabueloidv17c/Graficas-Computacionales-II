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
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();	
	ImGui::StyleColorsDark();
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

void CUserInterface::SetFrame(float vertices, float faces, float meshes, float models, bool& isVertex, bool& isBlinn,
float& spotRadius, float& spotAlpha, float& spotBeta, float& pointRadius, Color& directionalColor, Color& pointColor, 
Color& spotColor)
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
	
	ImGui::Begin("Controllers");
		ImGui::Text("     ------Shader Parameters------");
		ImGui::Text("");
		ImGui::Checkbox("Vertex Calc", &isVertex);
		ImGui::Checkbox("Blinn Reflect", &isBlinn);
		ImGui::Text("");
		ImGui::Text("     ------Light Parameters------");
		ImGui::Text("");
		ImGui::SliderFloat("Spot Radius", &spotRadius, 0.0f, 10.0f);
		ImGui::SliderFloat("Spot Alpha", &spotAlpha, 0.0f, 1.0f);
		ImGui::SliderFloat("Spot Beta", &spotBeta, 0.0f, 1.0f);
		ImGui::Text("");
		ImGui::SliderFloat("Point Radius", &pointRadius, 0.0f, 10.0f);
		ImGui::Text("");
		ImGui::Text("     ------Color Parameters------");
		ImGui::ColorEdit3("Directional", &directionalColor.r);
		ImGui::ColorEdit3("Point", &pointColor.r);
		ImGui::ColorEdit3("Spot", &spotColor.r);
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