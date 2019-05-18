#pragma once
#include "Header.h"
#include "CMesh.h"

class CShaderProgram;
class CDeviceContext;
class CDevice;
class CCamera;
class CModel;

class CScene
{
public:

	bool					m_isParty;
	ColorData				m_colorData;
	LightingData			m_lightingData;

	std::vector <CModel>	m_models;

	void PartyMode();
	void Update(float time);
	void RotateLight(float time);
	void ChangeSpecularColor(Color color);
	void ChangeLightIntensity(float value);

	void SetColorData(ColorData data);
	void SetLightData(LightingData data);

	unsigned int GetNumVertices();
	unsigned int GetNumFaces();
	unsigned int GetNumModels();
	unsigned int GetNumMeshes();

#ifdef DIRECT_X
	bool Initialize(std::vector <ModelData> models, CDevice& device);
	void Render(CDeviceContext& deviceContext, CShaderProgram& shaderProgram, CCamera& camera, CCamera& otheCamera);
#else
	bool Initialize(std::vector <ModelData> models);
	void Render(CShaderProgram& shaderProgram, CCamera& camera, CCamera& otheCamera);
#endif

	CScene();
	~CScene() = default;
};