#pragma once
#include "Header.h"

class CUserInterface
{
public:
	void Initialize();
	void Initframe();
	void RenderFrame();
	
	void SetFrame(float vertices, float faces, float meshes, float models, bool& isVertex, bool& isBlinn,
	float& spotRadius, float& spotAlpha, float& spotBeta, float& pointRadius, Color& directionalColor,
	Color& pointColor, Color& spotColor);

	CUserInterface() = default;
	~CUserInterface();
};