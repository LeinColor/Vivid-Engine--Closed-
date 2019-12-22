#pragma once
#include "stdafx.h"
#include "ID.h"
#include "Transform.h"
#include "Camera.h"
#include "SpriteRenderer.h"

template<typename T>
class Component {
private:
	ID id;
public:
	T component;

	uint32_t GetInstanceID()
	{
		return id.Get();
	}
	void SetOwner() {
		component::owner = this;
	}
};