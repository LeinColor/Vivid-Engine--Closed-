#include "../VividEngine/Importer.h"
#include "../VividEngine/AppHandle.h"
#include "../VividEngine/Resources.h"
#include "../VividEngine/Components.h"

#include "../ThirdParty/entt/entt.hpp"

#include "SAMPLE01_DrawingTriangle.h"

struct position {
	float x;
	float y;
};
void SAMPLE01_DrawingTriangleScene::Start()
{
	// Set Window Title
	SetTitle("SAMPLE01: Drawing a Triangle");

	// Camera
	enttMainCamera = registry.create();
	registry.assign<Transform>(enttMainCamera,
		XMFLOAT3(0.0f, 0.0f, 0.0f),			// Position
		XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f),	// Rotation
		XMFLOAT3(1.0f, 1.0f, 1.0),			// Scale
		IDENTITY_MATRIX);					// World

	registry.assign<Camera>(enttMainCamera,
		XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT3(0.0f, 0.0f, 0.0f),
		IDENTITY_MATRIX,
		IDENTITY_MATRIX,
		IDENTITY_MATRIX,
		800.0f,
		0.001f,
		3.141592654f / 4.0f
		);

	// Cube
	auto enttCube = registry.create();
	registry.assign<Transform>(enttCube,
		XMFLOAT3(0.0f, 0.0f, -5.0f),		// Position
		XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f),	// Rotation
		XMFLOAT3(1.0f, 1.0f, 1.0),			// Scale
		IDENTITY_MATRIX);					// World

	registry.assign<Renderer3D>(enttCube,
		Resources::GetMeshID("Cube"),		// Mesh
		Resources::GetShaderID("Debug"));	// Shader
}

void SAMPLE01_DrawingTriangleScene::Update()
{

}

void SAMPLE01_DrawingTriangleScene::FixedUpdate()
{

}
void SAMPLE01_DrawingTriangleScene::SetTitle(LPCSTR text)
{
	SetWindowTextA(AppHandle::GetWindowHandle(), text);
}