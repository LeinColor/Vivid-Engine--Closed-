#include "../VividEngine/Importer.h"
#include "../VividEngine/AppHandle.h"
#include "../VividEngine/Resources.h"
#include "../VividEngine/Components.h"

#include "../ThirdParty/entt/entt.hpp"

#include "SAMPLE01_DrawingTriangle.h"

void SAMPLE01_DrawingTriangleScene::Start()
{
	// Set Window Title
	SetTitle("SAMPLE01: Drawing a Triangle");

	// Camera
	enttMainCamera = registry.create();
	registry.assign<Transform>(enttMainCamera,
		XMFLOAT3(0.0f, 0.0f, 0.0f),			// Position
		XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f),	// Rotation (Quaternion)
		XMFLOAT3(1.0f, 1.0f, 1.0),			// Scale
		IDENTITY_MATRIX);					// World

	registry.assign<Camera>(enttMainCamera);

	// Triangle
	auto enttTriangle = registry.create();
	registry.assign<Transform>(enttTriangle,
		XMFLOAT3(0.0f, 0.0f, 5.0f),			// Position
		XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f),	// Rotation (Quaternion)
		XMFLOAT3(1.0f, 1.0f, 1.0f),			// Scale
		IDENTITY_MATRIX);					// World

	registry.assign<Renderer3D>(enttTriangle,
		Resources::GetMeshID("Triangle"),	// Mesh
		Resources::GetShaderID("Debug"));	// Shader
}

void SAMPLE01_DrawingTriangleScene::Update()
{

}

void SAMPLE01_DrawingTriangleScene::FixedUpdate()
{

}
void SAMPLE01_DrawingTriangleScene::SetTitle(const LPCSTR text)
{
	SetWindowTextA(AppHandle::GetWindowHandle(), text);
}