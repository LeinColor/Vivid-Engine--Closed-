#pragma once

struct Transform {
	XMFLOAT3	position;
	XMFLOAT4	rotation;		// this is quaternion so it has 4 values (x,y,z,w)
	XMFLOAT3	scale;
	XMFLOAT4X4	world;
};

struct Camera {
	XMFLOAT3 eye;
	XMFLOAT3 focus;
	XMFLOAT3 up;
	XMFLOAT4X4	view;
	XMFLOAT4X4	projection;
	XMFLOAT4X4	ortho;

	float farZ = 800.0f;
	float nearZ = 0.001f;
	float fieldOfView = 3.141592654f / 4.0f;
};

struct Light {

};