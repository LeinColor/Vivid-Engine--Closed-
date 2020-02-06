#include "../VividEngine/Importer.h"
#include "../VividEngine/AppHandle.h"
#include "../VividEngine/Resources.h"
#include "../VividEngine/Components.h"

#include "../ThirdParty/entt/entt.hpp"

#include "SAMPLE02_DrawingCube.h"

entt::entity enttCube;
void SAMPLE02_DrawingCubeScene::Start()
{
	// Set Window Title
	SetTitle("SAMPLE02: Drawing a Cube");

	// Camera
	enttMainCamera = registry.create();
	registry.assign<Transform>(enttMainCamera,
		XMFLOAT3(0.0f, 0.0f, 0.0f),			// Position
		XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f),	// Rotation (Quaternion)
		XMFLOAT3(1.0f, 1.0f, 1.0),			// Scale
		IDENTITY_MATRIX);					// World

	registry.assign<Camera>(enttMainCamera);

	// Cube
	enttCube = registry.create();
	registry.assign<Transform>(enttCube,
		XMFLOAT3(0.0f, 0.0f, 5.0f),			// Position
		XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f),	// Rotation (Quaternion)
		XMFLOAT3(1.0f, 1.0f, 1.0f),			// Scale
		IDENTITY_MATRIX);					// World

	registry.assign<Renderer3D>(enttCube,
		Resources::GetMeshID("Cube"),	// Mesh
		Resources::GetShaderID("Debug"));	// Shader
}

void SAMPLE02_DrawingCubeScene::Update()
{
}

void SAMPLE02_DrawingCubeScene::FixedUpdate()
{

}
void SAMPLE02_DrawingCubeScene::SetTitle(const LPCSTR text)
{
	SetWindowTextA(AppHandle::GetWindowHandle(), text);
}