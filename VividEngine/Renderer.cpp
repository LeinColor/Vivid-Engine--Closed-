/**
	Renderer.cpp

	Purpose: Class responsible for rendering all objects.

	@author Lein
	@version 1.0 12/20/19
*/
#include "stdafx.h"
#include "Renderer.h"
#include "Debug.h"
using namespace vivid;

//extern AppHandle appHandle;

std::vector<Mesh*> meshVector;
std::vector<Material*> materialVector;
DirectX11Wrapper* Renderer::GetDevice()
{
	return dxWrapper;
}

void Renderer::SetDevice(DirectX11Wrapper* value)
{
	dxWrapper = value;
}

void Renderer::Initialize(Manager<GameObject>& manager)
{
	mainCamera = manager.At(0);

	LoadMesh();
}

void Renderer::Render(Manager<GameObject>& manager)
{
	dxWrapper->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);
	mainCamera->GetComponent<Camera>().Render(dxWrapper->GetScreenWidth(), dxWrapper->GetScreenHeight(), SCREEN_DEPTH, SCREEN_NEAR);

	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	worldMatrix = mainCamera->GetComponent<Transform>().GetWorldMatrix();
	viewMatrix = mainCamera->GetComponent<Camera>().GetViewMatrix();
	projectionMatrix = mainCamera->GetComponent<Camera>().GetProjectionMatrix();

	dxWrapper->EndScene();
}

void Renderer::LoadMesh()
{
	// Create new mesh instance
	Mesh* mesh = new Mesh();
	meshVector.push_back(mesh);

	// Load cube obj
	bool success = Importer::LoadObjFile("../VividEngine/cube.obj", mesh->vertices, mesh->uvs, mesh->normals);

	if (!success) // exit if file import failed
	{
		Debug::Log("Obj file import failed!");
		return;
	}

	struct vertex_t {
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT4 color;
	};

	UINT vertexCount = mesh->vertices.size();
	vertex_t* vertices = new vertex_t[vertexCount];			 // allocate vertex_t for vertex count.
	unsigned long* indices = new unsigned long[vertexCount]; // index count is same with vertex count.

	// Vertex buffer description definition
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(vertex_t) * vertexCount;
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
	ID3D11Buffer* vertexBuffer;
	dxWrapper->GetDevice()->CreateBuffer(&bd, &sd, &vertexBuffer);

}