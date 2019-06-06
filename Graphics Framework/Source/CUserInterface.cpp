#include "../Header/CUserInterface.h"
#include "../Header/CShaderManager.h"
#include "../Header/CScene.h"

#ifdef OPEN_GL
#include <windows.h>
#endif

CUserInterface::~CUserInterface()
{

#ifdef OPEN_GL
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
#endif

#ifdef DIRECT_X
	ImGui_ImplWin32_Shutdown();
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
	ImGui_ImplWin32_NewFrame();
#endif
#ifdef OPEN_GL
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
#endif

	ImGui::NewFrame();
}

void CUserInterface::SetFrame(ColorData& color, LightingData& light, bool& isVertex, bool& isBlinn, 
bool& isSpotOn, bool&isSpotChanged, bool& isPointOn, bool&isPointChanged)
{
	bool spot = isSpotOn;
	bool point = isPointOn;

	ImGui::Begin("Performance");
		ImGui::Text("");
		ImGui::Text("Frame Rate:			  %.1f/s", ImGui::GetIO().Framerate);
		ImGui::Text("Frame Pacing:			%.1f ms", 1000.0f / ImGui::GetIO().Framerate);
	ImGui::End();

	ImGui::Begin("Light and Color Controllers");
		ImGui::Text("     ------Shader Parameters------");
		ImGui::Text("");
		ImGui::Checkbox("Vertex Shader Light", &isVertex);
		ImGui::Checkbox("Blinn Reflection", &isBlinn);
		ImGui::Text("");
		ImGui::Text("     ------Light Parameters------");
		ImGui::Text("");
		ImGui::SliderFloat("Point Radius", &light.PointRadius, 0.0f, 10.0f);
		ImGui::SliderFloat("Spot Radius", &light.SpotRadius, 0.0f, 10.0f);
		ImGui::SliderFloat("Spot Alpha", &light.spotAlpha, 0.0f, 1.0f);
		ImGui::SliderFloat("Spot Beta", &light.spotBeta, 0.0f, 1.0f);
		ImGui::Text("");
		ImGui::Checkbox("Point Light On/Off ", &isPointOn);
		ImGui::Checkbox("Spot Light On/Off ", &isSpotOn);
		ImGui::Text("");
		ImGui::SliderFloat("Diffuse Intensity", &color.diffuseIntensity, 0.0f, 10.0f);
		ImGui::SliderFloat("Ambient Intensity", &color.ambientIntensity, 0.0f, 10.0f);
		ImGui::SliderFloat("Specular Intensity", &color.specularIntensity, 0.0f, 1.0f);
		ImGui::Text("");
		ImGui::Text("     ------Color Parameters------");
		ImGui::ColorEdit3("Diffuse Color", &color.diffuseColor.r);
		ImGui::ColorEdit3("Specular Color", &color.specularColor.r);
		ImGui::ColorEdit3("Ambient Color", &color.ambientColor.r);
		ImGui::Text("");
		ImGui::ColorEdit3("Point Color", &color.pointColor.r);
		ImGui::ColorEdit3("Spot Color", &color.spotColor.r);
	ImGui::End();

	if (spot != isSpotOn)
	{
		isSpotChanged = true;
	}
	if (point != isPointOn)
	{
		isPointChanged = true;
	}
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