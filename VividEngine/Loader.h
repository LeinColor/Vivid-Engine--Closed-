#pragma once
#include "DirectX11Wrapper.h"
#include <string>
#include <vector>

using namespace std;

class DirectX11Wrapper;

class Loader {
private:
	static DirectX11Wrapper* graphics;
public:
	static void Initialize();

	static void VertexShader();
	static void PixelShader();
	static void GeometryShader();
	static void HullShader();
	static void DomainShader();
	static void ComputeShader();
	static void InputLayout();
	static void ConstantBuffers();

	static void Meshes();
	static void Textures();
private:
	static void LoadVertexShaderFromFile(const wchar_t* fileName, const char* entryPoint);
	static void LoadPixelShaderFromFile(const wchar_t* fileName, const char* entryPoint);
	static void LoadMeshFromObj(const char* fileName);
	static void LoadLine();

	static void BindConstantBuffer(UINT byteWidth);
	static void BindShader(int num, int VS, int VSBuffer, int inputLayout, int PS, vector<int> CB, int GS = -1, int HS = -1, int DS = -1, int CS = -1);
};