#include "../Header/CScreenAlignedQuad.h"
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

void CUserInterface::Initialize(CScene* scene, CScreenAlignedQuad* quad)
{
	m_quad = quad;
	m_scene = scene;

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
#ifdef STEAM_VR
void CUserInterface::SetHeadsetEye(CTexture& texture)
{
	ImGui::Begin("Headset");
#ifdef OPEN_GL
	ImGui::Image((void*)(GLuint)texture.m_id, ImVec2(texture.m_width / 6, texture.m_height / 6), ImVec2(0,1), ImVec2(1,0));
#endif
#ifdef DIRECT_X
	ImGui::Image((void*)(ID3D11ShaderResourceView*) texture.m_pointer, ImVec2(texture.m_width / 6, texture.m_height / 6));
#endif
	ImGui::End();
}
#endif

void CUserInterface::SetFrame(bool& isVertex, bool& isBlinn, bool& isSpotOn, bool&isSpotChanged, bool& isPointOn, bool&isPointChanged)
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
		ImGui::SliderFloat("Point Radius", &m_scene->m_lightingData.PointRadius, 0.0f, 10.0f);
		ImGui::SliderFloat("Spot Radius", &m_scene->m_lightingData.SpotRadius, 0.0f, 10.0f);
		ImGui::SliderFloat("Spot Alpha", &m_scene->m_lightingData.spotAlpha, 0.0f, 1.0f);
		ImGui::SliderFloat("Spot Beta", &m_scene->m_lightingData.spotBeta, 0.0f, 1.0f);
		ImGui::Text("");
		ImGui::Checkbox("Point Light On/Off ", &isPointOn);
		ImGui::Checkbox("Spot Light On/Off ", &isSpotOn);
		ImGui::Text("");
		ImGui::SliderFloat("Diffuse Intensity", &m_scene->m_colorData.diffuseIntensity, 0.0f, 10.0f);
		ImGui::SliderFloat("Ambient Intensity", &m_scene->m_colorData.ambientIntensity, 0.0f, 10.0f);
		ImGui::SliderFloat("Specular Intensity", &m_scene->m_colorData.specularIntensity, 0.0f, 1.0f);
		ImGui::Text("");
		ImGui::Text("     ------Color Parameters------");
		ImGui::ColorEdit3("Diffuse Color", &m_scene->m_colorData.diffuseColor.r);
		ImGui::ColorEdit3("Specular Color", &m_scene->m_colorData.specularColor.r);
		ImGui::ColorEdit3("Ambient Color", &m_scene->m_colorData.ambientColor.r);
		ImGui::Text("");
		ImGui::ColorEdit3("Point Color", &m_scene->m_colorData.pointColor.r);
		ImGui::ColorEdit3("Spot Color", &m_scene->m_colorData.spotColor.r);
	ImGui::End();

	//ImGui::Begin("Post Processing parameters");
	//	ImGui::SliderInt("Horizontal Blur LOD", &m_quad->m_blurHData.LevelOfDetail, 0, 10);
	//	ImGui::SliderInt("Vertical Blur LOD", &m_quad->m_blurVData.LevelOfDetail, 0, 10);
	//	ImGui::Text("");
	//	ImGui::SliderInt("Bright LOD", &m_quad->m_brightData.LevelOfDetail, 0, 10);
	//	ImGui::SliderFloat("Bloom Threshold", &m_quad->m_brightData.BloomThreshold, 0.0f, 10.0f);
	//	ImGui::Text("");
	//	ImGui::SliderInt("Add Bright LOD", &m_quad->m_addBrightData.LevelOfDetail, 0, 10);
	//	ImGui::Text("");
	//	ImGui::SliderFloat("Bloom Multiplier", &m_quad->m_toneMapData.BloomMultiplier, 0.0f, 50.0f);
	//	ImGui::SliderFloat("Exposure", &m_quad->m_toneMapData.Exposure, 0.0f, 10.0f);
	//ImGui::End();

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