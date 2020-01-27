#include "Importer.h"

void Importer::LoadObjFile(MeshComponent& mesh)
{
	tinyobj::attrib_t attrib;
	vector<tinyobj::shape_t> shapes;
	vector<tinyobj::material_t> materials;

	string warn, error;
	bool success = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &error, fileName, "../VividEngine/Obj", true);

	if (!success) {
		vivid::Debug::Log("Obj file import failed");
		return;
	}

	for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
		auto& index = shapes[0].mesh.indices[i];

		if (index.vertex_index != 0) {
			positions.push_back(XMFLOAT3(
				attrib.vertices[index.vertex_index * 3 + 0],
				attrib.vertices[index.vertex_index * 3 + 1],
				attrib.vertices[index.vertex_index * 3 + 2]));
		}

		if (index.texcoord_index != 0) {
			texcoords.push_back(XMFLOAT2(
				attrib.texcoords[index.texcoord_index * 2 + 0],
				attrib.texcoords[index.texcoord_index * 2 + 1]));
		}

		if (index.normal_index != 0) {
			normals.push_back(XMFLOAT3(
				attrib.normals[index.normal_index * 3 + 0],
				attrib.normals[index.normal_index * 3 + 1],
				attrib.normals[index.normal_index * 3 + 2]));
		}
	}
}