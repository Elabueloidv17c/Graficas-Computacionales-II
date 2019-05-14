#pragma once

//--------------------------------------------------------------------------------------------------------------------------------
//#define DIRECT_X
#define OPEN_GL
//--------------------------------------------------------------------------------------------------------------------------------

//Common utilities
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

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
#include "imGUI/imgui_impl_win32.h"
 
//Type definitions
typedef glm::vec2   VECTOR2;
typedef glm::vec3   VECTOR3;
typedef glm::vec4   VECTOR4;
typedef glm::mat4x4 MATRIX4;

//Structs
struct Vertex
{
	VECTOR3 Position;
	VECTOR2 TexCoord;
	VECTOR3 Normals;
	VECTOR3 Tangents;
};

struct Point
{
	int x;
	int y;
};

struct Size
{
	float width;
	float height;
};

struct Rect
{
	Point pos;
	Size size;
};

struct Color
{
	float r;
	float g;
	float b;
	float a;
};

struct Vector
{
	float x;
	float y;
	float z;
};

struct ModelData
{
	const char* path;
	MATRIX4 transform;
};

enum InputEvent
{
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

struct LightingData
{
	Vector	directional;
	float   specularIntensity;
};

struct ColorData
{
	Color diffuseColor;
	Color specularColor;
	Color ambientColor;
	float diffuseIntensity;
	float specularIntensity;
	float ambientIntensity;
};

#ifdef OPEN_GL
//GLEW
#include <GL/glew.h>

//GLUT
#include <GL/glut.h>

typedef unsigned char* TextureData;

//SOIL
#include <SOIL2/SOIL2.h>

//ImGui
#include "../Header/imgui_impl_opengl3.h"

struct TextureDescription
{
	int					InternalFormat;
	int					Type;
	unsigned int		SamplerID;

	Rect				BufferSize;
	int					Format;
	int					AtachmentType;

	TextureDescription()
	{
		SamplerID = 0;
		InternalFormat = 0;
		Type = 0;
	}
};

struct SwapChainData
{
	TextureDescription	TextureDataInit;

	bool depthBuffer;
	bool stencilBuffer;

	int depthID;

	int numTagets;
	unsigned int targetID[4];

	SwapChainData()
	{
		depthBuffer = true;
		stencilBuffer = true;

		depthID = 0;

		targetID[0] = 0;
		targetID[1] = 0;
		targetID[2] = 0;
		targetID[3] = 0;
	}
};
#endif

#ifdef DIRECT_X
//#define D3DReflect

//ImGui
#include "ImGUI/imgui_impl_dx11.h"

#include "CStringConverter.h"
#include "Resource.h"

#include <Windows.h>

#include <D3DCompiler.h>
#include <D3dx9math.h>
#include <xnamath.h>
#include <d3dx11.h>
#include <d3d11.h>
#include <winnt.h>

typedef ID3D11Texture2D* TextureData;
#endif