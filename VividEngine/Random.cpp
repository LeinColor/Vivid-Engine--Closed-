#include "stdafx.h"
#include "Random.h"

#include <random>
namespace vivid {
	std::random_device rd;
	std::mt19937 mt(rd());
	uint32_t vivid::getRandom()
	{
		std::uniform_int_distribution<uint32_t> dist(0, UINT32_MAX);
		return dist(mt);
	}
}