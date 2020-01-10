#include "stdafx.h"
#include "Object.h"
#include "Component.h"
#include "Transform.h"
#include "Camera.h"
#include "SpriteRenderer.h"
#include "Renderer3D.h"
#include "Light.h"
#include "Scene.h"
#include "Random.h"

BD_CLASS(Component, Camera)
BD_CLASS(Component, SpriteRenderer)
BD_CLASS(Component, Transform)
BD_CLASS(Component, Renderer3D)
BD_CLASS(Component, Light)

Object::Object()
{
	entity = vivid::getRandom();

	AddComponent<Transform>();
	Scene::objects.push_back(this);
}

void Object::SetDebugFlag(bool flag)
{
	entity = 0;
}

bool Object::GetDebugFlag()
{
	return (entity == 0) ? true : false;
}