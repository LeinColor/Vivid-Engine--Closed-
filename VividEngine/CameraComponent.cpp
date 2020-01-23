#include "stdafx.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "AppHandle.h"

// TODO: Refactoring
void CameraComponent::Update(int screenWidth, int screenHeight)
{
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
}

XMVECTOR CameraComponent::GetEye() const
{
	return XMLoadFloat3(&eye);
}

XMVECTOR CameraComponent::GetFocus() const
{
	return XMLoadFloat3(&focus);
}

XMVECTOR CameraComponent::GetUp() const
{
	return XMLoadFloat3(&up);
}

XMMATRIX CameraComponent::GetViewMatrix() const
{
	return XMLoadFloat4x4(&view);
}

XMMATRIX CameraComponent::GetProjectionMatrix() const
{
	return XMLoadFloat4x4(&projection);
}

XMMATRIX CameraComponent::GetViewProjectionMatrix() const
{
	return XMMatrixMultiply(XMLoadFloat4x4(&view), XMLoadFloat4x4(&projection));
}

XMMATRIX CameraComponent::GetOrthoMatrix() const
{
	return XMLoadFloat4x4(&ortho);
}
