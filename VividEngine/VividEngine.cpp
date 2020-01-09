#include "stdafx.h"
#include "VividEngine.h"
#include "Time.h"

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
	EditorObject* axisX = new EditorObject();
	EditorObject* axisY = new EditorObject();
	EditorObject* axisZ = new EditorObject();
	axisX->state = DEBUG;
	axisY->state = DEBUG;
	axisZ->state = DEBUG;
	axisX->AddComponent<Renderer3D>();
	axisY->AddComponent<Renderer3D>();
	axisZ->AddComponent<Renderer3D>();
	axisX->GetComponent<Renderer3D>().mesh = Scene::meshes[MESH_CONE];
	axisY->GetComponent<Renderer3D>().mesh = Scene::meshes[MESH_CONE];
	axisZ->GetComponent<Renderer3D>().mesh = Scene::meshes[MESH_CONE];
	axisX->GetComponent<Renderer3D>().color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	axisY->GetComponent<Renderer3D>().color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	axisZ->GetComponent<Renderer3D>().color = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	axisX->GetComponent<Transform>().SetScale(0.015f, 0.033f, 0.015f);
	axisY->GetComponent<Transform>().SetScale(0.015f, 0.033f, 0.015f);
	axisZ->GetComponent<Transform>().SetScale(0.015f, 0.033f, 0.015f);

	GameObject* cube = new GameObject();
	cube->AddComponent<Renderer3D>();
	cube->GetComponent<Transform>().SetPosition(0.11f, 0.22f, -0.21f);
	cube->GetComponent<Transform>().SetScale(0.1f, 0.1f, 0.1f);

	GameObject* sphere = new GameObject();
	sphere->AddComponent<Renderer3D>();
	sphere->GetComponent<Transform>().SetPosition(3.0f, 0.0f, 0.0f);

	GameObject* cone = new GameObject();
	cone->AddComponent<Renderer3D>();
	cone->GetComponent<Transform>().SetPosition(-0.9f, 0.0f, -2.0f);

	GameObject* light = new GameObject();
	light->AddComponent<Light>();
	light->GetComponent<Light>().attrib.ambientColor = XMFLOAT4(0.15f, 0.15f, 0.15f, 1.0f);
	light->GetComponent<Light>().attrib.diffuseColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	light->GetComponent<Light>().attrib.lightDirection = XMFLOAT3(1.0f, 1.0f, 0.5f);
	light->GetComponent<Light>().attrib.specularColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	light->GetComponent<Light>().attrib.specularPower = 32.0f;

	cube->GetComponent<Renderer3D>().mesh = Scene::meshes[MESH_CUBE];
	//sphere->GetComponent<Renderer3D>().mesh = Scene::meshes[MESH_SPHERE];
}

void VividEngine::Initialize()
{
	if (renderer.GetDevice() == nullptr)
		renderer.SetDevice(new DirectX11Wrapper(AppHandle::GetWindowHandle(), false));
	
	// initialize renderer to load mesh, shader
	Time::Initialize();
	input.Initialize();
	renderer.Initialize();
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
		deltaTimeAccumulator -= targetFrameRateInv;
	}
	Update();
}

void VividEngine::FixedUpdate()
{
	MousePos pos;
	pos.x = input.GetMouseDx();
	pos.y = input.GetMouseDy();
	float z = input.lZ;
	//char buff[64];
	//sprintf_s(buff, "x:%ld y:%ld z:%f", pos.x, pos.y, z);
	//SetWindowTextA(AppHandle::GetWindowHandle(), buff);
}

