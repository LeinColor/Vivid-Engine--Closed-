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
#include "OBJ_Loader.h"
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


void Renderer::Initialize(Manager<GameObject>& manager)
{
	mainCamera = manager.At(0);

	LoadMesh(manager);
}


void Renderer::LoadMesh(Manager<GameObject>& manager)
{
	// Initialize loader
	objl::Loader loader;
	bool success = loader.LoadFile("../VividEngine/Obj/teapot.obj");

	if (!success) {
		Debug::Log("Obj file import failed!");
		return;
	}

	Mesh* mesh = new Mesh();
	meshes.push_back(mesh);
	
	//TODO: Not showing so should fix this!
	UINT vertexCount = loader.LoadedVertices.size();
	UINT indexCount = loader.LoadedIndices.size();
	vertex_PC_t* vertices = new vertex_PC_t[vertexCount];	 // allocate vertex_t for vertex count
	unsigned long* indices = new unsigned long[indexCount];  // allocate unsigned long for indices count

	meshes[0]->indexCount = indexCount;

	for (int i = 0; i < vertexCount; i++) {
		objl::Vertex vertex = loader.LoadedVertices[i];
		vertices[i].position = XMFLOAT3(vertex.Position.X, vertex.Position.Y, vertex.Position.Z);
		vertices[i].color = XMFLOAT4(vertex.TextureCoordinate.X + 0.15f, vertex.TextureCoordinate.Y + 0.15f, vertex.TextureCoordinate.X + 0.15f, 1.0f);
	}

	for (int i = 0; i < indexCount; i++) {
		indices[i] = loader.LoadedIndices[i];
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
	
	dxWrapper->GetContext()->DrawIndexed(meshes[0]->indexCount, 0, 0);
	
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