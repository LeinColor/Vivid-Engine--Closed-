#include "stdafx.h"
#include "VividEngine.h"
#include "Loader.h"
#include "Time.h"
#include "Physics.h"
#include "Ray.h"
#include "Manager.h"

#include <unordered_map>
using namespace vivid;

using GameObject = Object;
using EditorObject = Object;


void VividEngine::Start()
{
	// 3D
	GameObject* camera = new GameObject();
	camera->AddComponent<Camera>();
	camera->GetComponent<Transform>().SetPosition(0, 0, -3);

	// Editor Object
	EditorObject* coneX = new EditorObject();
	EditorObject* coneY = new EditorObject();
	EditorObject* coneZ = new EditorObject();
	editorObjects[EDITOR_CONE_X] = coneX;
	editorObjects[EDITOR_CONE_Y] = coneY;
	editorObjects[EDITOR_CONE_Z] = coneZ;
	coneX->AddComponent<Renderer3D>();
	coneY->AddComponent<Renderer3D>();
	coneZ->AddComponent<Renderer3D>();
	coneX->GetComponent<Renderer3D>().mesh = Manager::meshes[MESH_CONE];
	coneY->GetComponent<Renderer3D>().mesh = Manager::meshes[MESH_CONE];
	coneZ->GetComponent<Renderer3D>().mesh = Manager::meshes[MESH_CONE];
	coneX->GetComponent<Renderer3D>().material = Manager::materials[MATERIAL_DEBUG];
	coneY->GetComponent<Renderer3D>().material = Manager::materials[MATERIAL_DEBUG];
	coneZ->GetComponent<Renderer3D>().material = Manager::materials[MATERIAL_DEBUG];
	coneX->GetComponent<Renderer3D>().color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	coneY->GetComponent<Renderer3D>().color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	coneZ->GetComponent<Renderer3D>().color = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	coneX->GetComponent<Transform>().SetScale(0.015f, 0.033f, 0.015f);
	coneY->GetComponent<Transform>().SetScale(0.015f, 0.033f, 0.015f);
	coneZ->GetComponent<Transform>().SetScale(0.015f, 0.033f, 0.015f);
	coneX->SetActive(false);
	coneY->SetActive(false);
	coneZ->SetActive(false);

	GameObject* cube = new GameObject();
	cube->AddComponent<Renderer3D>();
	cube->GetComponent<Renderer3D>().mesh = Manager::meshes[MESH_CUBE];
	cube->GetComponent<Renderer3D>().material = Manager::materials[MATERIAL_BLINN_PHONG];
	cube->GetComponent<Renderer3D>().color = XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f);
	cube->GetComponent<Transform>().SetPosition(0.11f, 0.22f, -0.21f);
	cube->GetComponent<Transform>().SetScale(0.1f, 0.1f, 0.1f);

	GameObject* sphere = new GameObject();
	sphere->AddComponent<Renderer3D>();
	sphere->GetComponent<Renderer3D>().mesh = Manager::meshes[MESH_SPHERE];
	sphere->GetComponent<Renderer3D>().material = Manager::materials[MATERIAL_BLINN_PHONG];
	sphere->GetComponent<Renderer3D>().color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	sphere->GetComponent<Transform>().SetPosition(-0.5f, 0.0f, -0.2f);
	sphere->GetComponent<Transform>().SetScale(0.16f, 0.16f, 0.16f);

	GameObject* cone = new GameObject();
	cone->AddComponent<Renderer3D>();
	cone->GetComponent<Transform>().SetPosition(-0.9f, 0.0f, -2.0f);

	for (float f = -1.0; f <= 1.0; f += 0.1) {
		GameObject* t = new GameObject();
		t->AddComponent<Renderer3D>();
		t->GetComponent<Renderer3D>().mesh = Manager::meshes[MESH_SPHERE];
		t->GetComponent<Renderer3D>().material = Manager::materials[MATERIAL_BLINN_PHONG];
		t->GetComponent<Renderer3D>().color = XMFLOAT4((f+1)*0.5, 1-(f+1)*0.5, (f+0.25)*0.5, 1.0f);
		t->GetComponent<Transform>().SetPosition(f, 0.5-f, cos(f));
		t->GetComponent<Transform>().SetScale(0.11f, 0.11f, 0.11f);
	}

	GameObject* light = new GameObject();
	light->AddComponent<Light>();
	light->GetComponent<Light>().attrib.ambientColor = XMFLOAT4(0.15f, 0.15f, 0.15f, 1.0f);
	light->GetComponent<Light>().attrib.diffuseColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	light->GetComponent<Light>().attrib.lightDirection = XMFLOAT3(1.0f, 1.0f, 0.5f);
	light->GetComponent<Light>().attrib.specularColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	light->GetComponent<Light>().attrib.specularPower = 32.0f;
}

