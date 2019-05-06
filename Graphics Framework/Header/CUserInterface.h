#pragma once
#include "Header.h"

class CUserInterface
{
public:
	void Initialize();
	void Initframe();
	void RenderFrame();
	
	void SetFrame(float vertices, float faces, float meshes, float models);

	CUserInterface() = default;
	~CUserInterface();
};