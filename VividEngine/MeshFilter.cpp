#include <fstream>
#include "stdafx.h"
#include "MeshFilter.h"

MeshFilter::MeshFilter()
{
}

MeshFilter::MeshFilter(const MeshFilter& other)
{
}

MeshFilter::~MeshFilter()
{
}

bool MeshFilter::Initialize(ID3D11Device* device, const char* modelFilename)
{
	// load model
	if (!LoadModel(modelFilename))
	{
		return false;
	}

	// initialize vertex buffer and index buffer
	return InitializeBuffers(device);
}

void MeshFilter::Shutdown()
{
	// shutdown vertex buffer and index buffer
	ShutdownBuffers();

	// release model data
	ReleaseModel();
}

void MeshFilter::Render(ID3D11DeviceContext* deviceContext)
{
	// put vertex buffer and index buffer in pipeline to prepare drawing
	RenderBuffers(deviceContext);
}

int MeshFilter::GetIndexCount()
{
	return indexCount;
}

bool MeshFilter::LoadModel(const char* filename)
{
	// open model file
	std::ifstream fin;
	fin.open(filename);

	// terminate program if cannot open file
	if (fin.fail())
	{
		return false;
	}

	// read until before vertex count
	char input = 0;
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	// read vertex count
	fin >> vertexCount;

	// assign vertexCount to indexCount
	indexCount = vertexCount;

	// create model
	model = new ModelType[vertexCount];
	if (!model)
	{
		return false;
	}

	// read until before vertex data 
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// read vertex data
	for (int i = 0; i < vertexCount; i++)
	{
		fin >> model[i].x >> model[i].y >> model[i].z;
		fin >> model[i].tu >> model[i].tv;
		fin >> model[i].nx >> model[i].ny >> model[i].nz;
	}

	// close model file
	fin.close();

	return true;
}

bool MeshFilter::InitializeBuffers(ID3D11Device* device)
{
	// create vertices array
	VertexType* vertices = new VertexType[vertexCount];
	if (!vertices)
	{
		return false;
	}

	// create indices array
	unsigned long* indices = new unsigned long[indexCount];
	if (!indices)
	{
		return false;
	}

	// assign model data to vertex data and index data
	for (int i = 0; i < vertexCount; i++)
	{
		vertices[i].position = XMFLOAT3(model[i].x, model[i].y, model[i].z);
		vertices[i].texture = XMFLOAT2(model[i].tu, model[i].tv);
		vertices[i].normal = XMFLOAT3(model[i].nx, model[i].ny, model[i].nz);

		indices[i] = i;
	}

	// set static vertex buffer description
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// provide vertices pointer to subresource
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// create vertex buffer
	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer)))
	{
		return false;
	}

	// set static index buffer description
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// provide indices pointer to subresource
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// create index buffer
	if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer)))
	{
		return false;
	}

	// release vertices and indices
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void MeshFilter::ShutdownBuffers()
{
	// release index buffer
	if (indexBuffer)
	{
		indexBuffer->Release();
		indexBuffer = 0;
	}

	// release vertex buffer
	if (vertexBuffer)
	{
		vertexBuffer->Release();
		vertexBuffer = 0;
	}
}

void MeshFilter::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	// set vertex buffer's stride and offset
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	// set vertex buffer to enable in Input Assembler in order to rendering
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	// set index buffer to enable in Input Assembler in order to rendering
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// set primtive topology to triangle
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void MeshFilter::ReleaseModel()
{
	if (model)
	{
		delete[] model;
		model = 0;
	}
}