#include "stdafx.h"
#include "GameObject.h"
#include "Component.h"
#include "Camera.h"
#include "Object.h"

Camera::Camera()
{
}

Camera::Camera(const Camera& other)
{
}

Camera::~Camera()
{
}


// TODO: Refactoring
void Camera::Render(int screenWidth, int screenHeight, float screenDepth, float screenNear)
{
	XMFLOAT3 up, lookAt, position;
	XMFLOAT4 rotation;
	XMVECTOR upVector, positionVector, lookAtVector;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;


	// get transform component to get position and rotation
	auto transform = owner->GetComponent<Transform>();

	// set up XMFLOAT3 value
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// load up to upVector
	upVector = XMLoadFloat3(&up);

	// set camera position
	position = transform.GetPosition();

	// load position to positionVector
	positionVector = XMLoadFloat3(&position);

	// set lookAt XMFLOAT3 value
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// load lookAt to lookAtVector
	lookAtVector = XMLoadFloat3(&lookAt);

	rotation = transform.GetRotation();

	// yaw (Y axis), pitch (X axis), roll (Z axis)
	// convert degree to radian
	pitch = rotation.x * 0.0174532925f;
	yaw = rotation.y * 0.0174532925f;
	roll = rotation.z * 0.0174532925f;

	// make rotation matrix with yaw, pitch, roll
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// transform coordinate to lookAtVector, upVector
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	// convert position to viewer(lookAtVector)
	lookAtVector = XMVectorAdd(positionVector, lookAtVector);

	XMMATRIX viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
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
