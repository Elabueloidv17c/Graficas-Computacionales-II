#include "../Header/CInputManager.h"
#include "../Header/CModel.h"

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

void CInputManager::Update(CCameraManager& camera, CScene& scene, float time)
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
	if (m_events & InputEvent::KeyP)
	{
		scene.RotateLight(time);
	}
	if (m_events & InputEvent::KeyO)
	{
		scene.ChangeLightIntensity(0.01f);
	}
	if (m_events & InputEvent::KeyI)
	{
		scene.ChangeLightIntensity(-0.01f);
	}
	if (m_events & InputEvent::KeyJ)
	{

	}
	if (m_events & InputEvent::KeyK)
	{
		if (scene.m_lightingData.SpotRadius > 0.0f)
		{
			scene.m_lightingData.SpotRadius = 0.0f;
		}
		else
		{
			scene.m_lightingData.SpotRadius = scene.m_spotRadius;
		}
		m_events &= ~(InputEvent::KeyK);
	}
	if (m_events & InputEvent::KeyL)
	{
		if (scene.m_lightingData.PointRadius > 0.0f)
		{
			scene.m_lightingData.PointRadius = 0.0f;
		}
		else
		{
			scene.m_lightingData.PointRadius = scene.m_pointRadius;
		}
		m_events &= ~(InputEvent::KeyL);
	}
	if (m_events & InputEvent::KeyE)
	{
		for (int i = 0; i < scene.m_models.size(); i++)
		{
			scene.m_models[i].ChangeScale();
		}

		m_events &= ~(InputEvent::KeyE);
	}
	if (m_events & InputEvent::KeyF)
	{
		scene.PartyMode();

		m_events &= ~(InputEvent::KeyF);
	}
	if (m_events & InputEvent::KeyQ)
	{
		for (int i = 0; i < scene.m_models.size(); i++)
		{
			scene.m_models[i].Rotate(50.0f);
		}
	}
}

