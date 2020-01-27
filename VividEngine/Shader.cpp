#include "stdafx.h"
#include "Renderer.h"
#include "GraphicsAPI.h"
#include "Shader.h"
#include "Debug.h"

#include <fstream>
#include <string>
#include <vector>

using namespace vivid;
using namespace std;

void Shader::Initialize()
{

}

Shader::Shader(const std::string& fileName, INPUT_LAYOUT_TYPE inputLayoutTypeValue)
{
	BYTE flag = GetSubShaderFlag(fileName);

	if (flag & VERTEX_SHADER)
	{
		CompileFromFile(fileName + "_vs.hlsl", SHADER_TYPE::VERTEX_SHADER);
		CreateSubShader(SHADER_TYPE::VERTEX_SHADER);
	}
	if (flag & PIXEL_SHADER)
	{
		CompileFromFile(fileName + "_ps.hlsl", SHADER_TYPE::PIXEL_SHADER);
		CreateSubShader(SHADER_TYPE::PIXEL_SHADER);
	}
	if (flag & GEOMETRY_SHADER)
	{
		CompileFromFile(fileName + "_gs.hlsl", SHADER_TYPE::GEOMETRY_SHADER);
		CreateSubShader(SHADER_TYPE::GEOMETRY_SHADER);
	}
	if (flag & HULL_SHADER)
	{
		CompileFromFile(fileName + "_hs.hlsl", SHADER_TYPE::HULL_SHADER);
		CreateSubShader(SHADER_TYPE::HULL_SHADER);
	}
	if (flag & DOMAIN_SHADER)
	{
		CompileFromFile(fileName + "_ds.hlsl", SHADER_TYPE::DOMAIN_SHADER);
		CreateSubShader(SHADER_TYPE::DOMAIN_SHADER);
	}
	if (flag & COMPUTE_SHADER)
	{
		CompileFromFile(fileName + "_cs.hlsl", SHADER_TYPE::COMPUTE_SHADER);
		CreateSubShader(SHADER_TYPE::COMPUTE_SHADER);
	}
	CreateInputLayout(inputLayoutTypeValue);
}

// TODO: support all of layout using shader reflection later.
void Shader::CreateInputLayout(INPUT_LAYOUT_TYPE inputLayoutTypeValue)
{
	HRESULT hr = E_FAIL;
	GraphicsAPI* graphics = Renderer::GetGraphicsAPI();

	inputLayoutType = inputLayoutTypeValue;

	switch (inputLayoutType) {
	case DEFAULT:
	{
		// SemanticName, SemanticIndex, Format, InputSlot, AlignedByteOffset, InputSlotClass, InstanceDataStepRate
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXTURE" , 0, DXGI_FORMAT_R32G32_FLOAT		, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL"	, 0, DXGI_FORMAT_R32G32B32_FLOAT	, 2, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		graphics->GetDevice()->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), blobs.vsBlob->GetBufferPointer(), blobs.vsBlob->GetBufferSize(), &inputLayout);
		break;
	}
	case POS:
	{
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		graphics->GetDevice()->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), blobs.vsBlob->GetBufferPointer(), blobs.vsBlob->GetBufferSize(), &inputLayout);
		break;
	}
	case POS_TEX_NORMAL:
	{
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXTURE" , 0, DXGI_FORMAT_R32G32_FLOAT		, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL"	, 0, DXGI_FORMAT_R32G32B32_FLOAT	, 2, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		graphics->GetDevice()->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), blobs.vsBlob->GetBufferPointer(), blobs.vsBlob->GetBufferSize(), &inputLayout);
		break;
	}
	case UNDEFINED_INPUT_LAYOUT:
		Debug::Log("Exception: You tried to create undefined input layout.");
		break;
	}
}

