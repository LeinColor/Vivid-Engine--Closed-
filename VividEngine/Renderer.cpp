/**
	Renderer.cpp

	Purpose: Class responsible for rendering all objects.

	@author Lein
	@version 1.0 12/20/19
*/
#include "stdafx.h"
#include "Renderer.h"
#include "Descriptor.h"
#include "Debug.h"
#include "Material.h"
#include "Shader.h"
#include "Texture.h"
#include "Renderer3D.h"
#include "Maths.h"
#include "Time.h"
#include "tiny_obj_loader.h"
using namespace vivid;
using namespace std;


// Shader containers
std::vector<ID3D11VertexShader*> vertexShaders;
std::vector<ID3D11PixelShader*> pixelShaders;
std::vector<ID3D11GeometryShader*> geometryShaders;
std::vector<ID3D11HullShader*> hullShaders;
std::vector<ID3D11DomainShader*> domainShaders;
std::vector<ID3D11ComputeShader*> computeShaders;
std::vector<Shader*> shaders;

// Layout container
std::vector<ID3D11InputLayout*> inputLayouts;
std::vector<ID3D11Buffer*> constantBuffers;

std::vector<ID3D11Buffer*> resourceBuffers;
std::vector<ID3D11SamplerState*> samplerStates;

std::vector<GizmoLine> gizmoLines;

void Renderer::Initialize()
{
	// Load order sensitive because of mesh enum value.
	LoadMesh("../VividEngine/Obj/cube.obj");
	LoadMesh("../VividEngine/Obj/uvsphere.obj");
	LoadMesh("../VividEngine/Obj/cone.obj");
	LoadMesh("../VividEngine/Obj/plane.obj");
	LoadMesh("../VividEngine/Obj/cylinder.obj");
	LoadShader();

	LoadLine();


}

void Renderer::AddGizmoLine(const GizmoLine& line) {
	gizmoLines.push_back(line);
}

void Renderer::LoadMesh(const char* fileName)
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
	Scene::meshes.push_back(mesh);
	
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
		XMFLOAT4 color = XMFLOAT4(1.0, 0.0, 0.0, 1.0);
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
		dxWrapper->GetDevice()->CreateBuffer(&bd, &sd, mesh->GetVertexBuffer());
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
		dxWrapper->GetDevice()->CreateBuffer(&bd, &sd, mesh->GetIndexBuffer());
	}

	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE_ARRAY(indices);
}

