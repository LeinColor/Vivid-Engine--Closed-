#pragma once
#include "Object.h"
#include "Component.h"
#include "Transform.h"
#include "Camera.h"
#include "MeshFilter.h"

class GameObject : COMPONENTS {
public:
	template<typename T>
	T& GetComponent()
	{
		return this->Component<T>::component;
	}

	template<typename T>
	T& AddComponent() {
		this->Component<T>::SetOwner(this);
		return this->Component<T>::component;
	}
};