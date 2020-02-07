#include "../VividEngine/Importer.h"
#include "../VividEngine/AppHandle.h"
#include "../VividEngine/Resources.h"
#include "../VividEngine/Components.h"

#include "../ThirdParty/entt/entt.hpp"

#include "SAMPLE03_Texture.h"

void SAMPLE03_TextureScene::Start()
{
	// Set Window Title
	SetTitle("SAMPLE03: Texture");

	// Camera
	enttMainCamera = registry.create();
	registry.assign<Transform>(enttMainCamera,
		XMFLOAT3(0.0f, 0.0f, 0.0f),			// Position
		XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f),	// Rotation (Quaternion)
		XMFLOAT3(1.0f, 1.0f, 1.0),			// Scale
		IDENTITY_MATRIX);					// World

	registry.assign<Camera>(enttMainCamera);

	// Cube
	auto enttCube = registry.create();
	registry.assign<Transform>(enttCube,
		XMFLOAT3(0.0f, 0.0f, 5.0f),			// Position
		XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f),	// Rotation (Quaternion)
		XMFLOAT3(1.0f, 1.0f, 1.0f),			// Scale
		IDENTITY_MATRIX);					// World

	registry.assign<Renderer3D>(enttCube,
		Resources::GetMeshID("Cube"),	// Mesh
		Resources::GetShaderID("Texture"),
		Resources::GetTextureID("test"));	// Shader
}

void SAMPLE03_TextureScene::Update()
{
}

void SAMPLE03_TextureScene::FixedUpdate()
{

}
void SAMPLE03_TextureScene::SetTitle(const LPCSTR text)
{
	SetWindowTextA(AppHandle::GetWindowHandle(), text);
}