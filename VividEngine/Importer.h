#pragma once

#include <vector>
using namespace std;
class Importer {
public:
	static void LoadObjFile(const char* path,
		_Out_ vector<XMFLOAT3>& out_vertices,
		_Out_ vector<XMFLOAT2>& out_uvs,
		_Out_ vector<XMFLOAT3>& out_normals);

private:
};