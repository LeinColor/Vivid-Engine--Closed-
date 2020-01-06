#pragma once
#include "stdafx.h"

#include <vector>

class Object;
class Mesh;
class Shader;
class AABB;
class Texture;
class Scene {
public:
	static std::vector<Object*> objects;
	static std::vector<Object*> lights;
	static std::vector<Object*> gizmos; // for Debug, should to remove this
	static std::vector<AABB*> aabbs;
	static std::vector<Mesh*> meshes;
	static std::vector<Shader*> shaders;
	static std::vector<Texture*> textures;
};