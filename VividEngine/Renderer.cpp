/**
	Renderer.cpp

	Purpose: Class responsible for rendering all objects.

	@author Lein
	@version 1.0 12/20/19
*/
#include "Renderer.h"
using namespace vivid;

extern AppHandle appHandle;

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

	uint32_t test = mainCamera->GetComponent<Camera>().type;
	std::string s = std::to_string(test);
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	std::wstring stemp = r.c_str();
	LPCWSTR result = stemp.c_str();
	MessageBox(appHandle.GetWindowHandle(), result, L"Debug", MB_OK);

	device->EndScene();
//	for (size_t i = 1; i < manager.GetSize(); i++) {
//		auto object = manager.At(i);
//		object->GetComponent<SpriteRenderer>().LoadSprite(device->GetDevice(), L"../VividEngine/data.png");
//	}
}