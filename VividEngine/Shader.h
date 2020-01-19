#pragma once
#include "stdafx.h"
#include <unordered_map>
using namespace std;

class Shader {
public:
	VSType VS;
	PSType PS;
	GSType GS;
	HSType HS;
	DSType DS;
	CSType CS;

	ILType IL;
	vector<CBType> CBSet;

	ID3D11Buffer** resourceBuffers;
	ID3D11SamplerState** samplerStates;

	static void Initialize();
private:
};

struct VertexBuffer {
	ID3D11Buffer* buffer = nullptr;
	D3D11_BUFFER_DESC* desc = nullptr;
};

struct IndexBuffer {
	ID3D11Buffer* buffer = nullptr;
	D3D11_BUFFER_DESC* desc = nullptr;
};

struct InputLayout {
	ID3D11InputLayout* layout = nullptr;
	D3D11_INPUT_ELEMENT_DESC* desc = nullptr;
};

struct ConstantBuffer {
	ID3D11Buffer* constantBuffer = nullptr;
	D3D11_BUFFER_DESC* desc = nullptr;
};

struct VertexShader {
private:
	ID3D11VertexShader* shader = nullptr;
public:
	inline ID3D11VertexShader* Get() { return shader; }
};

struct PixelShader {
private:
	ID3D11PixelShader* shader = nullptr;
public:
	inline ID3D11PixelShader* Get() { return shader; }
};

struct GeometryShader {
private:
	ID3D11GeometryShader* shader = nullptr;
public:
	inline ID3D11GeometryShader* Get() { return shader; }
};

struct HullShader {
private:
	ID3D11HullShader* shader = nullptr;
public:
	inline ID3D11HullShader* Get() { return shader; }
};

struct DomainShader {
private:
	ID3D11DomainShader* shader = nullptr;
public:
	inline ID3D11DomainShader* Get() { return shader; }
};

struct ComputeShader {
private:
	ID3D11ComputeShader* shader = nullptr;
public:
	inline ID3D11ComputeShader* Get() { return shader; }
};