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

ID3D11Buffer* vertexBuffer;
ID3D11Buffer* indexBuffer;
ID3D11Buffer* matrixBuffer;

// Define struct here
struct vertex_PC_t {	// Position,  Color
	XMFLOAT3 position;
	XMFLOAT4 color;
};

struct vertex_PT_t {	// Position, Texture
	XMFLOAT3 position;
	XMFLOAT2 texture;
};

struct vertex_PCN_t {	// Position, Texture, Color
	XMFLOAT3 position;
	XMFLOAT4 color;
	XMFLOAT3 normal;
};

struct vertex_PTN_t {	// Position, Texture, Normal
	XMFLOAT3 position;
	XMFLOAT2 texture;
	XMFLOAT3 normal;
};

struct MatrixBufferType {
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX projection;
};

void Renderer::Initialize(Manager<GameObject>& manager)
{
	mainCamera = manager.At(0);

	LoadMesh(manager);
}

void Renderer::Render(Manager<GameObject>& manager)
{
	// Prepare to render
	dxWrapper->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);
	////////////////////////////////////////////////////////

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
	dxWrapper->GetContext()->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// Get constant buffer pointer
	MatrixBufferType* dataPtr = (MatrixBufferType*)mappedResource.pData;

	// copy to constant buffer
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	dxWrapper->GetContext()->Unmap(matrixBuffer, 0);

	unsigned bufferNumber = 0;
	dxWrapper->GetContext()->VSSetConstantBuffers(bufferNumber, 1, &matrixBuffer);

	

	// Bind pipeline before drawing
	unsigned int stride = sizeof(vertex_PC_t);
	unsigned int offset = 0;

	dxWrapper->GetContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	dxWrapper->GetContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	dxWrapper->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	dxWrapper->GetContext()->IASetInputLayout(inputLayouts[0]);
	dxWrapper->GetContext()->VSSetShader(vertexShaders[0], nullptr, 0);
	dxWrapper->GetContext()->PSSetShader(pixelShaders[0], nullptr, 0);
	
	dxWrapper->GetContext()->DrawIndexed(meshes[0]->vertices.size(), 0, 0);
	
	/////////////////////////////////////////////////////
	// Render end
	dxWrapper->EndScene();
}

void Renderer::LoadMesh(Manager<GameObject>& manager)
{
	// Create new mesh instance
	Mesh* mesh = new Mesh();
	meshes.push_back(mesh);

	// Load cube obj
	bool success = Importer::LoadObjFile("../VividEngine/cube.obj", mesh->vertices, mesh->uvs, mesh->normals);

	if (!success) // exit if file import failed
	{
		Debug::Log("Obj file import failed!");
		return;
	}


	//TODO: Not showing so should fix this!
	UINT vertexCount = mesh->vertices.size();
	UINT indexCount = vertexCount;
	vertex_PC_t* vertices = new vertex_PC_t[vertexCount];	 // allocate vertex_t for vertex count.
	unsigned long* indices = new unsigned long[indexCount];  // index count is same with vertex count.

	for (int i = 0; i < vertexCount; i++) {
		vertices[i].position = mesh->vertices[i];
		vertices[i].color = XMFLOAT4(mesh->uvs[i].x, mesh->uvs[i].y, mesh->uvs[i].x, 1.0f);
		indices[i] = i;
	}

	//*************************************
	//**        Vertex Buffer            **
	//*************************************
	// Vertex buffer description definition
	{
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(vertex_PC_t) * vertexCount;
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
		dxWrapper->GetDevice()->CreateBuffer(&bd, &sd, &vertexBuffer);
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
		dxWrapper->GetDevice()->CreateBuffer(&bd, &sd, &indexBuffer);
	}

	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE_ARRAY(indices);



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
		dxWrapper->GetDevice()->CreateBuffer(&bd, nullptr, &matrixBuffer);
		constantBuffers.push_back(matrixBuffer);
	}
}

DirectX11Wrapper* Renderer::GetDevice()
{
	return dxWrapper;
}

void Renderer::SetDevice(DirectX11Wrapper* value)
{
	dxWrapper = value;
}