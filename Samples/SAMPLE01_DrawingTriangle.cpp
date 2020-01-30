#include "../VividEngine/Importer.h"
#include "../VividEngine/AppHandle.h"

#include "../ThirdParty/entt/entt.hpp"
#include "../VividEngine/MeshComponent.h"

#include "SAMPLE01_DrawingTriangle.h"

struct velocity {
	float dx;
	float dy;
};
void SAMPLE01_DrawingTriangleScene::Start()
{
	// Set Window Title
	SetWindowTextA(AppHandle::GetWindowHandle(), "SAMPLE01: Drawing a Triangle");



	entt::registry registry;
	auto entityMeshCube = registry.create();
	registry.assign<velocity>(entityMeshCube, 1.f, 2.f);
	auto view = registry.view<velocity>();
	for (auto entity : view) {
		auto& mesh = view.get<velocity>(entity);
		char buf[128];
		sprintf_s(buf, "xdxdxd");
		SetWindowTextA(AppHandle::GetWindowHandle(), buf);

	}
	//=======================================
	// Mesh
	MeshComponent& meshCube = Importer::LoadObjFile("../VividEngine/Obj/cube.obj", this);
	//=======================================
	// Shader
	ShaderComponent& shader = Importer::LoadShaderFile("Debug", INPUT_LAYOUT_TYPE::POS, this);
	//=======================================
	// Camera
	Entity cameraEntity = ECS::CreateEntity();
	ObjectComponent& objCamera = objects.Create(cameraEntity);
	TransformComponent& tfCamera = transforms.Create(cameraEntity);
	tfCamera.SetPosition(0.0f, 0.0f, -5.0f);

	CameraComponent& camera = cameras.Create(cameraEntity);
	camera.transform = tfCamera;
	//=======================================
	// Cube
	Entity cubeEntity = ECS::CreateEntity();
	ObjectComponent& objCube = objects.Create(cubeEntity);
	TransformComponent& tfCube = transforms.Create(cubeEntity);
	tfCube.SetPosition(0.0f, 0.0f, 0.0f);

	objCube.meshEntity = meshes.GetEntity(0);
	objCube.shaderEntity = shaders.GetEntity(0);
}

void SAMPLE01_DrawingTriangleScene::Update()
{

}

void SAMPLE01_DrawingTriangleScene::FixedUpdate()
{

}