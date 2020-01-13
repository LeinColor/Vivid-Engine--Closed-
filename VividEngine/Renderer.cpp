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
#include "Manager.h"
#include "tiny_obj_loader.h"
using namespace vivid;
using namespace std;

DirectX11Wrapper* Renderer::graphics;
std::vector<GizmoLine> gizmoLines;

void Renderer::Render()
{
	// Set Main Camera
	mainCamera = Scene::GetMainCamera();

	// Prepare to render
	graphics->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	mainCamera->GetComponent<Camera>().Update(graphics->GetScreenWidth(), graphics->GetScreenHeight());

	XMMATRIX viewMatrix, projectionMatrix;
	viewMatrix = mainCamera->GetComponent<Camera>().GetViewMatrix();
	projectionMatrix = mainCamera->GetComponent<Camera>().GetProjectionMatrix();
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);

	for (int i = 0; i < Scene::objects.size(); i++) {
		// if object doesn't have Renderer3D component, skip immediately
		auto& renderer3D = Scene::objects[i]->GetComponent<Renderer3D>();
		if (&renderer3D == nullptr)
			continue;

		// if object doesn't have mesh, skip rendering
		auto& mesh = renderer3D.mesh;
		if (mesh == nullptr)
			continue;

		// Get object's world matrix and transpose it to render.
		auto worldMatrix = Scene::objects[i]->GetComponent<Transform>().GetWorldMatrix();
		worldMatrix = XMMatrixTranspose(worldMatrix);

		auto& material = renderer3D.material;
		auto& shader = material->shader;
		unsigned int stride = sizeof(vertex_PCN_t);
		unsigned int offset = 0;

		// Lock constant buffer to write description.
		MatrixBufferType cbMatrix = { worldMatrix, viewMatrix, projectionMatrix };
		graphics->UpdateBuffer(Manager::constantBuffers[CONSTANT_BUFFER_WVP], &cbMatrix, sizeof(MatrixBufferType));
		graphics->GetContext()->VSSetConstantBuffers(0, 1, &Manager::constantBuffers[CONSTANT_BUFFER_WVP]);

		if (shader == Manager::shaders[SHADER_BLINN_PHONG])
		{
			// Draw AABB box
			// AABB transformation by each object matrix
			AABB aabb = mesh->aabb.Transform(Scene::objects[i]->GetComponent<Transform>().GetWorldMatrix());
			XMFLOAT3 aabbVertices[8];
			for (int i = 0; i < 8; i++) {
				aabbVertices[i] = aabb.GetVertex(i);
			}
			XMFLOAT4 color{ 1,1,1,1 };
			GizmoLine line[12];
			line[0] = { aabbVertices[0], aabbVertices[1], color };
			line[1] = { aabbVertices[0], aabbVertices[4], color };
			line[2] = { aabbVertices[1], aabbVertices[5], color };
			line[3] = { aabbVertices[4], aabbVertices[5], color };
			line[4] = { aabbVertices[0], aabbVertices[3], color };
			line[5] = { aabbVertices[1], aabbVertices[2], color };
			line[6] = { aabbVertices[2], aabbVertices[3], color };
			line[7] = { aabbVertices[4], aabbVertices[7], color };
			line[8] = { aabbVertices[5], aabbVertices[6], color };
			line[9] = { aabbVertices[6], aabbVertices[7], color };
			line[10] = { aabbVertices[3], aabbVertices[7], color };
			line[11] = { aabbVertices[2], aabbVertices[6], color };

			for (int i = 0; i < 12; i++) {
				gizmoLines.push_back(line[i]);
			}

			CameraBufferType cbCamera = { mainCamera->GetComponent<Transform>().GetPosition(), 0.0f };
			graphics->UpdateBuffer(Manager::constantBuffers[CONSTANT_BUFFER_CAMERA], &cbCamera, sizeof(cbCamera));
			graphics->GetContext()->VSSetConstantBuffers(1, 1, &Manager::constantBuffers[CONSTANT_BUFFER_CAMERA]);

			LightBufferType cbLight;
			auto attrib = Scene::objects[7]->GetComponent<Light>().attrib;
			cbLight = { attrib.ambientColor, attrib.diffuseColor, attrib.lightDirection, attrib.specularPower, attrib.specularColor };
			graphics->UpdateBuffer(Manager::constantBuffers[CONSTANT_BUFFER_LIGHT], &cbLight, sizeof(cbLight));
			graphics->GetContext()->PSSetConstantBuffers(0, 1, &Manager::constantBuffers[CONSTANT_BUFFER_LIGHT]);
		}
		else if (shader == Manager::shaders[SHADER_DEBUG])
		{
			// gizmo drawing (absolute scale)
			XMVECTOR vk = XMLoadFloat3(&Scene::objects[i]->GetComponent<Transform>().GetPosition());
			XMVECTOR result = XMVector3Transform(vk, mainCamera->GetComponent<Camera>().GetViewProjectionMatrix());
			float w = XMVectorGetW(result) / 64;
			Scene::objects[i]->GetComponent<Transform>().SetScale(w / 2, w, w / 2);

			ColorBufferType cbColor = { Scene::objects[i]->GetComponent<Renderer3D>().color };
			graphics->UpdateBuffer(Manager::constantBuffers[CONSTANT_BUFFER_COLOR], &cbColor, sizeof(cbColor));
			graphics->GetContext()->VSSetConstantBuffers(1, 1, &Manager::constantBuffers[CONSTANT_BUFFER_COLOR]);
		}

		graphics->GetContext()->IASetVertexBuffers(0, 1, mesh->GetVertexBuffer(), &stride, &offset);
		graphics->GetContext()->IASetInputLayout(*shader->inputLayouts);
		graphics->GetContext()->IASetIndexBuffer(*mesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
		graphics->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		graphics->GetContext()->VSSetShader(*shader->vertexShader, nullptr, 0);
		graphics->GetContext()->PSSetShader(*shader->pixelShader, nullptr, 0);
		graphics->GetContext()->DrawIndexed(mesh->positions.size(), 0, 0);
	}

	// Render gizmo lines
	for (auto& line : gizmoLines) {
		DrawLine(line.startPoint, line.endPoint, line.color);
	}

	// Clear gizmo line queue
	gizmoLines.clear();

	// Render end
	graphics->EndScene();
	
}

