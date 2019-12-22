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

void Renderer::Initialize(Manager<GameObject>& manager3D)
{
	mainCamera = manager3D.At(0);
}

void Renderer::Render2D(Manager<Sprite>& manager2D)
{
	device->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);
	mainCamera->GetComponent<Camera>().Render(device->GetScreenWidth(), device->GetScreenHeight(), SCREEN_DEPTH, SCREEN_NEAR);

	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;

	
	for (size_t i = 0; i < manager2D.GetSize(); i++) {
		auto object = manager2D.At(i);
		object->GetComponent<SpriteRenderer>().LoadSprite(device->GetDevice(), L"../VividEngine/data.png");

	}
}

void Renderer::Render3D(Manager<GameObject>& manager3D)
{

}