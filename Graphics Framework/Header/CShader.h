#pragma once
#include "Header.h"

class CShader
{
	char* m_code;
	int m_id;

	void Erase();

	bool LoadSource(const char* path);		

public:

	~CShader();
	CShader();
	
	bool Initialize(const std::string path, int typeFlag);
	int GetId();
};