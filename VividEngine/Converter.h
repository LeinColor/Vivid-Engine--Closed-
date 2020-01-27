#pragma once
#include "stdafx.h"

#include <string>

namespace vivid {
	class Converter {
	public:
		static std::string CharArrayToString(char* chars);
		static std::wstring StringToWString(std::string str);
		static const char* WCharToChar(const wchar_t* wchars);
		static const wchar_t* CharToWChar(const char* chars);
	};
}