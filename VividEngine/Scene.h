#pragma once
#include "stdafx.h"

#include <vector>
#include <unordered_map>

class Object;
class Mesh;
class Shader;
class AABB;
class Texture;
class Scene {
public:
	static std::vector<Object*> objects;
	static std::vector<Object*> lights;
	static std::vector<AABB*> aabbs;

	static XMVECTOR ScreenToWorldPoint(const XMFLOAT3& pos);
public:
	static Object* GetMainCamera();
	static Object* GetLight();
};