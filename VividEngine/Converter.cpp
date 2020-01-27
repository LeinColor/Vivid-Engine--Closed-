#include "Converter.h"
#include "Debug.h"

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

	const char* Converter::WCharToChar(const wchar_t* wchars)
	{
		char* pStr;

		int strSize = WideCharToMultiByte(CP_ACP, 0, wchars, -1, nullptr, 0, nullptr, nullptr);
		pStr = new char[strSize];

		WideCharToMultiByte(CP_ACP, 0, wchars, -1, pStr, strSize, nullptr, nullptr);
		return pStr;
	}

	const wchar_t* Converter::CharToWChar(const char* chars)
	{
		wchar_t* pStr;

		int strSize = MultiByteToWideChar(CP_ACP, 0, chars, -1, nullptr, NULL);
		pStr = new WCHAR[strSize];
		MultiByteToWideChar(CP_ACP, 0, chars, strlen(chars) + 1, pStr, strSize);
		return pStr;
	}
}