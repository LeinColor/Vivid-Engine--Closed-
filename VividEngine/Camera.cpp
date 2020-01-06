#include "stdafx.h"
#include "Object.h"
#include "Component.h"
#include "Transform.h"
#include "Camera.h"
#include "Object.h"

// TODO: Refactoring
void Camera::Render(int screenWidth, int screenHeight, float screenDepth, float screenNear)
{
	// get transform component to get position and rotation
	auto transform = owner->GetComponent<Transform>();

	// make rotation matrix with yaw, pitch, roll
	XMMATRIX mat = XMMatrixRotationQuaternion(XMLoadFloat4(&transform.GetRotation()));
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR focusPos = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	// transform coordinate to lookAtVector, upVector
	up = XMVector3TransformCoord(up, mat);
	focusPos = XMVector3TransformCoord(focusPos, mat);

	// convert position to viewer(lookAtVector)
	XMVECTOR eyePos = XMLoadFloat3(&transform.GetPosition());
	focusPos = XMVectorAdd(eyePos, focusPos);

	XMMATRIX viewMatrix = XMMatrixLookAtLH(eyePos, focusPos, up);
	XMStoreFloat4x4(&view, viewMatrix);

	fieldOfView = FIELD_OF_VIEW;
	float screenAspect = screenWidth / screenHeight;

	XMMATRIX projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);
	XMStoreFloat4x4(&projection, projectionMatrix);

	XMMATRIX orthoMatrix = XMMatrixOrthographicLH(screenWidth, screenHeight, screenNear, screenDepth);
	XMStoreFloat4x4(&ortho, orthoMatrix);
	
}

XMMATRIX Camera::GetViewMatrix() const
{
	return XMLoadFloat4x4(&view);
}

XMMATRIX Camera::GetProjectionMatrix() const
{
	return XMLoadFloat4x4(&projection);
}

XMMATRIX Camera::GetOrthoMatrix() const
{
	return XMLoadFloat4x4(&ortho);
}