void Renderer::DrawLine(XMFLOAT3 startPoint, XMFLOAT3 endPoint, XMFLOAT4 color)
{
	vertex_P_t* vertices = new vertex_P_t[2];
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	vertex_P_t* verticesPtr;

	vertices[0].position = startPoint;
	vertices[1].position = endPoint;

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
	graphics->GetContext()->Map(Manager::constantBuffers[CONSTANT_BUFFER_WVP], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// Get constant buffer pointer
	MatrixBufferType* pMatrixBufferData = (MatrixBufferType*)mappedResource.pData;

	// copy to constant buffer
	pMatrixBufferData->world = worldMatrix;
	pMatrixBufferData->view = viewMatrix;
	pMatrixBufferData->projection = projectionMatrix;

	graphics->GetContext()->Unmap(Manager::constantBuffers[CONSTANT_BUFFER_WVP], 0);

	unsigned bufferNumber = 0;
	graphics->GetContext()->VSSetConstantBuffers(bufferNumber, 1, &Manager::constantBuffers[CONSTANT_BUFFER_WVP]);

	graphics->GetContext()->Map(*Manager::meshes[MESH_LINE]->GetVertexBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	verticesPtr = (vertex_P_t*)mappedResource.pData;
	memcpy(verticesPtr, (void*)vertices, (sizeof(vertex_P_t) * 2));
	graphics->GetContext()->Unmap(*Manager::meshes[MESH_LINE]->GetVertexBuffer(), 0);
	



	graphics->GetContext()->Map(Manager::constantBuffers[CONSTANT_BUFFER_COLOR], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	ColorBufferType* pColorBufferData = (ColorBufferType*)mappedResource.pData;

	// copy to constant buffer
	pColorBufferData->color = color;
	//pColorBufferData->color = Scene::objects[i]->GetComponent<Renderer3D>().color;

	graphics->GetContext()->Unmap(Manager::constantBuffers[CONSTANT_BUFFER_COLOR], 0);

	bufferNumber = 1;
	graphics->GetContext()->VSSetConstantBuffers(bufferNumber, 1, &Manager::constantBuffers[CONSTANT_BUFFER_COLOR]);


	unsigned int stride = sizeof(vertex_P_t);
	unsigned int offset = 0;
	graphics->GetContext()->IASetInputLayout(Manager::inputLayouts[IL_POS]);
	graphics->GetContext()->VSSetShader(Manager::vertexShaders[VS_DEBUG], nullptr, 0);
	graphics->GetContext()->PSSetShader(Manager::pixelShaders[PS_DEBUG], nullptr, 0);

	graphics->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	graphics->GetContext()->IASetVertexBuffers(0, 1, Manager::meshes[MESH_LINE]->GetVertexBuffer(), &stride, &offset);
	graphics->GetContext()->IASetIndexBuffer(*Manager::meshes[MESH_LINE]->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
	graphics->GetContext()->DrawIndexed(2, 0, 0);
}


DirectX11Wrapper* Renderer::GetGraphicsInterface()
{
	return graphics;
}

void Renderer::SetGraphicsInterface(DirectX11Wrapper* value)
{
	graphics = value;
}

void Renderer::AddGizmoLine(const GizmoLine& line) {
	gizmoLines.push_back(line);
}