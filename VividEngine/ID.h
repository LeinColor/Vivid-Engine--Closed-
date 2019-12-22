#pragma once
#include "stdafx.h"
#include "Random.h"

class ID {
private:
	uint32_t id;
public:
	ID()
	{
		id = vivid::getRandom();
	}

	uint32_t Get()
	{
		return id;
	}
};