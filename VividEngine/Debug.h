#pragma once

#include <string>

namespace vivid {
	class Debug {
	public:
		static void Log(const char* format, ...);
	};
}