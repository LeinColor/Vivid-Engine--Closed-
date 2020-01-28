#pragma once
#include "TransformComponent.h"

class CameraComponent
{
	XMFLOAT3 eye;
	XMFLOAT3 focus;
	XMFLOAT3 up;
	float farZ = 800.0f;
	float nearZ = 0.001f;
	float fieldOfView = 3.141592654f / 4.0f;

	XMFLOAT4X4	view;
	XMFLOAT4X4	projection;
	XMFLOAT4X4	ortho;


public:
	ALIGNED_ALLOC_16;

	TransformComponent transform;

	CameraComponent() {}
	CameraComponent(const CameraComponent&) {}
	~CameraComponent() {}

	void Update(int screenWidth, int screenHeight);

	XMVECTOR GetEye() const;
	XMVECTOR GetFocus() const;
	XMVECTOR GetUp() const;
	XMMATRIX GetViewMatrix() const;
	XMMATRIX GetProjectionMatrix() const;
	XMMATRIX GetViewProjectionMatrix() const;
	XMMATRIX GetOrthoMatrix() const;
};