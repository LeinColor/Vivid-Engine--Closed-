#pragma once
#include "Scene.h"
#include "Components.h"

class Importer {
public:
	static MeshComponent& LoadObjFile(const char* fileName, Scene* scene);
	static ShaderComponent& LoadShaderFile(const std::string& fileName, INPUT_LAYOUT_TYPE inputLayoutTypeValue, Scene* scene);
};