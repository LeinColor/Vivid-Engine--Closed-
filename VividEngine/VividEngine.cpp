#include "stdafx.h"
#include "VividEngine.h"
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
	cube->GetComponent<Transform>().Rotate(XMFLOAT3(0.3f, 0.2f, 0.1f));

	GameObject* sphere = new GameObject();
	sphere->AddComponent<Renderer3D>();
	sphere->GetComponent<Transform>().SetPosition(3, 0, 0);

	GameObject* cone = new GameObject();
	cone->AddComponent<Renderer3D>();
	cone->GetComponent<Transform>().SetPosition(-0.9, 0, -2);

	GameObject* light = new GameObject();
	light->AddComponent<Light>();
	light->GetComponent<Light>().attrib.ambientColor = XMFLOAT4(0.15f, 0.15f, 0.15f, 1.0f);
	light->GetComponent<Light>().attrib.diffuseColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	light->GetComponent<Light>().attrib.lightDirection = XMFLOAT3(1.0f, 1.0f, 0.5f);
	light->GetComponent<Light>().attrib.specularColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	light->GetComponent<Light>().attrib.specularPower = 32.0f;

	cube->GetComponent<Renderer3D>().mesh = Manager::meshes[MESH_CUBE];
	sphere->GetComponent<Renderer3D>().mesh = Manager::meshes[MESH_SPHERE];

	EditorObject* axisX = new EditorObject();
	EditorObject* axisY = new EditorObject();
	EditorObject* axisZ = new EditorObject();
	axisX->mesh = Manager::meshes[MESH_CYLINDER];
	axisY->mesh = Manager::meshes[MESH_CYLINDER];
	axisZ->mesh = Manager::meshes[MESH_CYLINDER];
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