#pragma once
#include "Header.h"

class CShaderProgram;
class CCamera;
class CMesh;

#ifdef DIRECT_X
#include "CDeviceContext.h"
#include "CCamera.h"
#include "CDevice.h"
#endif

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

#ifdef OPEN_GL
	bool Initialize(ModelData data);
	void Render(CShaderProgram& shaderProgram, CCamera& camera, CCamera& otherCamera);
#endif

#ifdef DIRECT_X
	bool Initialize(ModelData data, CDevice& device);
	void Render(CDeviceContext& deviceContext, CShaderProgram& shaderProgram, CCamera& camera, CCamera& otherCamera);
#endif

	void Erase();
};