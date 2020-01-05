#pragma once
#include "stdafx.h"

using namespace DirectX;
// Define enum here
enum TYPE {
	UNKNOWN,
	OBJECT,
	COMPONENT,
	TRANSFORM,
	CAMERA,
	SPRITE_RENDERER,
	RENDERER_3D,
	LIGHT,
};

enum OBJECT_STATE {
	DEFAULT,
	DEBUG,
};

enum MESH_ENUM {
	MESH_CUBE,
	MESH_SPHERE,
	MESH_CONE,
	MESH_PLANE,
	MESH_CYLINDER,
	MESH_DEFAULT_COUNT
};

enum VERTEX_SHADER_ENUM {
	VS_DEBUG,
	VS_BLINN_PHONG,
};

enum INPUT_LAYOUT_ENUM {
	INPUT_LAYOUT_POS,
	INPUT_LAYOUT_POS_COLOR_NORMAL,
	INPUT_LAYOUT_POS_TEX_NORMAL,
};

enum PIXEL_SHADER_ENUM {
	PS_DEBUG,
	PS_BLINN_PHONG,
};

enum CONSTANT_BUFFER_ENUM {
	CONSTANT_BUFFER_WVP,
	CONSTANT_BUFFER_CAMERA,
	CONSTANT_BUFFER_LIGHT,
	CONSTANT_BUFFER_COLOR,
	CONSTANT_BUFFER_DEFAULT_COUNT
};

struct GizmoLine {
	XMFLOAT3 startPoint = XMFLOAT3(0, 0, 0);
	XMFLOAT3 endPoint = XMFLOAT3(0, 0, 0);;
	XMFLOAT4 color = XMFLOAT4(1, 1, 1, 1);
};


// Define struct here
struct vertex_P_t {		// Position
	XMFLOAT3 position;
};

struct vertex_PC_t {	// Position,  Color
	XMFLOAT3 position;
	XMFLOAT4 color;
};

struct vertex_PT_t {	// Position, Texture
	XMFLOAT3 position;
	XMFLOAT2 texture;
};

struct vertex_PCN_t {	// Position, Color, Normal
	XMFLOAT3 position;
	XMFLOAT4 color;
	XMFLOAT3 normal;
};

struct vertex_PTN_t {	// Position, Texture, Normal
	XMFLOAT3 position;
	XMFLOAT2 texture;
	XMFLOAT3 normal;
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