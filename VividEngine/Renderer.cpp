/**
	Renderer.cpp

	Purpose: Class responsible for rendering all objects.

	@author Lein
	@version 1.0 12/20/19
*/
#include "stdafx.h"
#include "Renderer.h"
#include "ComponentSystem.h"
#include "Components.h"
#include "Debug.h"

#include "../ThirdParty/entt/entt.hpp"
using namespace vivid;
using namespace std;

GraphicsAPI* Renderer::graphics;

ID3D11SamplerState* sampleState;
ID3D11Buffer* transformCB;

void Renderer::Initialize()
{
	graphics = new GraphicsAPI(AppHandle::GetWindowHandle(), false);

	InitializeSampleStates();
	InitializeConstantBuffers();
}

void Renderer::Shutdown()
{}

void Renderer::InitializeConstantBuffers()
{
	{
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(TransformConstants);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;
		graphics->CreateBuffer(&bd, nullptr, &transformCB);
	}
}

void Renderer::InitializeSampleStates()
{
	{
		D3D11_SAMPLER_DESC samplerDesc;
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		graphics->CreateSamplerState(&samplerDesc, &sampleState);
	}
}

void Renderer::Render()
{
	entt::registry& registry = scene->registry;

	if (registry.view<Transform, Camera>().empty())
		return;

	enttMainCamera = scene->enttMainCamera;

	auto& camera = registry.view<Transform, Camera>().get<Camera>(enttMainCamera);
	auto& cameraTransform = registry.view<Transform, Camera>().get<Transform>(enttMainCamera);
	ComponentSystem::Update(cameraTransform, camera);

	view = XMMatrixTranspose(XMLoadFloat4x4(&camera.view));
	projection = XMMatrixTranspose(XMLoadFloat4x4(&camera.projection));

	graphics->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	auto entityObjects = registry.view<Transform, Renderer3D>();

	for (auto entity : entityObjects) {
		auto& transform = entityObjects.get<Transform>(entity);
		auto& renderer3D = entityObjects.get<Renderer3D>(entity);

		if (renderer3D.meshID == INVALID_MESH_ID || renderer3D.shaderID == INVALID_SHADER_ID)
			return;

		ComponentSystem::Update(transform);

		world = XMMatrixTranspose(XMLoadFloat4x4(&transform.world));

		Mesh& mesh = Resources::GetMesh(renderer3D.meshID);
		Shader& shader = Resources::GetShader(renderer3D.shaderID);
		Texture& texture = Resources::GetTexture(renderer3D.textureID);

		switch (shader.inputLayoutType) {
			case POS:
			{
				ID3D11Buffer* vbs[] = {
					mesh.vertexBufferPos,
				};
				UINT strides[] = {
					sizeof(XMFLOAT3),
				};
				UINT offsets[] = {
					0,
				};
				graphics->GetContext()->IASetVertexBuffers(0, ARRAYSIZE(vbs), vbs, strides, offsets);
			}
			break;

			case POS_TEX:
			{
				ID3D11Buffer* vbs[] = {
					mesh.vertexBufferPos,
					mesh.vertexBufferTex,
				};
				UINT strides[] = {
					sizeof(XMFLOAT3),
					sizeof(XMFLOAT2),
				};
				UINT offsets[] = {
					0,
					0,
				};
				graphics->GetContext()->IASetVertexBuffers(0, ARRAYSIZE(vbs), vbs, strides, offsets);
				graphics->GetContext()->PSSetSamplers(0, 1, &sampleState);
				graphics->GetContext()->PSSetShaderResources(0, 1, texture.GetData());
			}
			break;
		}

		graphics->GetContext()->IASetIndexBuffer(mesh.indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		graphics->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		graphics->GetContext()->IASetInputLayout(shader.inputLayout);

		TransformConstants tc = { world, view, projection };
		UpdateConstantBuffers(tc);
		graphics->GetContext()->VSSetConstantBuffers(0, 1, &transformCB);

		graphics->GetContext()->VSSetShader(shader.subShaders.vs, NULL, 0);
		graphics->GetContext()->PSSetShader(shader.subShaders.ps, NULL, 0);
		graphics->GetContext()->DrawIndexed(mesh.positions.size(), 0, 0);
	}
	graphics->EndScene();
}

void Renderer::UpdateVertexBuffer(Mesh& mesh)
{
	UINT indexCount = mesh.positions.size();

	// Update vertex position
	if (mesh.positions.size() > 0) {
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(XMFLOAT3) * indexCount;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = mesh.positions.data();
		data.SysMemPitch = 0;
		data.SysMemSlicePitch = 0;

		graphics->CreateBuffer(&bd, &data, &mesh.vertexBufferPos);
	}

	// Update vertex color
	if (mesh.colors.size() > 0) {
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(XMFLOAT4) * indexCount;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = mesh.colors.data();
		data.SysMemPitch = 0;
		data.SysMemSlicePitch = 0;

		graphics->CreateBuffer(&bd, &data, &mesh.vertexBufferCol);
	}

	// Update vertex texture
	if (mesh.texcoords.size() > 0) {
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(XMFLOAT2) * indexCount;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = mesh.texcoords.data();
		data.SysMemPitch = 0;
		data.SysMemSlicePitch = 0;

		graphics->CreateBuffer(&bd, &data, &mesh.vertexBufferTex);
	}

	// Update vertex normal
	if (mesh.normals.size() > 0) {
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(XMFLOAT3) * indexCount;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = mesh.normals.data();
		data.SysMemPitch = 0;
		data.SysMemSlicePitch = 0;

		graphics->CreateBuffer(&bd, &data, &mesh.vertexBufferNor);
	}
}

void Renderer::UpdateIndexBuffer(Mesh& mesh)
{
	UINT indexCount = mesh.positions.size();

	unsigned long* indices = new unsigned long[indexCount];

	for (UINT i = 0; i < indexCount; i++) {
		indices[i] = i;
	}

	// Update index buffer
	{
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned long) * indexCount;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = indices;
		data.SysMemPitch = 0;
		data.SysMemSlicePitch = 0;

		graphics->CreateBuffer(&bd, &data, &mesh.indexBuffer);
	}
}