#ifdef OPEN_GL
void CInputManager::ProcessInput(GLFWwindow* window, int key, int action)
{
	//--------------------------------------------------------------------
	//Keyboard 
	//--------------------------------------------------------------------

	// Move forward
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		m_events |= InputEvent::KeyW;
		std::cout << "W key down" << std::endl;
	}
	if (key == GLFW_KEY_W && action == GLFW_RELEASE)
	{
		m_events &= ~(InputEvent::KeyW);
		std::cout << "W key up" << std::endl;
	}

	// Move left
	if (key == GLFW_KEY_A && action == GLFW_PRESS)
	{
		m_events |= InputEvent::KeyA;
		std::cout << "A key down" << std::endl;
	}
	if (key == GLFW_KEY_A && action == GLFW_RELEASE)
	{
		m_events &= ~(InputEvent::KeyA);
		std::cout << "A key up" << std::endl;
	}

	// Move bacward
	if (key == GLFW_KEY_S && action == GLFW_PRESS)
	{
		m_events |= InputEvent::KeyS;
		std::cout << "S key down" << std::endl;
	}
	if (key == GLFW_KEY_S && action == GLFW_RELEASE)
	{
		m_events &= ~(InputEvent::KeyS);
		std::cout << "S key up" << std::endl;
	}

	// Move right
	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		m_events |= InputEvent::KeyD;
		std::cout << "D key down" << std::endl;
	}
	if (key == GLFW_KEY_D && action == GLFW_RELEASE)
	{
		m_events &= ~(InputEvent::KeyD);
		std::cout << "D key up" << std::endl;
	}

	// Move up
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		m_events |= InputEvent::KeySpace;
		std::cout << "Space bar down" << std::endl;
	}
	if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
	{
		m_events &= ~(InputEvent::KeySpace);
		std::cout << "Space bar up" << std::endl;
	}

	// Set Scale
	if (key == GLFW_KEY_E && action == GLFW_PRESS)
	{
		m_events |= InputEvent::KeyE;
		std::cout << "E key down" << std::endl;
	}
	if (key == GLFW_KEY_E && action == GLFW_RELEASE)
	{
		m_events &= ~(InputEvent::KeyE);
		std::cout << "E key up" << std::endl;
	}

	// Set Rotation
	if (key == GLFW_KEY_Q && action == GLFW_PRESS)
	{
		m_events |= InputEvent::KeyQ;
		std::cout << "Q key down" << std::endl;
	}
	if (key == GLFW_KEY_Q && action == GLFW_RELEASE)
	{
		m_events &= ~(InputEvent::KeyQ);
		std::cout << "Q key up" << std::endl;
	}

	// Set Camera
	if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		m_events |= InputEvent::KeyR;
		std::cout << "R key down" << std::endl;
	}
	if (key == GLFW_KEY_R && action == GLFW_RELEASE)
	{
		m_events &= ~(InputEvent::KeyR);
		std::cout << "Q key up" << std::endl;
	}

	// Set Camera
	if (key == GLFW_KEY_F && action == GLFW_PRESS)
	{
		m_events |= InputEvent::KeyF;
		std::cout << "F key down" << std::endl;
	}
	if (key == GLFW_KEY_F && action == GLFW_RELEASE)
	{
		m_events &= ~(InputEvent::KeyF);
		std::cout << "F key up" << std::endl;
	}

	// Set Zoom
	if (key == GLFW_KEY_T && action == GLFW_PRESS)
	{
		m_events |= InputEvent::KeyT;
		std::cout << "T key down" << std::endl;
	}
	if (key == GLFW_KEY_T && action == GLFW_RELEASE)
	{
		m_events &= ~(InputEvent::KeyT);
		std::cout << "T key up" << std::endl;
	}

	//Change Light Direction
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		m_events |= InputEvent::KeyP;
		std::cout << "P key down" << std::endl;
	}
	if (key == GLFW_KEY_P && action == GLFW_RELEASE)
	{
		m_events &= ~(InputEvent::KeyP);
		std::cout << "P key up" << std::endl;
	}

	//Increase Reflectibity
	if (key == GLFW_KEY_O && action == GLFW_PRESS)
	{
		m_events |= InputEvent::KeyO;
		std::cout << "O key down" << std::endl;
	}
	if (key == GLFW_KEY_O && action == GLFW_RELEASE)
	{
		m_events &= ~(InputEvent::KeyO);
		std::cout << "O key up" << std::endl;
	}

	//Decrease Reflectibity
	if (key == GLFW_KEY_I && action == GLFW_PRESS)
	{
		m_events |= InputEvent::KeyI;
		std::cout << "I key down" << std::endl;
	}
	if (key == GLFW_KEY_I && action == GLFW_RELEASE)
	{
		m_events &= ~(InputEvent::KeyI);
		std::cout << "I key up" << std::endl;
	}

	//Unused
	if (key == GLFW_KEY_K && action == GLFW_PRESS)
	{
		m_events |= InputEvent::KeyK;
		std::cout << "K key down" << std::endl;
	}
	if (key == GLFW_KEY_K && action == GLFW_RELEASE)
	{
		m_events &= ~(InputEvent::KeyK);
		std::cout << "K key up" << std::endl;
	}

	//Increase Reflectibity
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		m_events |= InputEvent::KeyL;
		std::cout << "L key down" << std::endl;
	}
	if (key == GLFW_KEY_L && action == GLFW_RELEASE)
	{
		m_events &= ~(InputEvent::KeyL);
		std::cout << "L key up" << std::endl;
	}

	//Decrease Reflectibity
	if (key == GLFW_KEY_J && action == GLFW_PRESS)
	{
		m_events |= InputEvent::KeyJ;
		std::cout << "J key down" << std::endl;
	}
	if (key == GLFW_KEY_J && action == GLFW_RELEASE)
	{
		m_events &= ~(InputEvent::KeyJ);
		std::cout << "J key up" << std::endl;
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

	// Right down
	if (button == InputEvent::RightMouse && isPressed)
	{
		m_events |= InputEvent::RightMouse;
		std::cout << "Right mouse button down" << std::endl;
	}

	// Righ up
	if (button == InputEvent::RightMouse && !isPressed)
	{
		m_events &= ~(InputEvent::RightMouse);
		std::cout << "Right mouse button up" << std::endl;
	}
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

	//Change Light Direction
	if (key == 'p' || key == 'P' && (m_events & InputEvent::KeyP))
	{
		m_events &= ~(InputEvent::KeyP);
		std::cout << "P key up" << std::endl;
	}

	//Increase Reflectibity
	if (key == 'o' || key == 'O' && (m_events & InputEvent::KeyO))
	{
		m_events &= ~(InputEvent::KeyO);
		std::cout << "O key up" << std::endl;
	}

	//Decrease Reflectibity
	if (key == 'i' || key == 'I' && (m_events & InputEvent::KeyI))
	{
		m_events &= ~(InputEvent::KeyI);
		std::cout << "I key up" << std::endl;
	}

	if (key == 'k' || key == 'K' && (m_events & InputEvent::KeyK))
	{
		m_events &= ~(InputEvent::KeyK);
		std::cout << "K key up" << std::endl;
	}

	//Increase Reflectibity
	if (key == 'l' || key == 'L' && (m_events & InputEvent::KeyL))
	{
		m_events &= ~(InputEvent::KeyL);
		std::cout << "L key up" << std::endl;
	}

	//Decrease Reflectibity
	if (key == 'j' || key == 'J' && (m_events & InputEvent::KeyJ))
	{
		m_events &= ~(InputEvent::KeyJ);
		std::cout << "J key up" << std::endl;
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

	// Rotate Light
	if (key == 'p' || key == 'P')
	{
		m_events |= InputEvent::KeyP;
		std::cout << "P key down" << std::endl;
	}

	// Increase light intensity
	if (key == 'o' || key == 'O')
	{
		m_events |= InputEvent::KeyO;
		std::cout << "O key down" << std::endl;
	}

	// Reduce Light Intensity
	if (key == 'i' || key == 'I')
	{
		m_events |= InputEvent::KeyI;
		std::cout << "I key down" << std::endl;
	}

	// Change Red Channel
	if (key == 'j' || key == 'J')
	{
		m_events |= InputEvent::KeyJ;
		std::cout << "J key down" << std::endl;
	}

	// Change Green Channel
	if (key == 'k' || key == 'K')
	{
		m_events |= InputEvent::KeyK;
		std::cout << "O key down" << std::endl;
	}

	// Change Blue Channel
	if (key == 'l' || key == 'L')
	{
		m_events |= InputEvent::KeyL;
		std::cout << "L key down" << std::endl;
	}
}
#endif