void VividEngine::Initialize()
{
	if (Renderer::GetGraphicsInterface() == nullptr)
		Renderer::SetGraphicsInterface(new DirectX11Wrapper(AppHandle::GetWindowHandle(), false));
	
	// initialize renderer to load mesh, shader
	Time::Initialize();
	input.Initialize();
	Loader::Initialize();
	initialized = true;
}

void VividEngine::Run()
{
	if (!initialized) {
		Initialize();
		Start();
	}

	Time::deltaTime = float(std::max(0.0, Time::Elapsed() / 1000.0));
	Time::Record();

	deltaTimeAccumulator += Time::deltaTime;
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
XMVECTOR test;
RaycastHit hit;
int prevIndex = -1;
XMFLOAT4 prevColor;
bool isHit;
void VividEngine::FixedUpdate()
{
	char buffer[128];
	if (isHit)
		sprintf_s(buffer, "%d", prevIndex);
	else
		sprintf_s(buffer, "%d", prevIndex);
	SetWindowTextA(AppHandle::GetWindowHandle(), buffer);
}

void VividEngine::Update()
{
	// Read Input
	input.ReadInput();

	auto mainCamera = Scene::GetMainCamera();

	if (input.GetKey(DIK_LEFTARROW)) {
		Scene::objects[4]->GetComponent<Transform>().Translate(-0.5f * Time::deltaTime, 0, 0);
	}
	if (input.GetKey(DIK_RIGHTARROW)) {
		Scene::objects[4]->GetComponent<Transform>().Translate(0.5f * Time::deltaTime, 0, 0);
	}
	if (input.GetKey(DIK_UPARROW)) {
		Scene::objects[4]->GetComponent<Transform>().Translate(0, 0, -0.5f * Time::deltaTime);
	}
	if (input.GetKey(DIK_DOWNARROW)) {
		Scene::objects[4]->GetComponent<Transform>().Translate(0, 0, +0.5f * Time::deltaTime);
	}

	// Input animate example
	if (input.GetMouseButtonDown(0)) {
		mainCamera->GetComponent<Transform>().Translate(-input.GetMouseDx() * 0.003f, input.GetMouseDy() * 0.003f, 0);
		test = Scene::ScreenToWorldPoint(XMFLOAT3(input.GetMouseLocation().x, input.GetMouseLocation().y, 0));
		isHit = Physics::Raycast(XMLoadFloat3(&mainCamera->GetComponent<Transform>().GetPosition()), test, hit, 11, 1);
		if (isHit) {
			if (prevIndex != -1) {
				Scene::objects[prevIndex]->GetComponent<Renderer3D>().color = prevColor;
			}
			prevColor = Scene::objects[hit.index]->GetComponent<Renderer3D>().color;
			Scene::objects[hit.index]->GetComponent<Renderer3D>().color = XMFLOAT4(1,1,1,1);
			prevIndex = hit.index;
			editorObjects[EDITOR_CONE_X]->SetActive(true);
			editorObjects[EDITOR_CONE_Y]->SetActive(true);
			editorObjects[EDITOR_CONE_Z]->SetActive(true);
		} else {
			if (prevIndex != -1)
				Scene::objects[prevIndex]->GetComponent<Renderer3D>().color = prevColor;
			prevIndex = -1;
			editorObjects[EDITOR_CONE_X]->SetActive(false);
			editorObjects[EDITOR_CONE_Y]->SetActive(false);
			editorObjects[EDITOR_CONE_Z]->SetActive(false);
		}
	}

	if (input.GetMouseButton(1)) {
		mainCamera->GetComponent<Transform>().Rotate(input.GetMouseDy() * 0.2f, input.GetMouseDx() * 0.2f, 0);
	}


	XMFLOAT3 tp; XMStoreFloat3(&tp, test);
	
	// Zoom in out
	XMVECTOR vDir = XMVectorSubtract(mainCamera->GetComponent<Camera>().GetFocus(), mainCamera->GetComponent<Camera>().GetEye());
	vDir = XMVector3Normalize(vDir);
	XMFLOAT3 dir;
	XMStoreFloat3(&dir, vDir);
	mainCamera->GetComponent<Transform>().Translate(Input::lZ * dir.x * 0.3f, Input::lZ * dir.y * 0.3f, Input::lZ * dir.z * 0.3f);
	Input::lZ = 0;

	if (input.GetKey(DIK_D) && prevIndex != -1) {
		Scene::objects[prevIndex]->GetComponent<Transform>().Rotate(0, -50 * Time::deltaTime, -50 * Time::deltaTime);
	}
	if (input.GetKey(DIK_ESCAPE)) {
		exit(0);
	}

	// Get object's position and rotation
	if (prevIndex != -1) {
		auto origin = Scene::objects[prevIndex]->GetComponent<Transform>().GetPosition();
		auto rotation = Scene::objects[prevIndex]->GetComponent<Transform>().GetRotation();
		auto camPos = mainCamera->GetComponent<Transform>().GetPosition();
		// Calculate axis matrix
		XMFLOAT3 scale = XMFLOAT3(0.5f, 0.5f, 0.5f);
		XMMATRIX W =
			XMMatrixScalingFromVector(XMLoadFloat3(&scale)) *
			XMMatrixRotationQuaternion(XMLoadFloat4(&rotation)) *
			XMMatrixTranslationFromVector(XMLoadFloat3(&origin));

		// Transform each axis point by matrix
		XMVECTOR right = XMVector3Transform(axisX, W);
		XMVECTOR up = XMVector3Transform(axisY, W);
		XMVECTOR forward = XMVector3Transform(axisZ, W);
		XMFLOAT3 endPointX, endPointY, endPointZ;
		XMStoreFloat3(&endPointX, right);
		XMStoreFloat3(&endPointY, up);
		XMStoreFloat3(&endPointZ, forward);

		// Set position of axis X, Y, Z
		editorObjects[EDITOR_CONE_X]->GetComponent<Transform>().SetPosition(endPointX.x, endPointX.y, endPointX.z);
		editorObjects[EDITOR_CONE_Y]->GetComponent<Transform>().SetPosition(endPointY.x, endPointY.y, endPointY.z);
		editorObjects[EDITOR_CONE_Z]->GetComponent<Transform>().SetPosition(endPointZ.x, endPointZ.y, endPointZ.z);

		// Axis X cone rotation (rotate Z for 270 degress)
		{
			XMFLOAT4 axisRot;
			XMVECTOR eulorToQuat = XMQuaternionRotationRollPitchYaw(0, 0, XMConvertToRadians(270.0f));
			XMVECTOR result = XMQuaternionNormalize(XMQuaternionMultiply(eulorToQuat, XMLoadFloat4(&rotation)));
			XMStoreFloat4(&axisRot, result);
			editorObjects[EDITOR_CONE_X]->GetComponent<Transform>().SetRotation(axisRot.x, axisRot.y, axisRot.z, axisRot.w);
		}

		// Axis Y cone rotation (rotation is same with object's)
		{
			XMFLOAT4 axisRot = Scene::objects[prevIndex]->GetComponent<Transform>().GetRotation();
			editorObjects[EDITOR_CONE_Y]->GetComponent<Transform>().SetRotation(axisRot.x, axisRot.y, axisRot.z, axisRot.w);
		}

		// Axis Z cone rotation (rotate X for 90 degrees)
		{
			XMFLOAT4 axisRot;
			XMVECTOR eulorToQuat = XMQuaternionRotationRollPitchYaw(XMConvertToRadians(90.0f), 0, 0);
			XMVECTOR result = XMQuaternionNormalize(XMQuaternionMultiply(eulorToQuat, XMLoadFloat4(&rotation)));
			XMStoreFloat4(&axisRot, result);
			editorObjects[EDITOR_CONE_Z]->GetComponent<Transform>().SetRotation(axisRot.x, axisRot.y, axisRot.z, axisRot.w);
		}

		GizmoLine gizmoAxisX, gizmoAxisY, gizmoAxisZ;
		gizmoAxisX.startPoint = gizmoAxisY.startPoint = gizmoAxisZ.startPoint = Scene::objects[prevIndex]->GetComponent<Transform>().GetPosition();
		gizmoAxisX.endPoint = endPointX;
		gizmoAxisY.endPoint = endPointY;
		gizmoAxisZ.endPoint = endPointZ;
		gizmoAxisX.color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		gizmoAxisY.color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
		gizmoAxisZ.color = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

		renderer.AddGizmoLine(gizmoAxisX);
		renderer.AddGizmoLine(gizmoAxisY);
		renderer.AddGizmoLine(gizmoAxisZ);
	}
}

void VividEngine::SetWindow(HWND hWnd, HINSTANCE hInstance)
{
	AppHandle::SetWindowHandle(hWnd);
	AppHandle::SetInstanceHandle(hInstance);
}