#pragma once
#include "stdafx.h"
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "GameObject.h"
#include <vector>

class Manager {
public:
	static vector<GameObject*> gameObjects;
	static vector<Mesh*> meshes;
	static vector<Shader*> shaders;
	static vector<Texture*> textures;
};