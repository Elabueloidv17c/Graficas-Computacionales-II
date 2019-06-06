#pragma once
#include "Header.h"

class CUserInterface
{
public:
	void Initialize();
	void Initframe();
	void RenderFrame();
	
	void SetFrame(ColorData& color, LightingData& light, bool& isVertex, bool& isBlinn, 
	bool& isSpotOn, bool&isSpotChanged, bool& isPointOn, bool&isPointChanged);

	CUserInterface() = default;
	~CUserInterface();
};