BYTE Shader::GetSubShaderFlag(const std::string& fileName)
{
	BYTE flag = 0;

	const string defaultPath = "../VividEngine/HLSL/";
	const string formats[] = { "_vs.hlsl", "_ps.hlsl", "_gs.hlsl", "_hs.hlsl", "_ds.hlsl", "_cs.hlsl" };
	const BYTE stage[] = { VERTEX_SHADER, PIXEL_SHADER, GEOMETRY_SHADER, HULL_SHADER, DOMAIN_SHADER, COMPUTE_SHADER };

	for (int i = 0; i < SHADER_STAGE_COUNT; i++) {
		string existingPath = defaultPath + fileName + formats[i];

		ifstream f(existingPath);
		if (f.is_open())
		{
			flag |= stage[i];
		}
		f.close();
	}
	return flag;
}

void Shader::CompileFromFile(const std::string& fileName, SHADER_TYPE subShaderType)
{
	HRESULT result = E_FAIL;
	string path = "../VividEngine/HLSL/";
	path += fileName;

	wstring wString = wstring(path.begin(), path.end());
	const wchar_t* pFileName = wString.c_str();
	
	switch (subShaderType) {
	case VERTEX_SHADER:
		result = D3DCompileFromFile(pFileName, nullptr, nullptr, "VSMain", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &blobs.vsBlob, nullptr);
		break;
	case PIXEL_SHADER:
		result = D3DCompileFromFile(pFileName, nullptr, nullptr, "PSMain", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &blobs.psBlob, nullptr);
		break;
	case GEOMETRY_SHADER:
		result = D3DCompileFromFile(pFileName, nullptr, nullptr, "GSMain", "gs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &blobs.gsBlob, nullptr);
		break;
	case HULL_SHADER:
		result = D3DCompileFromFile(pFileName, nullptr, nullptr, "HSMain", "hs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &blobs.hsBlob, nullptr);
		break;
	case DOMAIN_SHADER:
		result = D3DCompileFromFile(pFileName, nullptr, nullptr, "DSMain", "ds_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &blobs.dsBlob, nullptr);
		break;
	case COMPUTE_SHADER:
		result = D3DCompileFromFile(pFileName, nullptr, nullptr, "CSMain", "cs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &blobs.csBlob, nullptr);
		break;
	}

	if (FAILED(result))
	{
		Debug::Log("Cannot compile shader!");
		return;
	}
}

void Shader::CreateSubShader(SHADER_TYPE subShaderType)
{
	GraphicsAPI* graphics = Renderer::GetGraphicsAPI();

	switch (subShaderType)
	{
	case VERTEX_SHADER:
		graphics->GetDevice()->CreateVertexShader(blobs.vsBlob->GetBufferPointer(), blobs.vsBlob->GetBufferSize(), nullptr, &subShaders.vs);
		break;
	case PIXEL_SHADER:
		graphics->GetDevice()->CreatePixelShader(blobs.psBlob->GetBufferPointer(), blobs.psBlob->GetBufferSize(), nullptr, &subShaders.ps);
		break;
	case GEOMETRY_SHADER:
		graphics->GetDevice()->CreateGeometryShader(blobs.gsBlob->GetBufferPointer(), blobs.gsBlob->GetBufferSize(), nullptr, &subShaders.gs);
		break;
	case HULL_SHADER:
		graphics->GetDevice()->CreateHullShader(blobs.hsBlob->GetBufferPointer(), blobs.hsBlob->GetBufferSize(), nullptr, &subShaders.hs);
		break;
	case DOMAIN_SHADER:
		graphics->GetDevice()->CreateDomainShader(blobs.dsBlob->GetBufferPointer(), blobs.dsBlob->GetBufferSize(), nullptr, &subShaders.ds);
		break;
	case COMPUTE_SHADER:
		graphics->GetDevice()->CreateComputeShader(blobs.csBlob->GetBufferPointer(), blobs.csBlob->GetBufferSize(), nullptr, &subShaders.cs);
		break;
	}
}