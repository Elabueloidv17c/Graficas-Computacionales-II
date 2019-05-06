#pragma once
#include "../Header/CScene.h"

#ifdef DIRECT_X
bool CScene::Initialize(std::vector <ModelData> models, CDevice& device)
{
	m_models.resize(models.size());

	for (int i = 0; i < models.size(); i++)
	{
		m_models[i].Initialize(models[i], device);
	}

	return true;
}

void CScene::Render(CDeviceContext& deviceContext, CShaderProgram& shaderProgram, CCamera& camera, CCamera& otherCamera)
{
	for (int i = 0; i < m_models.size(); i++)
	{
		m_models[i].Render(deviceContext, shaderProgram, camera, otherCamera);
	}
}
#endif

#ifdef OPEN_GL
bool CScene::Initialize(std::vector <ModelData> models)
{
	m_models.resize(models.size());

	for (int i = 0; i < models.size(); i++)
	{
		m_models[i].Initialize(models[i]);
	}

	return true;
}

void CScene::Render(CShaderProgram& shaderProgram, CCamera& camera, CCamera& otheCamera)
{
	for (int i = 0; i < m_models.size(); i++)
	{
		m_models[i].Render(shaderProgram, camera, otheCamera);
	}
}
#endif

void CScene::Update(float time)
{
	for (int i = 0; i < m_models.size(); i++)
	{
		m_models[i].Update(time);
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