void Renderer::LoadShader()
{
	Shader* shader = new Shader();

	// resize vector capacity
	//constantBuffers.resize(CONSTANT_BUFFER_DEFAULT_COUNT);


	//***********************************
	//**       Vertex Shader           **
	//***********************************
	ID3D11VertexShader* vertexShaderDebug = nullptr;
	ID3D10Blob* vertexShaderBufferDebug = nullptr;
	if (FAILED(D3DCompileFromFile(L"../VividEngine/Debug.vs", nullptr, nullptr, "DebugVertexMain", "vs_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBufferDebug, nullptr)))
	{
		Debug::Log("Vertex Shader load failed!");
		return;
	}
	dxWrapper->GetDevice()->CreateVertexShader(vertexShaderBufferDebug->GetBufferPointer(), vertexShaderBufferDebug->GetBufferSize(), nullptr, &vertexShaderDebug);
	vertexShaders.push_back(vertexShaderDebug);

	ID3D11VertexShader* vertexShader = nullptr;
	ID3D10Blob* vertexShaderBuffer = nullptr;
	if (FAILED(D3DCompileFromFile(L"../VividEngine/BlinnPhong.vs", nullptr, nullptr, "LightVertexMain", "vs_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, nullptr)))
	{
		Debug::Log("Vertex Shader load failed!");
		return;
	}
	dxWrapper->GetDevice()->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), nullptr, &vertexShader);
	vertexShaders.push_back(vertexShader);


	//***********************************
	//**      Input Layout             **
	//***********************************
	{
		ID3D11InputLayout* inputLayout = nullptr;
		D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	 0, 0,							  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		dxWrapper->GetDevice()->CreateInputLayout(layoutDesc, ARRAYSIZE(layoutDesc), vertexShaderBufferDebug->GetBufferPointer(), vertexShaderBufferDebug->GetBufferSize(), &inputLayout);
		inputLayouts.push_back(inputLayout);
	}
	{
		ID3D11InputLayout* inputLayout = nullptr;
		D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	 0, 0,							  D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",	  0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		dxWrapper->GetDevice()->CreateInputLayout(layoutDesc, ARRAYSIZE(layoutDesc), vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &inputLayout);
		inputLayouts.push_back(inputLayout);
	}

	//***********************************
	//**       Pixel Shader            **
	//***********************************
	ID3D11PixelShader* pixelShaderDebug = nullptr;
	ID3D10Blob* pixelShaderBufferDebug = nullptr;
	if (FAILED(D3DCompileFromFile(L"../VividEngine/Debug.ps", nullptr, nullptr, "DebugPixelMain", "ps_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBufferDebug, nullptr)))
	{
		Debug::Log("Pixel Shader load failed!");
		return;
	}
	dxWrapper->GetDevice()->CreatePixelShader(pixelShaderBufferDebug->GetBufferPointer(), pixelShaderBufferDebug->GetBufferSize(), nullptr, &pixelShaderDebug);
	pixelShaders.push_back(pixelShaderDebug);

	ID3D11PixelShader* pixelShader = nullptr;
	ID3D10Blob* pixelShaderBuffer = nullptr;
	if (FAILED(D3DCompileFromFile(L"../VividEngine/BlinnPhong.ps", nullptr, nullptr, "LightPixelMain", "ps_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, nullptr)))
	{
		Debug::Log("Pixel Shader load failed!");
		return;
	}
	dxWrapper->GetDevice()->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), nullptr, &pixelShader);
	pixelShaders.push_back(pixelShader);

	SAFE_RELEASE(vertexShaderBuffer);
	SAFE_RELEASE(pixelShaderBuffer);
	SAFE_RELEASE(vertexShaderBufferDebug);
	SAFE_RELEASE(pixelShaderBufferDebug);

	//***********************************
	//**      Constant Buffer          **
	//***********************************
	// World, View, Projection (WVP) buffer
	{
		ID3D11Buffer* WVPBuffer = nullptr;
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(MatrixBufferType);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;
		dxWrapper->GetDevice()->CreateBuffer(&bd, nullptr, &WVPBuffer);
		constantBuffers.push_back(WVPBuffer);
		//constantBuffers[CONSTANT_BUFFER_WVP] = WVPBuffer;
	}
	// Camera buffer
	{
		ID3D11Buffer* cameraBuffer = nullptr;
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(CameraBufferType);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;
		dxWrapper->GetDevice()->CreateBuffer(&bd, nullptr, &cameraBuffer);
		constantBuffers.push_back(cameraBuffer);
		//constantBuffers[CONSTANT_BUFFER_CAMERA] = cameraBuffer;
	}
	// Light buffer
	{
		ID3D11Buffer* lightBuffer = nullptr;
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(LightBufferType);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;
		dxWrapper->GetDevice()->CreateBuffer(&bd, nullptr, &lightBuffer);
		constantBuffers.push_back(lightBuffer);
		//constantBuffers[CONSTANT_BUFFER_LIGHT] = lightBuffer;
	}
	// Color buffer
	{
		ID3D11Buffer* colorBuffer = nullptr;
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(ColorBufferType);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;
		dxWrapper->GetDevice()->CreateBuffer(&bd, nullptr, &colorBuffer);
		constantBuffers.push_back(colorBuffer);
		//constantBuffers[CONSTANT_BUFFER_LIGHT] = lightBuffer;
	}

	shader->vertexShader = &vertexShader;
	shader->pixelShader = &pixelShader;
	shaders.push_back(shader);
}

void Renderer::Render()
{
	// Set Main Camera
	mainCamera = Scene::objects[0];

	// Prepare to render
	dxWrapper->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	////////////////////////////////////////////////////////
	mainCamera->GetComponent<Camera>().Update(dxWrapper->GetScreenWidth(), dxWrapper->GetScreenHeight());
	auto viewMatrix = mainCamera->GetComponent<Camera>().GetViewMatrix();
	auto projectionMatrix = mainCamera->GetComponent<Camera>().GetProjectionMatrix();
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);

	for (int i = 1; i < Scene::objects.size() - 1; i++) {
		auto mesh = Scene::objects[i]->GetComponent<Renderer3D>().mesh;

		// if object doesn't have mesh, skip rendering
		if (mesh == nullptr)
			continue;

		// Get object's world matrix and transpose it to render.
		auto worldMatrix = Scene::objects[i]->GetComponent<Transform>().GetWorldMatrix();
		worldMatrix = XMMatrixTranspose(worldMatrix);

		// Draw AABB box
		if (Scene::objects[i]->state == DEFAULT) {
			// AABB transformation by each object matrix
			AABB aabb = mesh->aabb.Transform(Scene::objects[i]->GetComponent<Transform>().GetWorldMatrix());
			XMFLOAT3 aabbVertices[8];
			for (int i = 0; i < 8; i++) {
				aabbVertices[i] = aabb.GetVertex(i);
			}
			XMFLOAT4 color{ 1,1,1,1 };
			XMFLOAT3& a = mesh->aabb.minPos;
			XMFLOAT3& b = mesh->aabb.maxPos;
			DrawLine(aabbVertices[0], aabbVertices[1], color);
			DrawLine(aabbVertices[0], aabbVertices[4], color);
			DrawLine(aabbVertices[1], aabbVertices[5], color);
			DrawLine(aabbVertices[4], aabbVertices[5], color);
			DrawLine(aabbVertices[0], aabbVertices[3], color);
			DrawLine(aabbVertices[1], aabbVertices[2], color);
			DrawLine(aabbVertices[2], aabbVertices[3], color);
			DrawLine(aabbVertices[4], aabbVertices[7], color);
			DrawLine(aabbVertices[5], aabbVertices[6], color);
			DrawLine(aabbVertices[6], aabbVertices[7], color);
			DrawLine(aabbVertices[3], aabbVertices[7], color);
			DrawLine(aabbVertices[2], aabbVertices[6], color);
		}
		
		// Lock constant buffer to write description.
		MatrixBufferType cbMatrix = { worldMatrix, viewMatrix, projectionMatrix };
		dxWrapper->UpdateBuffer(constantBuffers[CONSTANT_BUFFER_WVP], &cbMatrix, sizeof(MatrixBufferType));
		dxWrapper->GetContext()->VSSetConstantBuffers(0, 1, &constantBuffers[CONSTANT_BUFFER_WVP]);

		
		unsigned int stride = sizeof(vertex_PCN_t);
		unsigned int offset = 0;
		if (Scene::objects[i]->state == DEFAULT) {
			CameraBufferType cbCamera = { mainCamera->GetComponent<Transform>().GetPosition(), 0.0f };
			dxWrapper->UpdateBuffer(constantBuffers[CONSTANT_BUFFER_CAMERA], &cbCamera, sizeof(cbCamera));
			dxWrapper->GetContext()->VSSetConstantBuffers(1, 1, &constantBuffers[CONSTANT_BUFFER_CAMERA]);

			LightBufferType cbLight;
			auto attrib = Scene::objects[7]->GetComponent<Light>().attrib;
			cbLight = { attrib.ambientColor, attrib.diffuseColor, attrib.lightDirection, attrib.specularPower, attrib.specularColor };
			dxWrapper->UpdateBuffer(constantBuffers[CONSTANT_BUFFER_LIGHT], &cbLight, sizeof(cbLight));
			dxWrapper->GetContext()->PSSetConstantBuffers(0, 1, &constantBuffers[CONSTANT_BUFFER_LIGHT]);

			// Bind pipeline before drawing
			dxWrapper->GetContext()->IASetInputLayout(inputLayouts[1]);
			dxWrapper->GetContext()->VSSetShader(vertexShaders[1], nullptr, 0);
			dxWrapper->GetContext()->PSSetShader(pixelShaders[1], nullptr, 0);
			dxWrapper->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		}
		else if (Scene::objects[i]->state == DEBUG)
		{
			// gizmo drawing (absolute scale)
			XMVECTOR vk = XMLoadFloat3(&Scene::objects[i]->GetComponent<Transform>().GetPosition());
			XMVECTOR result = XMVector3Transform(vk, mainCamera->GetComponent<Camera>().GetViewProjectionMatrix());
			float w = XMVectorGetW(result) / 64;
			Scene::objects[i]->GetComponent<Transform>().SetScale(w / 2, w, w / 2);


			ColorBufferType cbColor = { Scene::objects[i]->GetComponent<Renderer3D>().color };
			dxWrapper->UpdateBuffer(constantBuffers[CONSTANT_BUFFER_COLOR], &cbColor, sizeof(cbColor));
			dxWrapper->GetContext()->VSSetConstantBuffers(1, 1, &constantBuffers[CONSTANT_BUFFER_COLOR]);

			// Bind pipeline before drawing
			dxWrapper->GetContext()->IASetInputLayout(inputLayouts[0]);
			dxWrapper->GetContext()->VSSetShader(vertexShaders[0], nullptr, 0);
			dxWrapper->GetContext()->PSSetShader(pixelShaders[0], nullptr, 0);
			dxWrapper->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		}

		dxWrapper->GetContext()->IASetVertexBuffers(0, 1, mesh->GetVertexBuffer(), &stride, &offset);
		dxWrapper->GetContext()->IASetIndexBuffer(*mesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
		dxWrapper->GetContext()->DrawIndexed(mesh->positions.size(), 0, 0);
	}

	// Render gizmo lines
	for (auto& line : gizmoLines) {
		DrawLine(line.startPoint, line.endPoint, line.color);
	}

	auto ppp = mainCamera->GetComponent<Transform>().GetPosition();
	//DrawLine(XMFLOAT3(ppp.x,ppp.y,ppp.z+1), XMFLOAT3(target.x, target.y, target.z + 2), XMFLOAT4(0, 1, 0, 1));

	// Clear gizmo line queue
	gizmoLines.clear();

	/////////////////////////////////////////////////////
	// Render end
	dxWrapper->EndScene();
	
}

ID3D11Buffer* lineVertexBuffer = nullptr;
ID3D11Buffer* lineIndexBuffer = nullptr;
void Renderer::LoadLine()
{
	//TODO: Not showing so should fix this!
	UINT indexCount = 2;
	vertex_PCN_t* vertices = new vertex_PCN_t[indexCount];	 // allocate vertex_t for vertex count
	unsigned long* indices = new unsigned long[indexCount];  // allocate unsigned long for indices count

	for (int i = 0; i < indexCount; i++) {
		// vertex position
		vertices[i].position = XMFLOAT3(0, 0, 0);
		vertices[i].color = XMFLOAT4(0.0, 0.0, 0.0, 1.0);
		vertices[i].normal = XMFLOAT3(0, 0, 0);
		indices[i] = i;
	}

	//*************************************
	//**        Vertex Buffer            **
	//*************************************
	// Vertex buffer description definition
	{
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(vertex_PCN_t) * indexCount;
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
		dxWrapper->GetDevice()->CreateBuffer(&bd, &sd, &lineVertexBuffer);
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
		dxWrapper->GetDevice()->CreateBuffer(&bd, &sd, &lineIndexBuffer);
	}

	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE_ARRAY(indices);
}

void Renderer::DrawLine(XMFLOAT3 startPoint, XMFLOAT3 endPoint, XMFLOAT4 color)
{
	vertex_PCN_t* vertices = new vertex_PCN_t[2];
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	vertex_PCN_t* verticesPtr;

	vertices[0].position = startPoint;
	vertices[0].color = color;

	vertices[1].position = endPoint;
	vertices[1].color = color;

	//*********************************
	//**      Constant Buffer        **
	//*********************************
	auto worldMatrix = XMLoadFloat4x4(&IDENTITYMATRIX);
	auto viewMatrix = mainCamera->GetComponent<Camera>().GetViewMatrix();
	auto projectionMatrix = mainCamera->GetComponent<Camera>().GetProjectionMatrix();

	// Transpose matrix to use within shader.
	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);

	// Lock constant buffer to write description.
	dxWrapper->GetContext()->Map(constantBuffers[CONSTANT_BUFFER_WVP], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// Get constant buffer pointer
	MatrixBufferType* pMatrixBufferData = (MatrixBufferType*)mappedResource.pData;

	// copy to constant buffer
	pMatrixBufferData->world = worldMatrix;
	pMatrixBufferData->view = viewMatrix;
	pMatrixBufferData->projection = projectionMatrix;

	dxWrapper->GetContext()->Unmap(constantBuffers[CONSTANT_BUFFER_WVP], 0);

	unsigned bufferNumber = 0;
	dxWrapper->GetContext()->VSSetConstantBuffers(bufferNumber, 1, &constantBuffers[CONSTANT_BUFFER_WVP]);

	dxWrapper->GetContext()->Map(lineVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	verticesPtr = (vertex_PCN_t*)mappedResource.pData;
	memcpy(verticesPtr, (void*)vertices, (sizeof(vertex_PCN_t) * 2));
	dxWrapper->GetContext()->Unmap(lineVertexBuffer, 0);
	



	dxWrapper->GetContext()->Map(constantBuffers[CONSTANT_BUFFER_COLOR], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	ColorBufferType* pColorBufferData = (ColorBufferType*)mappedResource.pData;

	// copy to constant buffer
	pColorBufferData->color = color;
	//pColorBufferData->color = Scene::objects[i]->GetComponent<Renderer3D>().color;

	dxWrapper->GetContext()->Unmap(constantBuffers[CONSTANT_BUFFER_COLOR], 0);

	bufferNumber = 1;
	dxWrapper->GetContext()->VSSetConstantBuffers(bufferNumber, 1, &constantBuffers[CONSTANT_BUFFER_COLOR]);


	unsigned int stride = sizeof(vertex_PCN_t);
	unsigned int offset = 0;
	dxWrapper->GetContext()->IASetInputLayout(inputLayouts[1]);
	dxWrapper->GetContext()->VSSetShader(vertexShaders[0], nullptr, 0);
	dxWrapper->GetContext()->PSSetShader(pixelShaders[0], nullptr, 0);

	dxWrapper->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	dxWrapper->GetContext()->IASetVertexBuffers(0, 1, &lineVertexBuffer, &stride, &offset);
	dxWrapper->GetContext()->IASetIndexBuffer(lineIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	dxWrapper->GetContext()->DrawIndexed(2, 0, 0);
}


DirectX11Wrapper* Renderer::GetDevice()
{
	return dxWrapper;
}

void Renderer::SetDevice(DirectX11Wrapper* value)
{
	dxWrapper = value;
}