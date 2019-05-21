#include "..\Header\CCameraManager.h"

//Initialize the main camera 
void CCameraManager::Initialize(VECTOR3 position, VECTOR3 up, VECTOR3 front, VECTOR3 right, float yaw, float pitch, float speed, float rotateSpeed)
{
	if (m_cameras.size() == 0)
	{
		m_cameras.push_back(CCamera());
		m_cameras[0].Initialize(position, up, front, right, yaw, pitch, speed, rotateSpeed);
		m_activeCamera = 0;
	}

	else
	{
		std::cout << "Warning:  Camera manager already initialized" << std::endl;
	}
}

void CCameraManager::AddCamera(VECTOR3 position, VECTOR3 up, VECTOR3 front, VECTOR3 right, float yaw, float pitch, float speed, float rotateSpeed)
{
	m_cameras.push_back(CCamera());
	m_activeCamera = m_cameras.size() - 1;
	m_cameras[m_activeCamera].Initialize(position, up, front, right, yaw, pitch, speed, rotateSpeed);
}

void CCameraManager::Erase()
{
	m_cameras.clear();
}

void CCameraManager::ChangeCamera()
{
	m_activeCamera++;

	if (m_activeCamera == m_cameras.size())
	{
		m_activeCamera = 0;
	}
}

void CCameraManager::SetViewMatrix(float fieldOfView, Size aspectRatio, float nearZ, float farZ)
{
	for (int i = 0; i < m_cameras.size(); i++)
	{
		m_cameras[i].SetViewMatrix(fieldOfView, aspectRatio, nearZ, farZ);
	}
}

void CCameraManager::Resize(Size aspectRatio)
{
	for (int i = 0; i < m_cameras.size(); i++)
	{
		m_cameras[i].Resize(aspectRatio);
	}
}

void CCameraManager::Zoom()
{
	for (int i = 0; i < m_cameras.size(); i++)
	{
		m_cameras[i].Zoom();
	}
}

void CCameraManager::MoveForward()
{
	m_cameras[m_activeCamera].MoveForward();
}

void CCameraManager::MoveBackward()
{
	m_cameras[m_activeCamera].MoveBackward();
}

void CCameraManager::MoveLeft()
{
	m_cameras[m_activeCamera].MoveLeft();
}

void CCameraManager::MoveRight()
{
	m_cameras[m_activeCamera].MoveRight();
}

void CCameraManager::MoveUp()
{
	m_cameras[m_activeCamera].MoveUp();
}

void CCameraManager::Rotate(float x, float y)
{
	m_cameras[m_activeCamera].Rotate(x, y);
}

CCamera* CCameraManager::GetActiveCamera()
{
	return &m_cameras[m_activeCamera];
}

CCamera* CCameraManager::GetSecundaryCamera()
{
	if (m_activeCamera == 1)
	{
		return &m_cameras[0];
	}
	else
	{
		return &m_cameras[1];
	}
}