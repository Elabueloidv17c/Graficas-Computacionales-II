//------------------------------------------------------------------------------------------------------------------------------
//API
//------------------------------------------------------------------------------------------------------------------------------

#pragma once
#define DIRECT_X
//#define OPEN_GL

//------------------------------------------------------------------------------------------------------------------------------
//Shaders
//------------------------------------------------------------------------------------------------------------------------------

#define POINT_LIGHT
#define SPOT_LIGHT

//------------------------------------------------------------------------------------------------------------------------------
//VR
//------------------------------------------------------------------------------------------------------------------------------

#define STEAM_VR

//------------------------------------------------------------------------------------------------------------------------------
//Common utilities
//------------------------------------------------------------------------------------------------------------------------------

#include <iostream>
#include <vector>
#include <fstream>
#include <string>


//VR
#include <OpenVR/openvr.h>

//Load 3D Meshes
#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>

//Math
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/matrix_projection.hpp>

//User Interface
#include "imGUI/imgui.h"

//Type definitions
typedef unsigned char unsignedInt8;
typedef glm::vec2   VECTOR2;
typedef glm::vec3   VECTOR3;
typedef glm::vec4   VECTOR4;
typedef glm::mat4x4 MATRIX4;

//------------------------------------------------------------------------------------------------------------------------------
//Structs
//------------------------------------------------------------------------------------------------------------------------------
struct Vertex {
	VECTOR3 Position;
	VECTOR2 TexCoord;
	VECTOR3 Normals;
	VECTOR3 Tangents;
};

struct Point {
	int x;
	int y;
};

struct Size {
	float width;
	float height;
};

struct Rect {
	Point pos;
	Size size;
};

struct Color {
	float r;
	float g;
	float b;
	float a;
};

struct Vector {
	float x;
	float y;
	float z;
};

struct ModelData {
	const char* path;
	MATRIX4* transform;
	bool isHeadset;
};

enum InputEvent {
	RightMouse = 1,
	MiddleMouse = 2,
	LeftMouse = 4,
	KeyW = 8,
	KeyA = 16,
	KeyS = 32,
	KeyD = 64,
	KeyQ = 128,
	KeyE = 256,
	KeyR = 512,
	KeyF = 1024,
	KeyT = 2048,
	KeySpace = 4096,
	KeyP = 8192,
	KeyO = 16384,
	KeyI = 32768,
	KeyL = 65536,
	KeyK = 131072,
	KeyJ = 262144
};

struct LightingData {
	Vector	directional;
	float   specularPower;

	Vector	pointPosition;
	float   PointRadius;
	
	Vector	viewPosition;
	float   SpotRadius;

	Vector	spotPosition;
	float   spotAlpha;
	
	Vector	spotDirection;
	float   spotBeta;
};

struct ColorData {
	Color pointColor;
	Color spotColor;

	Color diffuseColor;
	Color specularColor;
	Color ambientColor;

	float diffuseIntensity;
	float specularIntensity;
	float ambientIntensity;

	float foo;
};

struct BrightData {
	int LevelOfDetail;
	float BloomThreshold;

	VECTOR2 foo;
};

struct BlurHData {
	Size ViewportDimensions;
	int LevelOfDetail;

	float foo;
};

struct BlurVData {
	Size ViewportDimensions;
	int LevelOfDetail;

	float foo;
};

struct AddBrightData {
	int LevelOfDetail;

	VECTOR3 foo;
};

struct ToneMapData {
	float Exposure;
	float BloomMultiplier;

	VECTOR2 foo;
};

struct ShaderData {
	const char*	filePath;
#ifdef DIRECT_X
	const char*	entryPoint;
	const char*	shaderModel;
#endif
#ifdef OPEN_GL
	int typeFlag;
#endif
	bool isVertex;
	bool isBlinn;
};

struct ShaderProgramData {
	ShaderData vertex;
	ShaderData pixel;
};

#ifdef OPEN_GL
struct LayoutElement {
	std::string Name;
};

typedef unsigned char* TextureData;

//GLEW
#include <GL/glew.h>

//SOIL
#include <SOIL2/SOIL2.h>

//ImGui
#include "imGUI/imgui_impl_glfw.h"
#include "imGUI/imgui_impl_opengl3.h" 

//GLFW
#include <GLFW/glfw3.h>

struct UniformData
{
	std::string name;
	int location;
};

struct TextureDescription
{
	int							InternalFormat;
	int							Type;
	unsigned int		SamplerID;

	Rect						BufferSize;
	int							Format;
	int							AtachmentType;

	TextureDescription()
	{
		SamplerID = 0;
		InternalFormat = 0;
		Type = 0;
	}
};
#endif
#ifdef DIRECT_X

//ImGui
#include "ImGUI/imgui_impl_dx11.h"
#include "imGUI/imgui_impl_win32.h"

#include "CStringConverter.h"
#include "Resource.h"

#include <d3d11shader.h>
#include <D3DCompiler.h>
#include <D3dx9math.h>
#include <xnamath.h>
#include <Windows.h>
#include <d3dx11.h>
#include <d3d11.h>
#include <winnt.h>

typedef D3D11_INPUT_ELEMENT_DESC LayoutElement;
typedef ID3D11Texture2D* TextureData;
#endif

struct WindowData {
	std::string title;
	Rect dimensions;
	Color color;

#ifdef DIRECT_X
	std::string className;
	WNDPROC pWndProc;
	HINSTANCE hInstance;
	INT nCmdShow;
#endif
};