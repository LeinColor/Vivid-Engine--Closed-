#include "../VividEngine/Importer.h"
#include "../VividEngine/AppHandle.h"
#include "../VividEngine/Resources.h"
#include "../VividEngine/Components.h"

#include "../ThirdParty/entt/entt.hpp"

#include "SAMPLE01_DrawingTriangle.h"

void SAMPLE01_DrawingTriangleScene::Start()
{
	// Camera
	auto enttCamera = registry.create();
	registry.assign<Transform>(enttCamera,
		XMFLOAT3(0.0f, 0.0f, 0.0f),			// Position
		XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f),	// Rotation
		XMFLOAT3(1.0f, 1.0f, 1.0),			// Scale
		IDENTITY_MATRIX);					// World

	registry.assign<Camera>(enttCamera);

	// Cube
	auto enttCube = registry.create();
	registry.assign<Transform>(enttCube,
		XMFLOAT3(0.0f, 0.0f, -5.0f),		// Position
		XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f),	// Rotation
		XMFLOAT3(1.0f, 1.0f, 1.0),			// Scale
		IDENTITY_MATRIX);					// World

	registry.assign<Renderer3D>(enttCube,
		Resources::GetMeshID("Cube"),
		Resources::GetShaderID("Debug"));

	// Set Window Title
	SetWindowTextA(AppHandle::GetWindowHandle(), "SAMPLE01: Drawing a Triangle");
}

void SAMPLE01_DrawingTriangleScene::Update()
{

}

void SAMPLE01_DrawingTriangleScene::FixedUpdate()
{

}