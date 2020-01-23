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

Shader::Shader(const wchar_t* fileName, INPUT_LAYOUT inputLayoutTypeValue)
{
	BYTE flag = GetSubShaderFlag(fileName);

	if (flag & VERTEX_SHADER)
	{
		CompileFromFile(fileName, SHADER_TYPE::VERTEX_SHADER);
		CreateSubShader(SHADER_TYPE::VERTEX_SHADER);
	}
	if (flag & PIXEL_SHADER)
	{
		CompileFromFile(fileName, SHADER_TYPE::PIXEL_SHADER);
		CreateSubShader(SHADER_TYPE::PIXEL_SHADER);
	}
	if (flag & GEOMETRY_SHADER)
	{
		CompileFromFile(fileName, SHADER_TYPE::GEOMETRY_SHADER);
		CreateSubShader(SHADER_TYPE::GEOMETRY_SHADER);
	}
	if (flag & HULL_SHADER)
	{
		CompileFromFile(fileName, SHADER_TYPE::HULL_SHADER);
		CreateSubShader(SHADER_TYPE::HULL_SHADER);
	}
	if (flag & DOMAIN_SHADER)
	{
		CompileFromFile(fileName, SHADER_TYPE::DOMAIN_SHADER);
		CreateSubShader(SHADER_TYPE::DOMAIN_SHADER);
	}
	if (flag & COMPUTE_SHADER)
	{
		CompileFromFile(fileName, SHADER_TYPE::COMPUTE_SHADER);
		CreateSubShader(SHADER_TYPE::COMPUTE_SHADER);
	}
	CreateInputLayout(inputLayoutTypeValue);


}

// TODO: support all of layout using shader reflection later.
void Shader::CreateInputLayout(INPUT_LAYOUT inputLayoutTypeValue)
{
	HRESULT hr = E_FAIL;
	GraphicsAPI* graphics = Renderer::GetGraphicsAPI();

	inputLayoutType = inputLayoutTypeValue;

	switch (inputLayoutType) {
	case DEFAULT:
		// SemanticName, SemanticIndex, Format, InputSlot, AlignedByteOffset, InputSlotClass, InstanceDataStepRate
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXTURE" , 0, DXGI_FORMAT_R32G32_FLOAT		, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL"	, 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		graphics->GetDevice()->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), blobs.vsBlob->GetBufferPointer(), blobs.vsBlob->GetBufferSize(), &inputLayout);
		break;
	case POS:
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		graphics->GetDevice()->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), blobs.vsBlob->GetBufferPointer(), blobs.vsBlob->GetBufferSize(), &inputLayout);
		break;
	case POS_TEX_NORMAL:
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXTURE" , 0, DXGI_FORMAT_R32G32_FLOAT		, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL"	, 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		graphics->GetDevice()->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), blobs.vsBlob->GetBufferPointer(), blobs.vsBlob->GetBufferSize(), &inputLayout);
		break;
	case UNDEFINED:
		Debug::Log("Exception: You tried to create undefined input layout.");
		break;
	}
}

BYTE Shader::GetSubShaderFlag(const wchar_t* fileName)
{
	BYTE flag;

	const string defaultPath = "../VividEngine/HLSL/";
	const string formats[] = { "_vs.hlsl", "_ps.hlsl", "_gs.hlsl", "_hs.hlsl", "_ds.hlsl", "_cs.hlsl" };
	const BYTE stage[] = { VERTEX_SHADER, PIXEL_SHADER, GEOMETRY_SHADER, HULL_SHADER, DOMAIN_SHADER, COMPUTE_SHADER };

	for (int i = 0; i < SHADER_STAGE_COUNT; i++) {
		string existingPath = defaultPath + formats[i];

		ifstream f(existingPath);
		if (f.is_open())
		{
			flag |= stage[i];
		}
		f.close();
	}
	return flag;
}

void Shader::CompileFromFile(const wchar_t* fileName, SHADER_TYPE subShaderType)
{
	HRESULT result;

	switch (subShaderType) {
	case VERTEX_SHADER:
		result = D3DCompileFromFile(fileName, nullptr, nullptr, "VSMain", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &blobs.vsBlob, nullptr);
		break;
	case PIXEL_SHADER:
		result = D3DCompileFromFile(fileName, nullptr, nullptr, "PSMain", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &blobs.psBlob, nullptr);
		break;
	case GEOMETRY_SHADER:
		result = D3DCompileFromFile(fileName, nullptr, nullptr, "GSMain", "gs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &blobs.gsBlob, nullptr);
		break;
	case HULL_SHADER:
		result = D3DCompileFromFile(fileName, nullptr, nullptr, "HSMain", "hs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &blobs.hsBlob, nullptr);
		break;
	case DOMAIN_SHADER:
		result = D3DCompileFromFile(fileName, nullptr, nullptr, "DSMain", "ds_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &blobs.dsBlob, nullptr);
		break;
	case COMPUTE_SHADER:
		result = D3DCompileFromFile(fileName, nullptr, nullptr, "CSMain", "cs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &blobs.csBlob, nullptr);
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