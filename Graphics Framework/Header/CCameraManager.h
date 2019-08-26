#pragma once
#include "Header.h"
#include "CCamera.h"

class CCameraManager
{
public:

	unsigned int			m_activeCamera;
	std::vector <CCamera>	m_cameras;

	CCameraManager();
	~CCameraManager() = default;

	void Initialize(VECTOR3 position, VECTOR3 up, VECTOR3 front, VECTOR3 right, float yaw, float pitch, float speed, float rotateSpeed);
	void Erase();

	void AddCamera(VECTOR3 position, VECTOR3 up, VECTOR3 front, VECTOR3 right, float yaw, float pitch, float speed, float rotateSpeed);
	void SetViewMatrix(float fieldOfView, Size aspectRatio, float nearZ, float farZ);
	void Resize(Size aspectRatio);
	void ChangeCamera();

	void Rotate(float x, float y);
	void MoveForward();
	void MoveBackward();
	void MoveLeft();
	void MoveRight();
	void MoveUp();
	void Zoom();

	CCamera* GetActiveCamera();
	CCamera* GetSecundaryCamera();
};