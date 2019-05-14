#pragma once
#include "../Header/Header.h"

class CStringConverter
{
public:

	static std::wstring StringToWide(std::string str);

#ifdef DIRECT_X
	static std::string WideToString(std::wstring wstr);
#endif
};