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

	void KeyBoardUp(unsigned char key);
	void KeyBoardDown(unsigned char key);

#ifdef OPEN_GL
	void MouseKey(int button, int state);
#endif

#ifdef DIRECT_X
	void MouseKey(InputEvent button, bool isPressed);
#endif

	void Update(CCameraManager& camera, CScene& scene);
};