#pragma once
#include "stdafx.h"

#include <vector>

class Scene {
public:
	virtual void Start() {};
	virtual void Update() {};
	virtual void FixedUpdate() {};
};