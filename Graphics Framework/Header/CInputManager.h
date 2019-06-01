#pragma once
#include "Header.h"
#include "CCameraManager.h"
#include "CScene.h"

class CInputManager
{
	int m_MouseButton;
	int m_MouseState;	
	int m_CurrentMouseX;
	int m_CurrentMouseY;
	int m_LastMouseX;	
	int m_LastMouseY;	

public:
	
	long m_events;		

	CInputManager();
	~CInputManager();


#ifdef OPEN_GL
	void ProcessInput(GLFWwindow* window, int key, int action);
#endif

#ifdef DIRECT_X
	void KeyBoardUp(unsigned char key);
	void KeyBoardDown(unsigned char key);
	void MouseKey(InputEvent button, bool isPressed);
#endif

	void Update(CCameraManager& camera, CScene& scene, float time);
};