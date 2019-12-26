#include "stdafx.h"
#include "VividEngine.h"
#include "GameObject.h"
#include "Texture.h"
#include "TextureShader.h"
//#include "Global.h"

using namespace vivid;

void VividEngine::Initialize()
{
	if (renderer.GetDevice() == nullptr)
		renderer.SetDevice(new DirectX11Wrapper(AppHandle::GetWindowHandle(), false));
	
	// 3D
	GameObject* camera = new GameObject();
	camera->AddComponent<Transform>();
	camera->AddComponent<Camera>();
	camera->GetComponent<Transform>().SetPosition(0, 0, -5);
	camera->GetComponent<Camera>().owner = camera;
	objectManager.Add(camera);
	
	GameObject* cube = new GameObject();
	cube->AddComponent<Transform>();
	cube->AddComponent<Renderer3D>();
	objectManager.Add(cube);

	/*
	// 2D
	GameObject* sprite = new GameObject();
	sprite->AddComponent<Transform>();
	sprite->AddComponent<SpriteRenderer>();
	objectManager.Add(sprite);
	*/

	renderer.Initialize(objectManager);

	initialized = true;
}

void VividEngine::Run()
{
	if (!initialized) {
		Initialize();
	}
	Update();
}

void VividEngine::Update()
{
	static float timer = 0.0f;

	timer += 0.1f;

	if (timer >= 1.0f) {
		timer = 0;
		// Render all of 2D objects
		renderer.Render(objectManager);
	}
}

void VividEngine::SetWindow(HWND hWnd, HINSTANCE hInstance)
{
	AppHandle::SetWindowHandle(hWnd);
	AppHandle::SetInstanceHandle(hInstance);
}