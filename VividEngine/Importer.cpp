#define _CRT_SECURE_NO_WARNINGS
#include "stdafx.h"
#include "Importer.h"
#include "Debug.h"

#include <fstream>

using namespace std;
using namespace vivid;
bool Importer::LoadObjFile(const char* path,
	_Out_ vector<XMFLOAT3>& out_vertices,
	_Out_ vector<XMFLOAT2>& out_uvs,
	_Out_ vector<XMFLOAT3>& out_normals)
{
	FILE* file = fopen(path, "r");
	if (file == NULL)
		return false;

	vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	vector<XMFLOAT3> tempVertices;
	vector<XMFLOAT2> tempUvs;
	vector<XMFLOAT3> tempNormals;

	while (true) {
		char lineHeader[128];
		// Read the first word of the line
		int res = fscanf(file, "%s", lineHeader);

		// Read file until EOF
		if (res == EOF)
			break;

		if (strcmp(lineHeader, "v") == 0) {
			XMFLOAT3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			tempVertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			XMFLOAT2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			tempUvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			XMFLOAT3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			tempNormals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			unsigned int vertexIndex[4], uvIndex[4], normalIndex[4];
			int matches =
				fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n",
					&vertexIndex[0], &uvIndex[0], &normalIndex[0],
					&vertexIndex[1], &uvIndex[1], &normalIndex[1],
					&vertexIndex[2], &uvIndex[2], &normalIndex[2],
					&vertexIndex[3], &uvIndex[3], &normalIndex[3]);

			if (matches != 12) {
				return false;
			}

			for (int i = 0; i < 4; i++) {
				vertexIndices.push_back(vertexIndex[i]);
				uvIndices.push_back(uvIndex[i]);
				normalIndices.push_back(normalIndex[i]);
			}
		}
	}
	fclose(file);

	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		XMFLOAT3 vertex = tempVertices[vertexIndex - 1];
		XMFLOAT2 uv		= tempUvs[uvIndex - 1];
		XMFLOAT3 normal = tempNormals[normalIndex - 1];

		out_vertices.push_back(vertex);
		out_uvs.push_back(uv);
		out_normals.push_back(normal);
	}
	return true;
}