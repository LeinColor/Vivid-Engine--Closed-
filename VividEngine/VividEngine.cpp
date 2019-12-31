#include "stdafx.h"
#include "VividEngine.h"
#include "GameObject.h"
#include "Texture.h"
#include "TextureShader.h"
using namespace vivid;

void VividEngine::Initialize()
{
	if (renderer.GetDevice() == nullptr)
		renderer.SetDevice(new DirectX11Wrapper(AppHandle::GetWindowHandle(), false));
	
	// initialize renderer to load mesh, shader
	renderer.Initialize();

	// 3D
	GameObject* camera = new GameObject();
	camera->AddComponent<Camera>();
	camera->GetComponent<Transform>().SetPosition(0, 0, -15);
	
	GameObject* cube = new GameObject();
	cube->AddComponent<Renderer3D>();

	GameObject* sphere = new GameObject();
	sphere->AddComponent<Renderer3D>();
	sphere->GetComponent<Transform>().SetPosition(3, 0, 0);

	GameObject* cone = new GameObject();
	cone->AddComponent<Renderer3D>();
	cone->GetComponent<Transform>().SetPosition(-0.9, 0, -2);

	cube->GetComponent<Renderer3D>().mesh = Manager::meshes[MESH_CUBE];
	sphere->GetComponent<Renderer3D>().mesh = Manager::meshes[MESH_SPHERE];
	//cone->GetComponent<Renderer3D>().mesh = Manager::meshes[MESH_CONE];

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

	timer += 0.001f;

	if (timer >= 1.0f) {
		timer = 0;
		// Render all of 2D objects
		renderer.Render();
	}
}

void VividEngine::SetWindow(HWND hWnd, HINSTANCE hInstance)
{
	AppHandle::SetWindowHandle(hWnd);
	AppHandle::SetInstanceHandle(hInstance);
}