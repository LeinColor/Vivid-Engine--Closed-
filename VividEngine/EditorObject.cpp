#include "stdafx.h"
#include "Mesh.h"
#include "EditorObject.h"

EditorObject::EditorObject()
{
	position = XMFLOAT3(0, 0, 0);
	rotation = XMFLOAT4(0, 0, 0, 1);
	scale = XMFLOAT3(1, 1, 1);
	world = IDENTITYMATRIX;
}

void EditorObject::SetPosition(const float x, const float y, const float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
	UpdateWorldMatrix();
}

void EditorObject::SetRotation(float x, float y, float z)
{
	rotation.x = x;
	rotation.y = y;
	rotation.z = z;
	UpdateWorldMatrix();
}

void EditorObject::SetScale(float x, float y, float z)
{
	scale.x = x;
	scale.y = y;
	scale.z = z;
	UpdateWorldMatrix();
}

void EditorObject::Translate(const float x, const float y, const float z)
{
	position.x += x;
	position.y += y;
	position.z += z;
	UpdateWorldMatrix();
}

void EditorObject::Rotate(const XMFLOAT3& value)
{
	XMVECTOR quat = XMLoadFloat4(&rotation);
	XMVECTOR x = XMQuaternionRotationRollPitchYaw(value.x, 0, 0);
	XMVECTOR y = XMQuaternionRotationRollPitchYaw(0, value.y, 0);
	XMVECTOR z = XMQuaternionRotationRollPitchYaw(0, 0, value.z);

	quat = XMQuaternionMultiply(x, quat);
	quat = XMQuaternionMultiply(quat, y);
	quat = XMQuaternionMultiply(z, quat);
	quat = XMQuaternionNormalize(quat);

	XMStoreFloat4(&rotation, quat);
	UpdateWorldMatrix();
}

void EditorObject::UpdateWorldMatrix()
{
	XMVECTOR S_local = XMLoadFloat3(&scale);
	XMVECTOR R_local = XMLoadFloat4(&rotation);
	XMVECTOR T_local = XMLoadFloat3(&position);
	XMMATRIX W =
		XMMatrixScalingFromVector(S_local) *
		XMMatrixRotationQuaternion(R_local) *
		XMMatrixTranslationFromVector(T_local);

	XMStoreFloat4x4(&world, W);

	XMVECTOR S, R, T;
	XMMatrixDecompose(&S, &R, &T, XMLoadFloat4x4(&world));
	XMStoreFloat3(&scale, S);
	XMStoreFloat4(&rotation, R);
	XMStoreFloat3(&position, T);

}

XMMATRIX EditorObject::GetWorldMatrix() const
{
	return XMLoadFloat4x4(&world);
}

XMFLOAT3 EditorObject::GetPosition() const
{
	return *((XMFLOAT3*)&world._41);
}

XMFLOAT4 EditorObject::GetRotation() const
{
	XMFLOAT4 rotation;
	XMStoreFloat4(&rotation, GetRotationVector());
	return rotation;
}

XMFLOAT3 EditorObject::GetScale() const
{
	XMFLOAT3 scale;
	XMStoreFloat3(&scale, GetScaleVector());
	return scale;
}

XMVECTOR EditorObject::GetPositionVector() const
{
	return XMLoadFloat3((XMFLOAT3*)&world._41);
}
XMVECTOR EditorObject::GetRotationVector() const
{
	XMVECTOR S, R, T;
	XMMatrixDecompose(&S, &R, &T, XMLoadFloat4x4(&world));
	return R;
}
XMVECTOR EditorObject::GetScaleVector() const
{
	XMVECTOR S, R, T;
	XMMatrixDecompose(&S, &R, &T, XMLoadFloat4x4(&world));
	return S;
}