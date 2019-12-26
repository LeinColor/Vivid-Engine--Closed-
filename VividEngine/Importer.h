#pragma once

#include <vector>
using namespace std;

class Importer {
public:

	struct attribute_t {
		vector<XMFLOAT3> vertices;
		vector<XMFLOAT2> texcoords;
		vector<XMFLOAT3> normals;
	};

	struct indices_t {
		vector<unsigned int> vertexIndices;
		vector<unsigned int> uvIndices;
		vector<unsigned int> normalIndices;
	};

	static bool LoadObjFile(const char* path,
		_Out_ vector<XMFLOAT3>& out_vertices,
		_Out_ vector<XMFLOAT2>& out_uvs,
		_Out_ vector<XMFLOAT3>& out_normals);

private:
};