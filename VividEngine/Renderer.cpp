/**
	Renderer.cpp

	Purpose: Class responsible for rendering all objects.

	@author Lein
	@version 1.0 12/20/19
*/
#include "Renderer.h"

using namespace vivid;

DirectX11Wrapper* Renderer::GetDevice()
{
	return device;
}

void Renderer::SetDevice(DirectX11Wrapper* value)
{
	device = value;
}

void Renderer::Initialize(Manager<GameObject>& manager)
{
	mainCamera = manager.At(0);
}

void Renderer::Render(Manager<GameObject>& manager)
{
	device->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);
	mainCamera->GetComponent<Camera>().Render(device->GetScreenWidth(), device->GetScreenHeight(), SCREEN_DEPTH, SCREEN_NEAR);

	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	worldMatrix = mainCamera->GetComponent<Transform>().GetWorldMatrix();
	viewMatrix = mainCamera->GetComponent<Camera>().GetViewMatrix();
	projectionMatrix = mainCamera->GetComponent<Camera>().GetProjectionMatrix();
	
	for (size_t i = 1; i < manager.GetSize(); i++) {
		auto object = manager.At(i);
		object->GetComponent<SpriteRenderer>().LoadSprite(device->GetDevice(), L"../VividEngine/data.png");

	}
}