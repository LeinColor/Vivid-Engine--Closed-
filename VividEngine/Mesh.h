#pragma once
#include "stdafx.h"
#include "Importer.h"
#include <vector>

class Mesh {
public:
	std::vector<XMFLOAT3> vertices;
	std::vector<XMFLOAT2> uvs;
	std::vector<XMFLOAT3> normals;

private:
};