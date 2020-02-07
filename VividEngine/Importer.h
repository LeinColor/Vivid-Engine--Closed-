#pragma once
#include "Scene.h"

class Importer {
public:
	static uint32_t LoadObjFile(const std::string name, const char* fileName);
	static uint32_t LoadShaderFile(const std::string name, const std::string& fileName, const INPUT_LAYOUT_TYPE inputLayoutTypeValue);
	static uint32_t LoadTextureFile(const std::string name, const WCHAR* fileName);
};