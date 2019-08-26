#pragma once
#include "Header.h"

class CTexture;
class CScreenAlignedQuad;
class CScene;

class CUserInterface
{
	CScene*										m_scene;
	CScreenAlignedQuad*				m_quad;

public:
	void Initialize(CScene* scene, CScreenAlignedQuad* quad);
	void Initframe();
	void RenderFrame();
	
#ifdef STEAM_VR
	void SetHeadsetEye(CTexture& texture);
#endif
	void SetFrame(bool& isVertex, bool& isBlinn, bool& isSpotOn, bool&isSpotChanged, bool& isPointOn, bool&isPointChanged);

	CUserInterface() = default;
	~CUserInterface();
};