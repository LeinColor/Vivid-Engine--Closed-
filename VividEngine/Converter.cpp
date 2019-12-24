#include "Converter.h"

namespace vivid {
	std::string Converter::CharArrayToString(char* chars)
	{
		std::string result(chars);
		return result;
	}

	std::wstring Converter::StringToWString(std::string str)
	{
		int len;
		int slength = static_cast<int>(str.length()) + 1;
		len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), slength, 0, 0);

		wchar_t* buf = new wchar_t[len];
		MultiByteToWideChar(CP_ACP, 0, str.c_str(), slength, buf, len);
		std::wstring r(buf);
		delete[] buf;
		return r;
	}
}