#include "../Header/CInputManager.h"

CInputManager::CInputManager()
{
	m_MouseButton = 0;
	m_MouseState = 0;

	m_CurrentMouseX = 0;
	m_CurrentMouseY = 0;
	
	m_LastMouseX = 0;
	m_LastMouseY = 0;

	m_events = 0;
}

CInputManager::~CInputManager()
{

}

void CInputManager::KeyBoardUp(unsigned char key)
{
	// Move forward
	if (key == 'w' || key == 'W' && (m_events & InputEvent::KeyW))
	{
		m_events &= ~(InputEvent::KeyW);
		std::cout << "W key up" << std::endl;
	}

	// Move left
	if (key == 'a' || key == 'A' && (m_events & InputEvent::KeyA))
	{
		m_events &= ~(InputEvent::KeyA);
		std::cout << "A key up" << std::endl;
	}

	// Move bacward
	if (key == 's' || key == 'S' && (m_events & InputEvent::KeyS))
	{
		m_events &= ~(InputEvent::KeyS);
		std::cout << "S key up" << std::endl;
	}

	// Move right
	if (key == 'd' || key == 'D' && (m_events & InputEvent::KeyD))
	{
		m_events &= ~(InputEvent::KeyD);
		std::cout << "D key up" << std::endl;
	}

	// Move up
	if (key == ' ' && (m_events & InputEvent::KeySpace))
	{
		m_events &= ~(InputEvent::KeySpace);
		std::cout << "Space bar up" << std::endl;
	}

	// Set Scale
	if (key == 'e' || key == 'E' && (m_events & InputEvent::KeyE))
	{
		m_events &= ~(InputEvent::KeyE);
		std::cout << "E key up" << std::endl;
	}

	// Set Rotation
	if (key == 'q' || key == 'Q' && (m_events & InputEvent::KeyQ))
	{
		m_events &= ~(InputEvent::KeyQ);
		std::cout << "Q key up" << std::endl;
	}

	// Set Camera
	if (key == 'r' || key == 'R' && (m_events & InputEvent::KeyR))
	{
		m_events &= ~(InputEvent::KeyR);
		std::cout << "Q key up" << std::endl;
	}

	// Set Camera
	if (key == 'f' || key == 'F' && (m_events & InputEvent::KeyF))
	{
		m_events &= ~(InputEvent::KeyF);
		std::cout << "F key up" << std::endl;
	}

	// Set Zoom
	if (key == 'f' || key == 'F' && (m_events & InputEvent::KeyT))
	{
		m_events &= ~(InputEvent::KeyT);
		std::cout << "T key up" << std::endl;
	}
}

void CInputManager::KeyBoardDown(unsigned char key)
{
	// Move forward
	if (key == 'w' || key == 'W')
	{
		m_events |= InputEvent::KeyW;
		std::cout << "W key down" << std::endl;
	}

	// Move left
	if (key == 'a' || key == 'A')
	{
		m_events |= InputEvent::KeyA;
		std::cout << "A key down" << std::endl;
	}

	// Move bacward
	if (key == 's' || key == 'S')
	{
		m_events |= InputEvent::KeyS;
		std::cout << "S key down" << std::endl;
	}

	// Move right
	if (key == 'd' || key == 'D')
	{
		m_events |= InputEvent::KeyD;
		std::cout << "D key down" << std::endl;
	}

	// Move up
	if (key == ' ')
	{
		m_events |= InputEvent::KeySpace;
		std::cout << "Space bar down" << std::endl;
	}

	// Set Rotation
	if (key == 'q' || key == 'Q')
	{
		m_events |= InputEvent::KeyQ;
		std::cout << "Q key down" << std::endl;
	}

	// Set Scale
	if (key == 'e' || key == 'E')
	{
		m_events |= InputEvent::KeyE;
		std::cout << "E key down" << std::endl;
	}

	// Set Camera
	if (key == 'r' || key == 'R')
	{
		m_events |= InputEvent::KeyR;
		std::cout << "R key down" << std::endl;
	}

	// Set Party Mode
	if (key == 'f' || key == 'F')
	{
		m_events |= InputEvent::KeyF;
		std::cout << "F key down" << std::endl;
	}

	// Set Zoom
	if (key == 't' || key == 'T')
	{
		m_events |= InputEvent::KeyT;
		std::cout << "T key down" << std::endl;
	}
}

void CInputManager::Update(CCameraManager& camera, CScene& scene)
{
	if (m_events & InputEvent::KeyW)
	{
		camera.MoveForward();
	}
	if (m_events & InputEvent::KeyS)
	{
		camera.MoveBackward();
	}
	if (m_events & InputEvent::KeyD)
	{
		camera.MoveRight();
	}
	if (m_events & InputEvent::KeyA)
	{
		camera.MoveLeft();
	}
	if (m_events & InputEvent::KeyR)
	{
		camera.ChangeCamera();
		m_events &= ~(InputEvent::KeyR);
	}
	if (m_events & InputEvent::KeyT)
	{
		camera.Zoom();
		m_events &= ~(InputEvent::KeyT);
	}
	if (m_events & InputEvent::KeySpace)
	{
		camera.MoveUp();
	}
	if (m_events & InputEvent::KeyE)
	{
		for (int i = 0; i < scene.m_models.size(); i++)
		{
			for (int j = 0; j < scene.m_models[i].m_meshes.size(); j++)
			{
				scene.m_models[i].m_meshes[j].ChangeScale();
			}
		}

		m_events &= ~(InputEvent::KeyE);
	}
	if (m_events & InputEvent::KeyF)
	{
		for (int i = 0; i < scene.m_models.size(); i++)
		{
			for (int j = 0; j < scene.m_models[i].m_meshes.size(); j++)
			{
				scene.m_models[i].m_meshes[j].PartyMode();
			}
		}

		m_events &= ~(InputEvent::KeyF);
	}
	if (m_events & InputEvent::KeyQ)
	{
		for (int i = 0; i < scene.m_models.size(); i++)
		{
			for (int j = 0; j < scene.m_models[i].m_meshes.size(); j++)
			{
				scene.m_models[i].m_meshes[j].Rotate(50.0f);
			}
		}
	}
}

#ifdef OPEN_GL
void CInputManager::MouseKey(int button, int state)
{
	// Left down
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		m_events |= InputEvent::LeftMouse;
		std::cout << "Left mouse button down" << std::endl;
	}

	// Left Up
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		m_events &= ~(InputEvent::LeftMouse);
		std::cout << "Left mouse button up" << std::endl;
	}

	// Right down
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		m_events |= InputEvent::RightMouse;
		std::cout << "Right mouse button down" << std::endl;
	}

	// Right Up
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
	{
		m_events &= ~(InputEvent::RightMouse);
		std::cout << "Right mouse button Up" << std::endl;
	}

	// Mid down
	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
	{
		m_events |= InputEvent::MiddleMouse;
		std::cout << "Middle mouse button down" << std::endl;
	}

	// Mid up
	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_UP)
	{
		m_events &= ~(InputEvent::MiddleMouse);
		std::cout << "Middle mouse button up" << std::endl;
	}
}
#endif

#ifdef DIRECT_X
void CInputManager::MouseKey(InputEvent button, bool isPressed)
{
	// Left down
	if (button == InputEvent::LeftMouse && isPressed)
	{
		m_events |= InputEvent::LeftMouse;
		std::cout << "Right mouse button down" << std::endl;
	}

	// Left up
	if (button == InputEvent::LeftMouse && !isPressed)
	{
		m_events &= ~(InputEvent::LeftMouse);
		std::cout << "Right mouse button up" << std::endl;
	}
}
#endif