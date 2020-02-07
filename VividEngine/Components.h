#pragma once
#include "stdafx.h"

struct Transform {
	XMFLOAT3	position;
	XMFLOAT4	rotation;		// this is quaternion so it has 4 values (x,y,z,w)
	XMFLOAT3	scale;
	XMFLOAT4X4	world;
};

struct Camera {
	XMFLOAT3 eye   = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 focus = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 up    = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT4X4 view       = IDENTITY_MATRIX;
	XMFLOAT4X4 projection = IDENTITY_MATRIX;
	XMFLOAT4X4 ortho      = IDENTITY_MATRIX;

	float farZ = 800.0f;
	float nearZ = 0.001f;
	float fieldOfView = 3.141592654f / 4.0f;
};

struct Light {

};

struct Renderer3D {
	uint32_t meshID;
	uint32_t shaderID;
	uint32_t textureID;
};