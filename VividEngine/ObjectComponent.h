#pragma once
#include "stdafx.h"
#include "MeshComponent.h"
#include "ECS.h"

#include <string>
class ObjectComponent {
public:
	std::string name;

	Entity meshEntity = INVALID_ENTITY;
	Entity shaderEntity = INVALID_ENTITY;
};