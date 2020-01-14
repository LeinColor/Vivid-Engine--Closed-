#include "stdafx.h"
#include "Loader.h"
#include "tiny_obj_loader.h"
#include "Debug.h"
#include "Maths.h"
#include "Mesh.h"
#include "Manager.h"
#include "Renderer.h"
#include "DirectX11Wrapper.h"
#include <string>
using namespace vivid;
using namespace std;

void Loader::Meshes()
{
	// Load order sensitive because of mesh enum value.
	LoadMeshFromObj("../VividEngine/Obj/cube.obj");
	LoadMeshFromObj("../VividEngine/Obj/uvsphere.obj");
	LoadMeshFromObj("../VividEngine/Obj/cone.obj");
	LoadMeshFromObj("../VividEngine/Obj/plane.obj");
	LoadMeshFromObj("../VividEngine/Obj/cylinder.obj");
	LoadLine();
}

void Loader::LoadMeshFromObj(const char* fileName)
{
	// Initialize loader
	string warn, error;
	tinyobj::attrib_t attrib;
	vector<tinyobj::shape_t> shapes;
	vector<tinyobj::material_t> materials;
	bool success = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &error, fileName, "../VividEngine/Obj", true);

	if (!success) {
		Debug::Log("Obj file import failed!");
		return;
	}

	Mesh* mesh = new Mesh();
	Manager::meshes.push_back(mesh);

	//TODO: Not showing so should fix this!
	UINT indexCount = shapes[0].mesh.indices.size();
	vertex_PCN_t* vertices = new vertex_PCN_t[indexCount];	 // allocate vertex_t for vertex count
	unsigned long* indices = new unsigned long[indexCount];  // allocate unsigned long for indices count

	XMFLOAT3 min = XMFLOAT3(FLT_MAX, FLT_MAX, FLT_MAX);
	XMFLOAT3 max = XMFLOAT3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	for (int i = 0; i < indexCount; i++) {
		auto& index = shapes[0].mesh.indices[i];

		// vertex position
		vertices[i].position = XMFLOAT3(
			attrib.vertices[index.vertex_index * 3 + 0],
			attrib.vertices[index.vertex_index * 3 + 1],
			attrib.vertices[index.vertex_index * 3 + 2]);

		min = vivid::Min(min, vertices[i].position);
		max = vivid::Max(max, vertices[i].position);

		// vertex color
		XMFLOAT4 color = XMFLOAT4(1.0, 1.0, 0.0, 1.0);
		/*
		if (index.texcoord_index >= 0) {
			color = XMFLOAT4(
				attrib.texcoords[index.texcoord_index * 2 + 0],
				attrib.texcoords[index.texcoord_index * 2 + 1],
				attrib.texcoords[index.texcoord_index * 2 + 0],
				1.0f);
		}
		*/
		vertices[i].color = color;

		XMFLOAT3 tmpNormal = XMFLOAT3(0.0f, 0.0f, 0.0f);
		if (index.normal_index >= 0) {
			tmpNormal = XMFLOAT3(
				attrib.normals[index.normal_index * 3 + 0],
				attrib.normals[index.normal_index * 3 + 1],
				attrib.normals[index.normal_index * 3 + 2]);
		}
		vertices[i].normal = tmpNormal;

		indices[i] = i;

		// Below exist to caching
		// assign position to mesh attribute's
		XMFLOAT3 position = XMFLOAT3(0, 0, 0);
		position = XMFLOAT3(
			attrib.vertices[index.vertex_index * 3 + 0],
			attrib.vertices[index.vertex_index * 3 + 1],
			attrib.vertices[index.vertex_index * 3 + 2]);

		// assign texcoord to mesh attribute's
		XMFLOAT2 texcoord = XMFLOAT2(0, 0);
		if (index.texcoord_index >= 0) {
			texcoord = XMFLOAT2(
				attrib.texcoords[index.texcoord_index * 2 + 0],
				attrib.texcoords[index.texcoord_index * 2 + 1]);
		}

		// assign normal to mesh attribute's
		XMFLOAT3 normal = XMFLOAT3(0, 0, 0);
		if (index.normal_index >= 0) {
			normal = XMFLOAT3(
				attrib.normals[index.normal_index * 3 + 0],
				attrib.normals[index.normal_index * 3 + 1],
				attrib.normals[index.normal_index * 3 + 2]);
		}

		mesh->positions.push_back(position);
		mesh->texcoords.push_back(texcoord);
		mesh->normals.push_back(normal);
	}
	mesh->aabb = AABB(min, max);

	//*************************************
	//**        Vertex Buffer            **
	//*************************************
	// Vertex buffer description definition
	{
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(vertex_PCN_t) * indexCount;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;

		// Vertex subresource data definition
		D3D11_SUBRESOURCE_DATA sd;
		sd.pSysMem = vertices;
		sd.SysMemPitch = 0;
		sd.SysMemSlicePitch = 0;

		// Create vertex buffer
		graphics->GetDevice()->CreateBuffer(&bd, &sd, mesh->GetVertexBuffer());
	}


	//*************************************
	//**        Index Buffer             **
	//*************************************
	// Index buffer description definition
	{
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned long) * indexCount;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;

		// Index subresource data definition
		D3D11_SUBRESOURCE_DATA sd;
		sd.pSysMem = indices;
		sd.SysMemPitch = 0;
		sd.SysMemSlicePitch = 0;

		// Create index buffer
		graphics->GetDevice()->CreateBuffer(&bd, &sd, mesh->GetIndexBuffer());
	}

	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE_ARRAY(indices);
}

void Loader::LoadLine()
{
	ID3D11Buffer* lineVertexBuffer = nullptr;
	ID3D11Buffer* lineIndexBuffer = nullptr;

	Mesh* mesh = new Mesh();
	Manager::meshes.push_back(mesh);

	//TODO: Not showing so should fix this!
	UINT indexCount = 2;
	vertex_P_t* vertices = new vertex_P_t[indexCount];	 // allocate vertex_t for vertex count
	unsigned long* indices = new unsigned long[indexCount];  // allocate unsigned long for indices count

	for (int i = 0; i < indexCount; i++) {
		// vertex position
		vertices[i].position = XMFLOAT3(0, 0, 0);
		indices[i] = i;
		mesh->positions.push_back(XMFLOAT3(0, 0, 0));
	}

	//*************************************
	//**        Vertex Buffer            **
	//*************************************
	// Vertex buffer description definition
	{
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(vertex_P_t) * indexCount;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;

		// Vertex subresource data definition
		D3D11_SUBRESOURCE_DATA sd;
		sd.pSysMem = vertices;
		sd.SysMemPitch = 0;
		sd.SysMemSlicePitch = 0;

		// Create vertex buffer
		graphics->GetDevice()->CreateBuffer(&bd, &sd, mesh->GetVertexBuffer());
	}


	//*************************************
	//**        Index Buffer             **
	//*************************************
	// Index buffer description definition
	{
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned long) * indexCount;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;

		// Index subresource data definition
		D3D11_SUBRESOURCE_DATA sd;
		sd.pSysMem = indices;
		sd.SysMemPitch = 0;
		sd.SysMemSlicePitch = 0;

		// Create index buffer
		graphics->GetDevice()->CreateBuffer(&bd, &sd, mesh->GetIndexBuffer());
	}

	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE_ARRAY(indices);
}