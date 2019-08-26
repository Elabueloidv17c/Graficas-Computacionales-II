#pragma once
#include "Header.h"

class CCamera
{
public:

	MATRIX4*	m_viewMatrix;
	MATRIX4*	m_projectionMatrix;

	VECTOR3		m_position;
	VECTOR3		m_front;
	VECTOR3		m_up;
	VECTOR3		m_right;
	VECTOR3		m_worldUp;

	float		m_yaw;
	float		m_pitch;

	Point		m_mouseLastPosition;
	float		m_movementSpeed;
	float		m_rotateSpeed;

	bool		m_isZoomActive;
	float		m_fieldOfView;
	Size		m_aspectRatio;
	float		m_near;
	float		m_far;

	void		Update();

	CCamera();
	~CCamera() = default;
	
	void Initialize(VECTOR3 position, VECTOR3 up, VECTOR3 front, VECTOR3 right, float yaw, float pitch, float speed, float rotateSpeed);

	void SetViewMatrix(float fieldOfView, Size aspectRatio, float nearZ, float farZ);
	void Resize(Size aspectRatio);

	void Zoom();
	void MoveForward();
	void MoveBackward();
	void MoveLeft();
	void MoveRight();
	void MoveUp();
	void Rotate(float x, float y);
};