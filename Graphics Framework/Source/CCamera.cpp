#include "../Header/CCamera.h"

CCamera::CCamera()
{
	m_isZoomActive = false;
}

void CCamera::Initialize(VECTOR3 position, VECTOR3 up, VECTOR3 front, VECTOR3 right, float yaw, float pitch, float speed, float rotateSpeed)
{
	m_mouseLastPosition = Point{ 0, 0 };

	m_position = position;

	m_up = up;
	m_worldUp = up;

	m_front = front;
	m_right = right;

	m_yaw = yaw;
	m_pitch = pitch;

#ifdef DIRECT_X
	m_movementSpeed = speed * 0.080f;
#else
	m_movementSpeed = speed;
#endif

	m_rotateSpeed = rotateSpeed;

	Update();
}

void CCamera::SetViewMatrix(float fieldOfView, Size aspectRatio, float nearZ, float farZ)
{
	m_fieldOfView = fieldOfView;
	m_aspectRatio = aspectRatio;
	m_near = nearZ;
	m_far = farZ;

	m_projectionMatrix = glm::perspective(m_fieldOfView, (aspectRatio.width / aspectRatio.height), nearZ, farZ);
}

void CCamera::Resize(Size aspectRatio)
{
	m_projectionMatrix = glm::perspective(m_fieldOfView, (aspectRatio.width / aspectRatio.height), m_near, m_far);
}

void CCamera::Update()
{
	VECTOR3 newFront;

	if (m_worldUp.z == 1)
	{
		newFront.x = -cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		newFront.y = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		newFront.z = sin(glm::radians(m_pitch));

		m_front = glm::normalize(newFront);
	}
	else
	{
		newFront.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		newFront.y = sin(glm::radians(m_pitch));
		newFront.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

		m_front = glm::normalize(newFront);
	}
	
	m_right = glm::normalize(glm::cross(m_front, m_worldUp));
	m_up = glm::normalize(glm::cross(m_right, m_front));

	m_viewMatrix = glm::lookAt(m_position, (m_position + m_front), m_up);
}

void CCamera::MoveForward()
{
	m_position += m_front * m_movementSpeed;
	Update();
}

void CCamera::MoveBackward()
{
	m_position -= m_front * m_movementSpeed;
	Update();
}

void CCamera::MoveLeft()
{
	m_position -= m_right * m_movementSpeed;
	Update();
}

void CCamera::MoveRight()
{
	m_position += m_right * m_movementSpeed;
	Update();
}

void CCamera::MoveUp()
{
	m_position += m_up * m_movementSpeed;
	Update();
}

void CCamera::Rotate(float x, float y)
{
	if (abs(x - m_mouseLastPosition.x) > 50)
	{
		m_mouseLastPosition.x = x;
		m_mouseLastPosition.y = y;
	}

	m_yaw += (x - m_mouseLastPosition.x) * m_rotateSpeed;
	m_pitch -= (y - m_mouseLastPosition.y) * m_rotateSpeed;

	if (m_pitch > 89.0f)
	{
		m_pitch = 89.0f;
	}

	if (m_pitch < -89.0f)
	{
		m_pitch = -89.0f;
	}

	m_mouseLastPosition.x = x;
	m_mouseLastPosition.y = y;

	Update();
}

void CCamera::Zoom()
{
	m_isZoomActive = !m_isZoomActive;

	if (m_isZoomActive)
	{
		m_fieldOfView *= 0.5f;
	}
	else
	{
		m_fieldOfView *= 2.0f;
	}

	SetViewMatrix(m_fieldOfView, m_aspectRatio, m_near, m_far);
}