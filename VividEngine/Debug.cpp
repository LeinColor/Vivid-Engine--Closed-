#include "Debug.h"
#include "AppHandle.h"
#include "Converter.h"

#include <sstream>

using namespace vivid;

namespace vivid {
	void Debug::Log(const char *format, ...)
	{
		char buf[1024];
		va_list marker;

		va_start(marker, format);
		vsnprintf(buf, 1023, format, marker);

		std::string str = Converter::CharArrayToString(buf);
		MessageBox(AppHandle::GetWindowHandle(), Converter::StringToWString(str).c_str(), L"Debug", MB_OK);
	}
}