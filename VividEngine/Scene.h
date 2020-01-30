#pragma once
#include "stdafx.h"
#include "ECS.h"

#include <vector>

class ObjectComponent;
class TransformComponent;
class CameraComponent;
class LightComponent;
class MeshComponent;
class ShaderComponent;

class Scene {
public:
	ComponentManager<ObjectComponent> objects;
	ComponentManager<TransformComponent> transforms;
	ComponentManager<CameraComponent> cameras;
	ComponentManager<LightComponent> lights;
	ComponentManager<MeshComponent> meshes;
	ComponentManager<ShaderComponent> shaders;

	virtual void Start() {};
	virtual void Update() {};
	virtual void FixedUpdate() {};
};