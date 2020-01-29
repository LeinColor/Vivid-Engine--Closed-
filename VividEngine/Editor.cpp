#include "stdafx.h"
#include "GraphicsAPI.h"
#include "Renderer.h"
#include "Editor.h"
#include "Time.h"
#include "Input.h"
#include "Importer.h"

#include "Scene.h"
#include "ECS.h"
#include "Components.h"

#include <unordered_map>
using namespace vivid;

void Editor::Start()
{
	Scene* scene = new Scene();
	renderer.SetScene(scene);

	//=======================================
	// Mesh
	MeshComponent& meshCube = Importer::LoadObjFile("../VividEngine/Obj/cube.obj", scene);
	//=======================================
	// Shader
	ShaderComponent& shader = Importer::LoadShaderFile("Debug", INPUT_LAYOUT_TYPE::POS, scene);
	//=======================================
	// Camera
	Entity cameraEntity = ECS::CreateEntity();
	ObjectComponent& objCamera = scene->objects.Create(cameraEntity);
	TransformComponent& tfCamera = scene->transforms.Create(cameraEntity);
	tfCamera.SetPosition(0.0f, 0.0f, -5.0f);

	CameraComponent& camera = scene->cameras.Create(cameraEntity);
	camera.transform = tfCamera;
	//=======================================
	// Cube
	Entity cubeEntity = ECS::CreateEntity();
	ObjectComponent& objCube = scene->objects.Create(cubeEntity);
	TransformComponent& tfCube = scene->transforms.Create(cubeEntity);
	tfCube.SetPosition(0.0f, 0.0f, 0.0f);

	objCube.meshEntity = scene->meshes.GetEntity(0);
	objCube.shaderEntity = scene->shaders.GetEntity(0);


	//=======================================

	renderer.Apply();
}

void Editor::Initialize()
{
	// initialize renderer to load mesh, shader
	time.Initialize();
	input.Initialize();
	renderer.Initialize();
	initialized = true;
}

void Editor::Run()
{
	if (!initialized) {
		Initialize();
		Start();
	}

	time.deltaTime = float(std::max(0.0, time.Elapsed() / 1000.0));
	time.Record();
	
	deltaTimeAccumulator += time.deltaTime;
	if (deltaTimeAccumulator > 10)
	{
		deltaTimeAccumulator = 0;
	}

	const float targetFrameRateInv = 1.0f / 60.0f;

	// call function for frame per second
	if (deltaTimeAccumulator >= targetFrameRateInv)
	{
		renderer.Render();
	}
	// call function for frame per second, but if lag occurred it will be calculated for skipped time line.
	while (deltaTimeAccumulator >= targetFrameRateInv)
	{
		FixedUpdate();	// calculate physics here
		//Update();
		deltaTimeAccumulator -= targetFrameRateInv;
	}
	Update();
}

void Editor::FixedUpdate()
{
}

void Editor::Update()
{
}

void Editor::SetWindow(HWND hWnd, HINSTANCE hInstance)
{
	AppHandle::SetWindowHandle(hWnd);
	AppHandle::SetInstanceHandle(hInstance);
}