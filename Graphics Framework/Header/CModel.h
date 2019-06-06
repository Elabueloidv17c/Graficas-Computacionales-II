#pragma once
#include "Header.h"

class CShaderProgram;
class CMesh;

#include "CDeviceContext.h"
#include "CCamera.h"
#include "CDevice.h"

class CModel
{
public:

	std::vector <MATRIX4>		m_transforms;
	bool						m_isRotating;
	float						m_scale;

	std::vector <CMesh>			m_meshes;
	std::string					m_modelPath;

	CModel();
	~CModel() = default;

	void SetTransform(MATRIX4 transform, unsigned int index);
	void Update(float time);
	void Rotate(float speed);
	void ChangeScale();
	
	unsigned int GetNumVertices();
	unsigned int GetNumFaces();
	void AddTransform(MATRIX4 transform);

	bool Initialize(ModelData data, CDevice& device);
	void Render(CDeviceContext& deviceContext, CShaderProgram& shaderProgram, CCamera& camera, CCamera& otherCamera);

	void Erase();
};