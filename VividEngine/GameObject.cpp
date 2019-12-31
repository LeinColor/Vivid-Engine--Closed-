#include "stdafx.h"
#include "GameObject.h"
#include "Object.h"
#include "Component.h"
#include "Transform.h"
#include "Camera.h"
#include "SpriteRenderer.h"
#include "Renderer3D.h"
#include "Light.h"
#include "Manager.h"

BD_CLASS(Component, Camera)
BD_CLASS(Component, SpriteRenderer)
BD_CLASS(Component, Transform)
BD_CLASS(Component, Renderer3D)
BD_CLASS(Component, Light)

GameObject::GameObject()
{
	AddComponent<Transform>();
	Manager::gameObjects.push_back(this);
}