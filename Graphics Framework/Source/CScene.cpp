#pragma once
#include "../Header/CShaderProgram.h"
#include "../Header/CScene.h"
#include "../Header/CModel.h"

CScene::CScene()
{
	m_isParty = false;

	m_colorData.diffuseColor = Color{ 1.0f, 1.0f, 1.0f, 1.0f };
	m_colorData.specularColor = Color{ 1.0f, 1.0f, 1.0f, 1.0f };

#ifdef DIRECT_X
	m_colorData.ambientColor = Color{ 0.0f, 1.0f, 0.0f, 1.0f };
#else
	m_colorData.ambientColor = Color{ 0.0f, 0.0f, 1.0f, 1.0f };
#endif

	m_colorData.diffuseIntensity = 1.0f;
	m_colorData.specularIntensity = 1.0f;
	m_colorData.ambientIntensity = 0.20f;
}

#ifdef DIRECT_X
bool CScene::Initialize(Vector directional, float specularity, std::vector <ModelData> models, CDevice& device)
{
	m_lightingData.directional = directional;
	m_lightingData.specularIntensity = specularity;

	m_models.resize(models.size());

	for (int i = 0; i < models.size(); i++)
	{
		m_models[i].Initialize(models[i], device);
	}

	return true;
}

void CScene::Render(CDeviceContext& deviceContext, CShaderProgram& shaderProgram, CCamera& camera, CCamera& otherCamera)
{
	deviceContext.UpdateColorData(shaderProgram.m_colorDataCB, m_colorData);

	for (int i = 0; i < m_models.size(); i++)
	{
		m_models[i].Render(deviceContext, shaderProgram, camera, otherCamera);
	}
}
#endif

#ifdef OPEN_GL
bool CScene::Initialize(Vector directional, float specularity, std::vector <ModelData> models)
{
	m_lightingData.directional = directional;
	m_lightingData.specularIntensity = specularity;

	m_models.resize(models.size());

	for (int i = 0; i < models.size(); i++)
	{
		m_models[i].Initialize(models[i]);
	}

	return true;
}

void CScene::Render(CShaderProgram& shaderProgram, CCamera& camera, CCamera& otheCamera)
{
	shaderProgram.UpdateLight(m_lightingData);
	shaderProgram.UpdateColor(m_colorData);

	for (int i = 0; i < m_models.size(); i++)
	{
		m_models[i].Render(shaderProgram, camera, otheCamera);
	}
}
#endif

void CScene::PartyMode()
{
	if (m_isParty)
	{
		m_colorData.diffuseColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	}

	m_isParty = !m_isParty;
}

void CScene::ChangeSpecularColor(Color color)
{
	if (m_colorData.specularColor.r >= 1.0f || m_colorData.specularColor.r <= 0.0f)
	{
		color.r = -color.r;
	}
	if (m_colorData.specularColor.g >= 1.0f || m_colorData.specularColor.g <= 0.0f)
	{
		color.g = -color.g;
	}
	if (m_colorData.specularColor.b >= 1.0f || m_colorData.specularColor.b <= 0.0f)
	{
		color.b = -color.b;
	}

	m_colorData.specularColor.r += color.r;
	m_colorData.specularColor.g += color.g;
	m_colorData.specularColor.b += color.b;
}

void CScene::Update(float time)
{
	for (int i = 0; i < m_models.size(); i++)
	{
		m_models[i].Update(time);
	}

	if (m_isParty)
	{
		m_colorData.diffuseColor.r = sinf(time * 1.0f) + 1.0f;
		m_colorData.diffuseColor.g = cosf(time * 3.0f) + 1.0f;
		m_colorData.diffuseColor.b = sinf(time * 5.0f) + 1.0f;
	}
}

void CScene::RotateLight(float time)
{
	m_lightingData.directional.x = cos(time);
	m_lightingData.directional.y = sin(time);
	m_lightingData.directional.z = cos(time) + sin(time);
}

void CScene::ChangeLightIntensity(float value)
{
	m_lightingData.specularIntensity += value;

	if (m_lightingData.specularIntensity > 1.0f)
	{
		m_lightingData.specularIntensity = 1.0f;
	}

	else if (m_lightingData.specularIntensity < 0.0f)
	{
		m_lightingData.specularIntensity = 0.0f;
	}
}

unsigned int CScene::GetNumVertices()
{
	unsigned int value = 0;

	for (int i = 0; i < m_models.size(); i++)
	{
		value += m_models[i].GetNumVertices();
	}

	return value;
}

unsigned int CScene::GetNumFaces()
{
	unsigned int value = 0;

	for (int i = 0; i < m_models.size(); i++)
	{
		value += m_models[i].GetNumFaces();
	}

	return value;
}

unsigned int CScene::GetNumModels()
{
	unsigned int value = m_models.size();

	return value;
}

unsigned int CScene::GetNumMeshes()
{
	unsigned int value = 0;

	for (int i = 0; i < m_models.size(); i++)
	{
		value += m_models[i].m_meshes.size();
	}

	return value;
}