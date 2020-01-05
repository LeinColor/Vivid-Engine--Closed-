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
	camera->GetComponent<Transform>().SetPosition(0, 0, -4);

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
	axisX->GetComponent<Transform>().SetScale(0.02f, 0.05f, 0.02f);
	axisY->GetComponent<Transform>().SetScale(0.02f, 0.05f, 0.02f);
	axisZ->GetComponent<Transform>().SetScale(0.02f, 0.05f, 0.02f);

	GameObject* cube = new GameObject();
	cube->AddComponent<Renderer3D>();
	cube->GetComponent<Transform>().SetScale(0.1f, 0.1f, 0.1f);

	GameObject* sphere = new GameObject();
	sphere->AddComponent<Renderer3D>();
	sphere->GetComponent<Transform>().SetPosition(3, 0, 0);

	GameObject* cone = new GameObject();
	cone->AddComponent<Renderer3D>();
	cone->GetComponent<Transform>().SetPosition(-0.9, 0, -2);

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
	while (deltaTimeAccumulator >= targetFrameRateInv)
	{
		FixedUpdate();	// calculate physics here

		renderer.Render();
		deltaTimeAccumulator -= targetFrameRateInv;
	}
	Update();
}

void VividEngine::FixedUpdate()
{

}

void VividEngine::Update()
{
	Scene::objects[4]->GetComponent<Transform>().Rotate(20 * Time::deltaTime, 20 * Time::deltaTime, 20 * Time::deltaTime);

	auto p = Scene::objects[4]->GetComponent<Transform>().GetRotation();
	XMVECTOR quat = XMLoadFloat4(&p);
	XMMATRIX m = XMMatrixRotationQuaternion(quat);
	auto right = XMVector3TransformCoord(axisX, m);
	auto up = XMVector3TransformCoord(axisY, m);
	auto forward = XMVector3TransformCoord(axisZ, m);

	XMFLOAT3 endPointX, endPointY, endPointZ;
	XMStoreFloat3(&endPointX, right);
	XMStoreFloat3(&endPointY, up);
	XMStoreFloat3(&endPointZ, forward);

	float xx, yy, zz, ww;
	auto aa = Scene::objects[4]->GetComponent<Transform>().GetRotation();
	xx = aa.x;
	yy = aa.y;
	zz = aa.z;
	ww = aa.w;

	XMVECTOR vx = XMQuaternionRotationRollPitchYaw(0, 0, XMConvertToRadians(270.0f));
	XMVECTOR vy = XMQuaternionRotationRollPitchYaw(XMConvertToRadians(90.0f), 0, 0);

	// Axis X rotation
	XMFLOAT4 k1;
	float gx = XMVectorGetX(vx);
	float gy = XMVectorGetY(vx);
	float gz = XMVectorGetZ(vx);
	float gw = XMVectorGetW(vx);
	XMStoreFloat4(&k1, vx);
	Scene::objects[1]->GetComponent<Transform>().SetRotation(gx, gy, gz, gw);
	Scene::objects[1]->GetComponent<Transform>().RotateQuaternion(p);

	// Axis Y rotation
	Scene::objects[2]->GetComponent<Transform>().SetRotation(xx, yy, zz, ww);

	// Axis Z rotation
	XMFLOAT4 k2;
	float hx = XMVectorGetX(vy);
	float hy = XMVectorGetY(vy);
	float hz = XMVectorGetZ(vy);
	float hw = XMVectorGetW(vy);
	XMStoreFloat4(&k2, vy);
	Scene::objects[3]->GetComponent<Transform>().SetRotation(hx, hy, hz, hw);
	Scene::objects[3]->GetComponent<Transform>().RotateQuaternion(p);

	// Cone Position
	Scene::objects[1]->GetComponent<Transform>().SetPosition(endPointX.x, endPointX.y, endPointX.z);
	Scene::objects[2]->GetComponent<Transform>().SetPosition(endPointY.x, endPointY.y, endPointY.z);
	Scene::objects[3]->GetComponent<Transform>().SetPosition(endPointZ.x, endPointZ.y, endPointZ.z);

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