// TODO: support all of layout using shader reflection later.
void Renderer::UpdateInputLayout(Shader& shader)
{
	HRESULT hr = E_FAIL;

	switch (shader.inputLayoutType) {
	case DEFAULT:
	{
		// SemanticName, SemanticIndex, Format, InputSlot, AlignedByteOffset, InputSlotClass, InstanceDataStepRate
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD" , 0, DXGI_FORMAT_R32G32_FLOAT		, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL"	, 0, DXGI_FORMAT_R32G32B32_FLOAT	, 2, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		graphics->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), shader.blobs.vs->GetBufferPointer(), shader.blobs.vs->GetBufferSize(), &shader.inputLayout);
		break;
	}
	case POS:
	{
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		graphics->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), shader.blobs.vs->GetBufferPointer(), shader.blobs.vs->GetBufferSize(), &shader.inputLayout);
		break;
	}
	case POS_TEX:
	{
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD" , 0, DXGI_FORMAT_R32G32_FLOAT		, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		graphics->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), shader.blobs.vs->GetBufferPointer(), shader.blobs.vs->GetBufferSize(), &shader.inputLayout);
		break;
	}
	case POS_TEX_NORMAL:
	{
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD" , 0, DXGI_FORMAT_R32G32_FLOAT		, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL"	, 0, DXGI_FORMAT_R32G32B32_FLOAT	, 2, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		graphics->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), shader.blobs.vs->GetBufferPointer(), shader.blobs.vs->GetBufferSize(), &shader.inputLayout);
		break;
	}
	case UNDEFINED_INPUT_LAYOUT:
		Debug::Log("Exception: You tried to create undefined input layout.");
		break;
	}
}

void Renderer::UpdateShader(Shader& shader)
{
	if (shader.shaderFlag & VERTEX_SHADER)
	{
		graphics->CreateVertexShader(shader.blobs.vs->GetBufferPointer(), shader.blobs.vs->GetBufferSize(), &shader.subShaders.vs);
	}
	if (shader.shaderFlag & PIXEL_SHADER)
	{
		graphics->CreatePixelShader(shader.blobs.ps->GetBufferPointer(), shader.blobs.ps->GetBufferSize(), &shader.subShaders.ps);
	}
	if (shader.shaderFlag & GEOMETRY_SHADER)
	{
		graphics->CreateGeometryShader(shader.blobs.gs->GetBufferPointer(), shader.blobs.gs->GetBufferSize(), &shader.subShaders.gs);
	}
	if (shader.shaderFlag & HULL_SHADER)
	{
		graphics->CreateHullShader(shader.blobs.hs->GetBufferPointer(), shader.blobs.hs->GetBufferSize(), &shader.subShaders.hs);
	}
	if (shader.shaderFlag & DOMAIN_SHADER)
	{
		graphics->CreateDomainShader(shader.blobs.ds->GetBufferPointer(), shader.blobs.ds->GetBufferSize(), &shader.subShaders.ds);
	}
	if (shader.shaderFlag & COMPUTE_SHADER)
	{
		graphics->CreateComputeShader(shader.blobs.cs->GetBufferPointer(), shader.blobs.cs->GetBufferSize(), &shader.subShaders.cs);
	}
}

void Renderer::Apply()
{
	// update all of mesh's vertex buffer and index buffer
	for (size_t i = 0; i < Resources::GetMeshCount(); i++) {
		auto& mesh = Resources::GetMesh(i);

		UpdateVertexBuffer(mesh);
		UpdateIndexBuffer(mesh);
	}

	// update all of shader's input layout
	for (size_t i = 0; i < Resources::GetShaderCount(); i++) {
		auto& shader = Resources::GetShader(i);

		UpdateInputLayout(shader);
		UpdateShader(shader);
	}
}


void Renderer::UpdateConstantBuffers(TransformConstants& transform)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	graphics->GetContext()->Map(transformCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	TransformConstants* data = (TransformConstants*)mappedResource.pData;

	data->worldMatrix = transform.worldMatrix;
	data->viewMatrix = transform.viewMatrix;
	data->projectionMatrix = transform.projectionMatrix;

	graphics->GetContext()->Unmap(transformCB, 0);
}