#pragma once
#include "stdafx.h"

#include "../ThirdParty/entt/entt.hpp"

#include <vector>

class Scene {
public:
	virtual void Start() {};
	virtual void Update() {};
	virtual void FixedUpdate() {};

	entt::registry registry;
	entt::entity enttMainCamera;
};