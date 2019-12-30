#include "stdafx.h"
#include "VividEngine.h"
#include "GameObject.h"
#include "Texture.h"
#include "TextureShader.h"
//#include "Global.h"

using namespace vivid;

vector<GameObject*> Manager::gameObjects;
vector<Mesh*> Manager::meshes;
vector<Texture*> Manager::textures;
vector<Shader*> Manager::shaders;
void VividEngine::Initialize()
{
	if (renderer.GetDevice() == nullptr)
		renderer.SetDevice(new DirectX11Wrapper(AppHandle::GetWindowHandle(), false));
	
	// 3D
	GameObject* camera = new GameObject();
	camera->AddComponent<Transform>();
	camera->AddComponent<Camera>();
	camera->GetComponent<Transform>().SetPosition(0, 0, -15);
	camera->GetComponent<Camera>().owner = camera;
	Manager::gameObjects.push_back(camera);
	
	GameObject* cube = new GameObject();
	cube->AddComponent<Transform>();
	cube->AddComponent<Renderer3D>();
	Manager::gameObjects.push_back(cube);

	GameObject* sphere = new GameObject();
	sphere->AddComponent<Transform>();
	sphere->AddComponent<Renderer3D>();
	Manager::gameObjects.push_back(sphere);

	renderer.Initialize();

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