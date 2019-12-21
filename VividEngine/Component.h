#pragma once
#include "stdafx.h"

template<typename T>
class Component {
public:
	T component;
	void SetOwner() {
		component::owner = this;
	}
};