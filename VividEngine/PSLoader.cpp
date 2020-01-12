#include "stdafx.h"
#include "Loader.h"
#include "Debug.h"
#include "Manager.h"
using namespace vivid;

void Loader::PixelShader()
{
	LoadPixelShaderFromFile(L"../VividEngine/Debug.ps", "DebugPixelMain");
	LoadPixelShaderFromFile(L"../VividEngine/BlinnPhong.ps", "LightPixelMain");
}

void Loader::LoadPixelShaderFromFile(const wchar_t* fileName, const char* entryPoint)
{
	ID3D11PixelShader* pixelShader = nullptr;
	ID3D10Blob* pixelShaderBuffer = nullptr;
	if (FAILED(D3DCompileFromFile(fileName, nullptr, nullptr, entryPoint, "ps_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, nullptr)))
	{
		Debug::Log("Pixel Shader load failed!");
		return;
	}
	graphics->GetDevice()->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), nullptr, &pixelShader);
	Manager::pixelShaders.push_back(pixelShader);
}