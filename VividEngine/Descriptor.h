#pragma once
#include "stdafx.h"

using namespace DirectX;
// Define enum here
enum INPUT_LAYOUT_TYPE {
	UNDEFINED_INPUT_LAYOUT = -1,
	DEFAULT,
	POS,
	POS_TEX_NORMAL,
};

enum MESH_ID {
	UNDEFINED_MESH = -1,
	MESH_CUBE,
	MESH_SPHERE,
	MESH_CONE,
	MESH_PLANE,
	MESH_CYLINDER,
	MESH_LINE,
	MESH_DEFAULT_COUNT
};

enum SHADER_ID {
	UNDEFINED_SHADER = -1,
	SHADER_DEBUG,
	SHADER_BLINN_PHONG,
};

enum MATERIAL_ID {
	UNDEFINED_MATERIAL = -1,
	MATERIAL_DEBUG,
	MATERIAL_BLINN_PHONG,
};

struct GizmoLine {
	XMFLOAT3 startPoint = XMFLOAT3(0, 0, 0);
	XMFLOAT3 endPoint = XMFLOAT3(0, 0, 0);;
	XMFLOAT4 color = XMFLOAT4(1, 1, 1, 1);
};

struct VertexDefault {
	XMFLOAT3 positions;
	XMFLOAT2 texcoords;
	XMFLOAT3 normals;
};

struct VertexPos {
	XMFLOAT3 positions;
};

struct VertexPosTexNor {
	XMFLOAT3 positions;
	XMFLOAT2 texcoords;
	XMFLOAT3 normals;
};

struct MatrixBufferType {
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX projection;
};

struct CameraBufferType {
	XMFLOAT3 cameraPosition;
	float padding;
};

struct LightBufferType {
	XMFLOAT4 ambientColor;
	XMFLOAT4 diffuseColor;
	XMFLOAT3 lightDirection;
	float specularPower;
	XMFLOAT4 specularColor;
};

struct ColorBufferType {
	XMFLOAT4 color;
};