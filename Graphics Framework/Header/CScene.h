#pragma once
#include "Header.h"
#include "CMesh.h"

class CLightingPass;
class CDeviceContext;
class CDevice;
class CCamera;
class CModel;

class CScene
{
public:

	std::string						m_path;
	std::vector <CModel>	m_models;

	bool									m_isParty;

	bool									m_isPointOn;
	bool									m_isPointChanged;

	bool									m_isSpotOn;
	bool									m_isSpotChanged;

	ColorData							m_colorData;
	LightingData					m_lightingData;
	float									m_spotRadius;
	float									m_pointRadius;


	void PartyMode();
	void Update(float time);
	void RotateLight(float time);
	void ChangeLightIntensity(float value);

	void SetColorData(ColorData data);
	void SetLightData(LightingData data);

	unsigned int GetNumVertices();
	unsigned int GetNumFaces();
	unsigned int GetNumModels();
	unsigned int GetNumMeshes();

	bool Initialize(std::vector <ModelData> models, CDevice& device);
	void Render(CDeviceContext& deviceContext, CLightingPass& shaderProgram, CCamera& camera, bool isVR);

	CScene();
	~CScene() = default;
};