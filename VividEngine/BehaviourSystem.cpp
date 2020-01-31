#include "BehaviourSystem.h"

//TransformComponent::TransformComponent()
//{
//	position = XMFLOAT3(0, 0, 0);
//	rotation = XMFLOAT4(0, 0, 0, 1);
//	scale = XMFLOAT3(1, 1, 1);
//	world = IDENTITYMATRIX;
//}
//
//void TransformComponent::SetPosition(const float x, const float y, const float z)
//{
//	position.x = x;
//	position.y = y;
//	position.z = z;
//	UpdateWorldMatrix();
//}
//
//void TransformComponent::SetRotation(const float x, const float y, const float z, const float w)
//{
//	rotation.x = x;
//	rotation.y = y;
//	rotation.z = z;
//	rotation.w = w;
//	UpdateWorldMatrix();
//}
//
//void TransformComponent::SetScale(const float x, const float y, const float z)
//{
//	scale.x = x;
//	scale.y = y;
//	scale.z = z;
//	UpdateWorldMatrix();
//}
//
//void TransformComponent::Translate(const float x, const float y, const float z)
//{
//	position.x += x;
//	position.y += y;
//	position.z += z;
//	UpdateWorldMatrix();
//}
//
//void TransformComponent::Rotate(const float x, const float y, const float z)
//{
//	XMVECTOR quat = XMLoadFloat4(&rotation);
//	XMVECTOR xx = XMQuaternionRotationRollPitchYaw(XMConvertToRadians(x), 0, 0);
//	XMVECTOR yy = XMQuaternionRotationRollPitchYaw(0, XMConvertToRadians(y), 0);
//	XMVECTOR zz = XMQuaternionRotationRollPitchYaw(0, 0, XMConvertToRadians(z));
//
//	quat = XMQuaternionMultiply(xx, quat);
//	quat = XMQuaternionMultiply(quat, yy);
//	quat = XMQuaternionMultiply(zz, quat);
//	quat = XMQuaternionNormalize(quat);
//
//	XMStoreFloat4(&rotation, quat);
//	UpdateWorldMatrix();
//}
//
//void TransformComponent::RotateQuaternion(const XMFLOAT4& quaternion)
//{
//	XMVECTOR result = XMQuaternionMultiply(XMLoadFloat4(&rotation), XMLoadFloat4(&quaternion));
//	result = XMQuaternionNormalize(result);
//	XMStoreFloat4(&rotation, result);
//	UpdateWorldMatrix();
//}
//
//void TransformComponent::UpdateWorldMatrix()
//{
//	XMVECTOR S_local = XMLoadFloat3(&scale);
//	XMVECTOR R_local = XMLoadFloat4(&rotation);
//	XMVECTOR T_local = XMLoadFloat3(&position);
//	XMMATRIX W =
//		XMMatrixScalingFromVector(S_local) *
//		XMMatrixRotationQuaternion(R_local) *
//		XMMatrixTranslationFromVector(T_local);
//
//	XMStoreFloat4x4(&world, W);
//}
//
//XMMATRIX TransformComponent::GetWorldMatrix() const
//{
//	return XMLoadFloat4x4(&world);
//}
//
//XMFLOAT3 TransformComponent::GetPosition() const
//{
//	return *((XMFLOAT3*)&world._41);
//}
//
//XMFLOAT4 TransformComponent::GetRotation() const
//{
//	XMFLOAT4 rotation;
//	XMStoreFloat4(&rotation, GetRotationVector());
//	return rotation;
//}
//
//XMFLOAT3 TransformComponent::GetScale() const
//{
//	XMFLOAT3 scale;
//	XMStoreFloat3(&scale, GetScaleVector());
//	return scale;
//}
//
//XMVECTOR TransformComponent::GetPositionVector() const
//{
//	return XMLoadFloat3((XMFLOAT3*)&world._41);
//}
//XMVECTOR TransformComponent::GetRotationVector() const
//{
//	XMVECTOR S, R, T;
//	XMMatrixDecompose(&S, &R, &T, XMLoadFloat4x4(&world));
//	return R;
//}
//XMVECTOR TransformComponent::GetScaleVector() const
//{
//	XMVECTOR S, R, T;
//	XMMatrixDecompose(&S, &R, &T, XMLoadFloat4x4(&world));
//	return S;
//}
















//void CameraComponent::Update(int screenWidth, int screenHeight)
//{
//	XMMATRIX rot = XMMatrixRotationQuaternion(XMLoadFloat4(&transform.GetRotation()));
//	XMVECTOR vEye = XMLoadFloat3(&transform.GetPosition());
//	XMVECTOR vFocus = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
//	XMVECTOR vUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
//
//	vUp = XMVector3TransformNormal(vUp, rot);
//	vFocus = XMVector3TransformNormal(vFocus, rot);
//	vFocus = XMVectorAdd(vEye, vFocus);
//	XMStoreFloat3(&eye, vEye);
//	XMStoreFloat3(&focus, vFocus);
//	XMStoreFloat3(&up, vUp);
//
//	XMMATRIX viewMatrix = XMMatrixLookAtLH(vEye, vFocus, vUp);
//	XMMATRIX projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenWidth / screenHeight, nearZ, farZ);
//	XMMATRIX orthoMatrix = XMMatrixOrthographicLH(screenWidth, screenHeight, nearZ, farZ);
//	XMStoreFloat4x4(&view, viewMatrix);
//	XMStoreFloat4x4(&projection, projectionMatrix);
//	XMStoreFloat4x4(&ortho, orthoMatrix);
//}
//
//XMVECTOR CameraComponent::GetEye() const
//{
//	return XMLoadFloat3(&eye);
//}
//
//XMVECTOR CameraComponent::GetFocus() const
//{
//	return XMLoadFloat3(&focus);
//}
//
//XMVECTOR CameraComponent::GetUp() const
//{
//	return XMLoadFloat3(&up);
//}
//
//XMMATRIX CameraComponent::GetViewMatrix() const
//{
//	return XMLoadFloat4x4(&view);
//}
//
//XMMATRIX CameraComponent::GetProjectionMatrix() const
//{
//	return XMLoadFloat4x4(&projection);
//}
//
//XMMATRIX CameraComponent::GetViewProjectionMatrix() const
//{
//	return XMMatrixMultiply(XMLoadFloat4x4(&view), XMLoadFloat4x4(&projection));
//}
//
//XMMATRIX CameraComponent::GetOrthoMatrix() const
//{
//	return XMLoadFloat4x4(&ortho);
//}
