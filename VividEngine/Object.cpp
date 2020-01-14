#include "stdafx.h"
#include "Object.h"
#include "Component.h"
#include "Transform.h"
#include "Camera.h"
#include "Renderer3D.h"
#include "Light.h"
#include "Scene.h"
#include "AABB.h"
#include "Random.h"

BD_CLASS(Component, Camera)
BD_CLASS(Component, Transform)
BD_CLASS(Component, Renderer3D)
BD_CLASS(Component, Light)

Object::Object()
{
	entity = vivid::getRandom();

	AddComponent<Transform>();
	Scene::objects.push_back(this);
	//Scene::aabbs.push_back(new AABB());
}

void Object::SetActive(bool flag)
{
	active = flag;
}

bool Object::GetActive()
{
	return active;
}