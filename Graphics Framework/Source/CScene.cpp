#pragma once
#include "../Header/CLightingPass.h"
#include "../Header/CScene.h"
#include "../Header/CModel.h"

CScene::CScene() {
	m_isParty = false;

	m_isSpotChanged = false;
	m_isSpotOn = true;

	m_isPointChanged = false;
	m_isPointOn = true;
}


bool 
CScene::Initialize(std::vector <ModelData> models, CDevice& device) {
	m_models.resize(models.size());
	for (int i = 0; i < models.size(); i++) {
		m_models[i].Initialize(models[i], device);
	}
	return true;
}

void 
CScene::Render(CDeviceContext& deviceContext, CLightingPass& pass, CCamera& camera, bool isVR) {
	deviceContext.UpdateColorData(pass.m_colorDataCB, m_colorData);
	for (int i = 0; i < m_models.size(); i++) {
		m_models[i].Render(deviceContext, pass, camera, isVR);
	}
}

void 
CScene::PartyMode() {
	if (m_isParty) {
		m_colorData.diffuseColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	}
	m_isParty = !m_isParty;
}

void 
CScene::Update(float time) {
	RotateLight(time);
	if (m_isSpotChanged)
	{
		if (m_lightingData.SpotRadius > 0.0f)
		{
			m_lightingData.SpotRadius = 0.0f;
		}
		else
		{
			m_lightingData.SpotRadius = m_spotRadius;
		}
		m_isSpotChanged = false;
	}
	if (m_isPointChanged) {
		if (m_lightingData.PointRadius > 0.0f) {
			m_lightingData.PointRadius = 0.0f;
		}
		else {
			m_lightingData.PointRadius = m_pointRadius;
		}
		m_isPointChanged = false;
	}
	for (int i = 0; i < m_models.size(); i++) {
		m_models[i].Update(time);
	}
	if (m_isParty) {
		m_colorData.diffuseColor.r = sinf(time * 1.0f) + 1.0f;
		m_colorData.diffuseColor.g = cosf(time * 3.0f) + 1.0f;
		m_colorData.diffuseColor.b = sinf(time * 5.0f) + 1.0f;
	}
}

void 
CScene::RotateLight(float time) {
	m_lightingData.directional.x = sin(time);
	m_lightingData.directional.y = cos(time);
	m_lightingData.directional.z = cos(time) + sin(time);
}

void 
CScene::ChangeLightIntensity(float value)
{
	m_lightingData.specularPower += value;
	if (m_lightingData.specularPower > 1.0f) {
		m_lightingData.specularPower = 1.0f;
	}
	else if (m_lightingData.specularPower < 0.0f) {
		m_lightingData.specularPower = 0.0f;
	}
}

void 
CScene::SetColorData(ColorData data) {
	m_colorData.diffuseColor = data.diffuseColor;
	m_colorData.pointColor = data.pointColor;
	m_colorData.spotColor = data.spotColor;

	m_colorData.specularColor = data.specularColor;

#ifdef DIRECT_X
	m_colorData.ambientColor = Color{ 0.0f, 1.0f, 0.0f, 1.0f };
#else
	m_colorData.ambientColor = Color{ 0.0f, 0.0f, 1.0f, 1.0f };
#endif

	m_colorData.diffuseIntensity = data.diffuseIntensity;
	m_colorData.specularIntensity = data.specularIntensity;
	m_colorData.ambientIntensity = data.ambientIntensity;
}

void 
CScene::SetLightData(LightingData data) {
	m_lightingData.directional = data.directional;
	m_lightingData.viewPosition = data.viewPosition;
	m_lightingData.pointPosition = data.pointPosition;
	m_lightingData.spotPosition = data.spotPosition;
	m_lightingData.spotDirection = data.spotDirection;

	m_lightingData.PointRadius = data.PointRadius;
	m_lightingData.SpotRadius =	data.SpotRadius;
	m_lightingData.spotBeta = data.spotBeta;
	m_lightingData.spotAlpha =	data.spotAlpha;
	m_lightingData.specularPower = data.specularPower;
}

unsigned int 
CScene::GetNumVertices() {
	unsigned int value = 0;
	for (int i = 0; i < m_models.size(); i++) {
		value += m_models[i].GetNumVertices();
	}
	return value;
}

unsigned int 
CScene::GetNumFaces() {
	unsigned int value = 0;
	for (int i = 0; i < m_models.size(); i++) {
		value += m_models[i].GetNumFaces();
	}
	return value;
}

unsigned int 
CScene::GetNumModels() {
	unsigned int value = m_models.size();
	return value;
}

unsigned int 
CScene::GetNumMeshes() {
	unsigned int value = 0;
	for (int i = 0; i < m_models.size(); i++) {
		value += m_models[i].m_meshes.size();
	}
	return value;
}