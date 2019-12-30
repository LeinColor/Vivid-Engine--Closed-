#pragma once
#include "stdafx.h"

#include <vector>

class GameObject;
class Mesh;
class Shader;
class Texture;
class Manager {
public:
	static std::vector<GameObject*> gameObjects;
	static std::vector<Mesh*> meshes;
	static std::vector<Shader*> shaders;
	static std::vector<Texture*> textures;
};