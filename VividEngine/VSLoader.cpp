#include "stdafx.h"
#include "Loader.h"
#include "Debug.h"
#include "DirectX11Wrapper.h"
#include "Renderer.h"
#include "Manager.h"

void Loader::VertexShader()
{
	LoadVertexShaderFromFile(L"../VividEngine/Debug.vs", "DebugVertexMain");
	LoadVertexShaderFromFile(L"../VividEngine/BlinnPhong.vs", "LightVertexMain");
}

void Loader::LoadVertexShaderFromFile(const wchar_t* fileName, const char* entryPoint)
{
	ID3D11VertexShader* vertexShader = nullptr;
	ID3D10Blob* vertexShaderBuffer = nullptr;
	if (FAILED(D3DCompileFromFile(fileName, nullptr, nullptr, entryPoint, "vs_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, nullptr)))
	{
		Debug::Log("Vertex Shader load failed!");
		return;
	}

	graphics->GetDevice()->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), nullptr, &vertexShader);
	Manager::vertexShaders.push_back(vertexShader);
	Manager::vertexShaderBuffers.push_back(vertexShaderBuffer);
}