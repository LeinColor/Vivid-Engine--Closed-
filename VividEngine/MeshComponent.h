#pragma once
#include "stdafx.h"
#include "Importer.h"
#include "AABB.h"
#include <vector>
#include <string>

class MeshComponent {
	friend class GraphicsAPI;
public:
	std::vector<XMFLOAT3> positions;
	std::vector<XMFLOAT4> colors;
	std::vector<XMFLOAT2> texcoords;
	std::vector<XMFLOAT3> normals;

	AABB aabb;

	ID3D11Buffer* vertexBufferPos = nullptr;
	ID3D11Buffer* vertexBufferCol = nullptr;
	ID3D11Buffer* vertexBufferTex = nullptr;
	ID3D11Buffer* vertexBufferNor = nullptr;
	ID3D11Buffer* indexBuffer = nullptr;

	UINT		  stride = 0;
	UINT		  offset = 0;
};