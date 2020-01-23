#pragma once
#include "stdafx.h"
#include "Importer.h"
#include "AABB.h"
#include <vector>
#include <string>

class Mesh {
public:
	std::vector<XMFLOAT3> positions;
	std::vector<XMFLOAT4> colors;
	std::vector<XMFLOAT2> texcoords;
	std::vector<XMFLOAT3> normals;
	AABB aabb;

	Mesh(std::string name);
	inline std::string GetName() { return name; }
	inline void SetName(const std::string value) { name = value; }

	void LoadFromObjFile(const char* fileName);

private:
	uint32_t	id;
	std::string name;
};