#pragma once
#include "Header.h"
#include "CShaderProgram.h"
#include "CMesh.h"

#ifdef DIRECT_X
#include "CDeviceContext.h"
#include "CCamera.h"
#include "CDevice.h"
#endif

class CModel
{
public:

	std::vector <CMesh>			m_meshes;
	std::string					m_modelPath;

	CModel() = default;
	~CModel() = default;

	void Update(float time);
	unsigned int GetNumVertices();
	unsigned int GetNumFaces();
	void AddTransform(MATRIX4 position);

#ifdef OPEN_GL
	bool Initialize(ModelData data);
	void Render(CShaderProgram& shaderProgram, CCamera& camera, CCamera& otherCamera);
#endif

#ifdef DIRECT_X
	bool Initialize(ModelData data, CDevice& device);
	void Render(CDeviceContext& deviceContext, CShaderProgram& shaderProgram, CCamera& camera, CCamera& otherCamera);
#endif
};