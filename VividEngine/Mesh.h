#pragma once
#include "stdafx.h"
#include "Importer.h"
#include <vector>

class Mesh {
public:
	std::vector<XMFLOAT3> positions;
	std::vector<XMFLOAT2> texcoords;
	std::vector<XMFLOAT3> normals;

	inline ID3D11Buffer** GetVertexBuffer() { return &vertexBuffer; }
	inline ID3D11Buffer** GetIndexBuffer() { return &indexBuffer; }
	inline ID3D11Buffer** GetMatrixBuffer() { return &matrixBuffer; }
	inline std::string GetName() { return name; }
	inline void SetName(const std::string value) { name = value; }


private:
	std::string name;
	ID3D11Buffer* vertexBuffer = nullptr;
	ID3D11Buffer* indexBuffer = nullptr;
	ID3D11Buffer* matrixBuffer = nullptr;
};