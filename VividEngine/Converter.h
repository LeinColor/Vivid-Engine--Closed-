#pragma once
#include "stdafx.h"

#include <string>

namespace vivid {
	class Converter {
	public:
		static std::string CharArrayToString(char* chars);
		static std::wstring StringToWString(std::string str);
	};
}