#pragma once
#include "stdafx.h"

#include <vector>

class Object;
class Mesh;
class Shader;
class Texture;
class Scene {
public:
	static std::vector<Object*> objects;
	static std::vector<Mesh*> meshes;
	static std::vector<Shader*> shaders;
	static std::vector<Texture*> textures;
};