void VividEngine::Update()
{
	// Read Input
	input.ReadInput();

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
	if (input.GetMouseButton(0)) {
		Scene::objects[0]->GetComponent<Transform>().Translate(-input.GetMouseDx() * 0.003f, input.GetMouseDy() * 0.003f, 0);
	}

	if (input.GetMouseButton(1)) {
		Scene::objects[0]->GetComponent<Transform>().Rotate(input.GetMouseDy() * 0.2f, input.GetMouseDx() * 0.2f, 0);
	}
	
	XMVECTOR vDir = XMVectorSubtract(Scene::objects[0]->GetComponent<Camera>().GetFocus(), Scene::objects[0]->GetComponent<Camera>().GetEye());
	vDir = XMVector3Normalize(vDir);
	XMFLOAT3 dir;
	XMStoreFloat3(&dir, vDir);
	Scene::objects[0]->GetComponent<Transform>().Translate(Input::lZ * dir.x * 0.3f, Input::lZ * dir.y * 0.3f, Input::lZ * dir.z * 0.3f);
	Input::lZ = 0;
	
	//if (abs(input.lZ) < 1e-6)
	//	input.lZ = 0;

	//if (input.lZ > 0)
	//	input.lZ -= Time::deltaTime;
	//else if (input.lZ < 0)
	//	input.lZ += Time::deltaTime;

	if (input.GetKey(DIK_D)) {
		Scene::objects[4]->GetComponent<Transform>().Rotate(0, -50 * Time::deltaTime, -50 * Time::deltaTime);
	}
	if (input.GetKey(DIK_ESCAPE)) {
		exit(0);
	}


	// Get object's position and rotation
	auto origin = Scene::objects[4]->GetComponent<Transform>().GetPosition();
	auto rotation = Scene::objects[4]->GetComponent<Transform>().GetRotation();
	auto camPos = Scene::objects[0]->GetComponent<Transform>().GetPosition();
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
	Scene::objects[1]->GetComponent<Transform>().SetPosition(endPointX.x, endPointX.y, endPointX.z);
	Scene::objects[2]->GetComponent<Transform>().SetPosition(endPointY.x, endPointY.y, endPointY.z);
	Scene::objects[3]->GetComponent<Transform>().SetPosition(endPointZ.x, endPointZ.y, endPointZ.z);

	// Axis X cone rotation (rotate Z for 270 degress)
	{
		XMFLOAT4 axisRot;
		XMVECTOR eulorToQuat = XMQuaternionRotationRollPitchYaw(0, 0, XMConvertToRadians(270.0f));
		XMVECTOR result = XMQuaternionNormalize(XMQuaternionMultiply(eulorToQuat, XMLoadFloat4(&rotation)));
		XMStoreFloat4(&axisRot, result);
		Scene::objects[1]->GetComponent<Transform>().SetRotation(axisRot.x, axisRot.y, axisRot.z, axisRot.w);
	}

	// Axis Y cone rotation (rotation is same with object's)
	{
		XMFLOAT4 axisRot = Scene::objects[4]->GetComponent<Transform>().GetRotation();
		Scene::objects[2]->GetComponent<Transform>().SetRotation(axisRot.x, axisRot.y, axisRot.z, axisRot.w);
	}

	// Axis Z cone rotation (rotate X for 90 degrees)
	{
		XMFLOAT4 axisRot;
		XMVECTOR eulorToQuat = XMQuaternionRotationRollPitchYaw(XMConvertToRadians(90.0f), 0, 0);
		XMVECTOR result = XMQuaternionNormalize(XMQuaternionMultiply(eulorToQuat, XMLoadFloat4(&rotation)));
		XMStoreFloat4(&axisRot, result);
		Scene::objects[3]->GetComponent<Transform>().SetRotation(axisRot.x, axisRot.y, axisRot.z, axisRot.w);
	}

	GizmoLine gizmoAxisX, gizmoAxisY, gizmoAxisZ;
	gizmoAxisX.startPoint = gizmoAxisY.startPoint = gizmoAxisZ.startPoint = Scene::objects[4]->GetComponent<Transform>().GetPosition();
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

void VividEngine::SetWindow(HWND hWnd, HINSTANCE hInstance)
{
	AppHandle::SetWindowHandle(hWnd);
	AppHandle::SetInstanceHandle(hInstance);
}