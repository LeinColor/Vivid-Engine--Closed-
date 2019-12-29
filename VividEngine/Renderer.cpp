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
#include "tiny_obj_loader.h"
using namespace vivid;


// Mesh and material conatiners
std::vector<Mesh*> meshes;
std::vector<Material*> materials;

// Shader containers
std::vector<ID3D11VertexShader*> vertexShaders;
std::vector<ID3D11PixelShader*> pixelShaders;
std::vector<ID3D11GeometryShader*> geometryShaders;
std::vector<ID3D11HullShader*> hullShaders;
std::vector<ID3D11DomainShader*> domainShaders;
std::vector<ID3D11ComputeShader*> computeShaders;

// Layout container
std::vector<ID3D11InputLayout*> inputLayouts;
std::vector<ID3D11Buffer*> constantBuffers;

std::vector<ID3D11Buffer*> resourceBuffers;
std::vector<ID3D11SamplerState*> samplerStates;

void Renderer::Initialize(Manager<GameObject>& manager)
{
	mainCamera = manager.At(0);

	// Load order sensitive because of enum value.
	LoadMesh("../VividEngine/Obj/cube.obj");
	LoadShader();
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
	meshes.push_back(mesh);
	
	//TODO: Not showing so should fix this!
	UINT indexCount = shapes[0].mesh.indices.size();
	vertex_PC_t* vertices = new vertex_PC_t[indexCount];	 // allocate vertex_t for vertex count
	unsigned long* indices = new unsigned long[indexCount];  // allocate unsigned long for indices count
	
	for (int i = 0; i < indexCount; i++) {
		auto& index = shapes[0].mesh.indices[i];

		// vertex position
		vertices[i].position = XMFLOAT3(
			attrib.vertices[index.vertex_index * 3 + 0],
			attrib.vertices[index.vertex_index * 3 + 1],
			attrib.vertices[index.vertex_index * 3 + 2]);

		// vertex cp;pr
		XMFLOAT4 color = XMFLOAT4(0.4, 0.4, 0.4, 0.6);
		if (index.texcoord_index >= 0) {
			color = XMFLOAT4(
				attrib.texcoords[index.texcoord_index * 2 + 0],
				attrib.texcoords[index.texcoord_index * 2 + 1],
				attrib.texcoords[index.texcoord_index * 2 + 0],
				1.0f);
		}
		vertices[i].color = color;

		indices[i] = i;
		
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
	
	//*************************************
	//**        Vertex Buffer            **
	//*************************************
	// Vertex buffer description definition
	{
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(vertex_PC_t) * indexCount;
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
	//***********************************
	//**       Vertex Shader           **
	//***********************************
	ID3D11VertexShader* vertexShader = nullptr;
	ID3D10Blob* vertexShaderBuffer = nullptr;
	if (FAILED(D3DCompileFromFile(L"../VividEngine/color.vs", nullptr, nullptr, "ColorVertexMain", "vs_5_0",
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
	ID3D11InputLayout* inputLayout = nullptr;
	D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	 0, 0,							  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	dxWrapper->GetDevice()->CreateInputLayout(layoutDesc, ARRAYSIZE(layoutDesc), vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &inputLayout);
	inputLayouts.push_back(inputLayout);



	//***********************************
	//**       Pixel Shader            **
	//***********************************
	ID3D11PixelShader* pixelShader = nullptr;
	ID3D10Blob* pixelShaderBuffer = nullptr;
	if (FAILED(D3DCompileFromFile(L"../VividEngine/color.ps", nullptr, nullptr, "ColorPixelMain", "ps_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, nullptr)))
	{
		Debug::Log("Pixel Shader load failed!");
		return;
	}
	dxWrapper->GetDevice()->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), nullptr, &pixelShader);
	pixelShaders.push_back(pixelShader);

	SAFE_RELEASE(vertexShaderBuffer);
	SAFE_RELEASE(pixelShaderBuffer);

	//***********************************
	//**      Constant Buffer          **
	//***********************************
	{
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(MatrixBufferType);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;
		dxWrapper->GetDevice()->CreateBuffer(&bd, nullptr, meshes[0]->GetMatrixBuffer());
//		constantBuffers.push_back(*meshes[0]->GetMatrixBuffer());
	}
}

void Renderer::Render(Manager<GameObject>& manager)
{
	static float angle = 0.001f;

	// Prepare to render
	dxWrapper->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);
	////////////////////////////////////////////////////////

	manager.At(1)->GetComponent<Transform>().Rotate(XMFLOAT3(0, angle, 0));
	mainCamera->GetComponent<Camera>().Render(dxWrapper->GetScreenWidth(), dxWrapper->GetScreenHeight(), SCREEN_DEPTH, SCREEN_NEAR);


	//*********************************
	//**      Constant Buffer        **
	//*********************************
	auto worldMatrix = manager.At(1)->GetComponent<Transform>().GetWorldMatrix(); // Cube's world matrix
	auto viewMatrix = mainCamera->GetComponent<Camera>().GetViewMatrix();
	auto projectionMatrix = mainCamera->GetComponent<Camera>().GetProjectionMatrix();

	// Transpose matrix to use within shader.
	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);
	
	// Lock constant buffer to write description.
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	dxWrapper->GetContext()->Map(*meshes[0]->GetMatrixBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// Get constant buffer pointer
	MatrixBufferType* dataPtr = (MatrixBufferType*)mappedResource.pData;

	// copy to constant buffer
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	dxWrapper->GetContext()->Unmap(*meshes[0]->GetMatrixBuffer(), 0);

	unsigned bufferNumber = 0;
	dxWrapper->GetContext()->VSSetConstantBuffers(bufferNumber, 1, meshes[0]->GetMatrixBuffer());

	

	// Bind pipeline before drawing
	unsigned int stride = sizeof(vertex_PC_t);
	unsigned int offset = 0;

	dxWrapper->GetContext()->IASetVertexBuffers(0, 1, meshes[0]->GetVertexBuffer(), &stride, &offset);
	dxWrapper->GetContext()->IASetIndexBuffer(*meshes[0]->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
	dxWrapper->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	dxWrapper->GetContext()->IASetInputLayout(inputLayouts[0]);
	dxWrapper->GetContext()->VSSetShader(vertexShaders[0], nullptr, 0);
	dxWrapper->GetContext()->PSSetShader(pixelShaders[0], nullptr, 0);
	
	dxWrapper->GetContext()->DrawIndexed(meshes[0]->positions.size(), 0, 0);
	
	/////////////////////////////////////////////////////
	// Render end
	dxWrapper->EndScene();
}



DirectX11Wrapper* Renderer::GetDevice()
{
	return dxWrapper;
}

void Renderer::SetDevice(DirectX11Wrapper* value)
{
	dxWrapper = value;
}