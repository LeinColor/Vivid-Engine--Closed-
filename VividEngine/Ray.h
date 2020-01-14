#pragma once
#include "stdafx.h"

class Ray {
public:
	Ray() {}


private:

};

struct RaycastHit {
	uint32_t entity;
	int index;
	XMFLOAT4 prevColor;
};