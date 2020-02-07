#include "../VividEngine/stdafx.h"
#include "../VividEngine/GraphicsAPI.h"
#include "../VividEngine/Renderer.h"
#include "../VividEngine/Time.h"
#include "../VividEngine/Input.h"
#include "../VividEngine/Importer.h"

#include "../VividEngine/Scene.h"
#include "../VividEngine/Components.h"

#include "SAMPLE01_DrawingTriangle.h"
#include "SAMPLE02_DrawingCube.h"
#include "SAMPLE03_Texture.h"

#include "Entrance.h"
#include <unordered_map>
using namespace vivid;

void Entrance::Start()
{
	// Load meshes
	Importer::LoadObjFile("Triangle", "../VividEngine/Obj/triangle.obj");
	Importer::LoadObjFile("Cube", "../VividEngine/Obj/cube.obj");

	// Load shaders
	Importer::LoadShaderFile("Debug", "Debug", INPUT_LAYOUT_TYPE::POS);
	Importer::LoadShaderFile("Texture", "Texture", INPUT_LAYOUT_TYPE::POS_TEX);

	// Load textures
	Importer::LoadTextureFile("test", L"../VividEngine/image/dd.png");
	
	// Initilize scenes
	Scene* sampleScene01 = new SAMPLE01_DrawingTriangleScene();
	Scene* sampleScene02 = new SAMPLE02_DrawingCubeScene();
	Scene* sampleScene03 = new SAMPLE03_TextureScene();


	//=======================================
	currentScene = sampleScene03;
	currentScene->Start();

	renderer.SetScene(currentScene);
	renderer.Apply();
}

void Entrance::Initialize()
{
	// initialize renderer to load mesh, shader
	time.Initialize();
	input.Initialize();
	renderer.Initialize();
	initialized = true;
}

void Entrance::Run()
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

void Entrance::FixedUpdate()
{
	currentScene->FixedUpdate();
}

void Entrance::Update()
{
	currentScene->Update();
}

void Entrance::SetWindow(HWND hWnd, HINSTANCE hInstance)
{
	AppHandle::SetWindowHandle(hWnd);
	AppHandle::SetInstanceHandle(hInstance);
}