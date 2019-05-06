#include "../Header/CStringConverter.h"

std::wstring CStringConverter::StringToWide(std::string str)
{
	std::wstring wide_string(str.begin(), str.end());

	return wide_string;
}


#ifdef DIRECT_X
std::string CStringConverter::WideToString(std::wstring wstr)
{
	int size_needed = WideCharToMultiByte(0, 0, wstr.c_str(), int(wstr.length() + 1), 0, 0, 0, 0);

	std::string str(size_needed, 0);
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), int(wstr.length() + 1), &str[0], size_needed, 0, 0);

	return str;
}
#endif