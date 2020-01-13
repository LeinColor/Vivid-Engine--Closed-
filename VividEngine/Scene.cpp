#include "Scene.h"
#include "Object.h"
#include "Camera.h"
#include "Renderer.h"
#include "Debug.h"
#include <vector>
using namespace vivid;
using namespace std;

vector<Object*> Scene::objects;
vector<AABB*> Scene::aabbs;

XMVECTOR Scene::ScreenToWorldPoint(const XMFLOAT3& pos)
{
	int screenWidth = Renderer::GetScreenWidth();
	int screenHeight = Renderer::GetScreenHeight();

	float pointX = ((2.0f * (float)pos.x) / (float)screenWidth) - 1.0f;
	float pointY = (((2.0f * (float)pos.y) / (float)screenHeight) - 1.0f) * -1.0f;

	auto mainCamera = GetMainCamera();
	auto& cameraComponent = mainCamera->GetComponent<Camera>();

	XMFLOAT3X3 projectionMatrix4;
	XMStoreFloat3x3(&projectionMatrix4, cameraComponent.GetProjectionMatrix());

	pointX = pointX / projectionMatrix4._11;
	pointY = pointY / projectionMatrix4._22;

	XMMATRIX inverseViewMatrix;
	inverseViewMatrix = XMMatrixInverse(nullptr, cameraComponent.GetViewMatrix());

	XMFLOAT3X3 inverseViewMatrix4;
	XMStoreFloat3x3(&inverseViewMatrix4, inverseViewMatrix);

	XMFLOAT3 direction;
	direction.x = (pointX * inverseViewMatrix4._11) + (pointY * inverseViewMatrix4._21) + inverseViewMatrix4._31;
	direction.y = (pointX * inverseViewMatrix4._12) + (pointY * inverseViewMatrix4._22) + inverseViewMatrix4._32;
	direction.z = (pointX * inverseViewMatrix4._13) + (pointY * inverseViewMatrix4._23) + inverseViewMatrix4._33;

	//XMMATRIX inverseWorldMatrix = XMMatrixInverse(nullptr, mainCamera->GetComponent<Transform>().GetWorldMatrix());
	XMStoreFloat3(&direction, XMVector3TransformNormal(XMVectorSet(direction.x, direction.y, direction.z, 0.0f), mainCamera->GetComponent<Transform>().GetWorldMatrix()));


	return XMVector3Normalize(XMVectorSet(direction.x, direction.y, direction.z, 0.0f));
}

Object* Scene::GetMainCamera()
{
	for (auto& object : Scene::objects)
	{
		auto& camera = object->GetComponent<Camera>();
		if (&camera != nullptr)
		{
			return object;
		}
	}
	Debug::Log("Couldn't find a main camera!");
}