#pragma once
#include "Header.h"
#include "CModel.h"

class CScene
{
public:

	std::vector <CModel> m_models;

	void Update(float time);
	unsigned int GetNumVertices();
	unsigned int GetNumFaces();
	unsigned int GetNumModels();
	unsigned int GetNumMeshes();

#ifdef OPEN_GL
	bool Initialize(std::vector <ModelData> models);
	void Render(CShaderProgram& shaderProgram, CCamera& camera, CCamera& otheCamera);
#endif

#ifdef DIRECT_X
	bool Initialize(std::vector <ModelData> models, CDevice& device);
	void Render(CDeviceContext& deviceContext, CShaderProgram& shaderProgram, CCamera& camera, CCamera& otheCamera);
#endif

	CScene() = default;
	~CScene() = default;
};