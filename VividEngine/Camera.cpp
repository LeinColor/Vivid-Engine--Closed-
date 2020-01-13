#include "stdafx.h"
#include "Object.h"
#include "Component.h"
#include "Transform.h"
#include "Camera.h"
#include "Object.h"
#include "AppHandle.h"

// TODO: Refactoring
void Camera::Update(int screenWidth, int screenHeight)
{
	auto transform = owner->GetComponent<Transform>();

	XMMATRIX rot = XMMatrixRotationQuaternion(XMLoadFloat4(&transform.GetRotation()));
	XMVECTOR vEye = XMLoadFloat3(&transform.GetPosition());
	XMVECTOR vFocus = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR vUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	vUp = XMVector3TransformNormal(vUp, rot);
	vFocus = XMVector3TransformNormal(vFocus, rot);
	vFocus = XMVectorAdd(vEye, vFocus);
	XMStoreFloat3(&eye, vEye);
	XMStoreFloat3(&focus, vFocus);
	XMStoreFloat3(&up, vUp);

	XMMATRIX viewMatrix = XMMatrixLookAtLH(vEye, vFocus, vUp);
	XMMATRIX projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenWidth / screenHeight, nearZ, farZ);
	XMMATRIX orthoMatrix = XMMatrixOrthographicLH(screenWidth, screenHeight, nearZ, farZ);
	XMStoreFloat4x4(&view, viewMatrix);
	XMStoreFloat4x4(&projection, projectionMatrix);
	XMStoreFloat4x4(&ortho, orthoMatrix);


	//char buffer[128];
	////sprintf_s(buffer, "%f %f %f", test.x, test.y, test.z);
	//sprintf_s(buffer, "%f %f", projection._11, projection._22);
	//SetWindowTextA(vivid::AppHandle::GetWindowHandle(), buffer);
}

XMVECTOR Camera::GetEye() const
{
	return XMLoadFloat3(&eye);
}

XMVECTOR Camera::GetFocus() const
{
	return XMLoadFloat3(&focus);
}

XMVECTOR Camera::GetUp() const
{
	return XMLoadFloat3(&up);
}

XMMATRIX Camera::GetViewMatrix() const
{
	return XMLoadFloat4x4(&view);
}

XMMATRIX Camera::GetProjectionMatrix() const
{
	return XMLoadFloat4x4(&projection);
}

XMMATRIX Camera::GetViewProjectionMatrix() const
{
	return XMMatrixMultiply(XMLoadFloat4x4(&view), XMLoadFloat4x4(&projection));
}

XMMATRIX Camera::GetOrthoMatrix() const
{
	return XMLoadFloat4x4(&ortho);
}
