#include "stdafx.h"
#include "Loader.h"
#include "Manager.h"
#include <vector>

void Loader::InputLayout()
{
	auto& vertexShaders = Manager::vertexShaders;
	auto& vertexShaderBuffers = Manager::vertexShaderBuffers;

	{
		ID3D11InputLayout* inputLayout = nullptr;
		D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	 0, 0,							  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		graphics->GetDevice()->CreateInputLayout(layoutDesc, ARRAYSIZE(layoutDesc), vertexShaderBuffers[0]->GetBufferPointer(), vertexShaderBuffers[0]->GetBufferSize(), &inputLayout);
		Manager::inputLayouts.push_back(inputLayout);
	}
	{
		ID3D11InputLayout* inputLayout = nullptr;
		D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	 0, 0,							  D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",	  0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		graphics->GetDevice()->CreateInputLayout(layoutDesc, ARRAYSIZE(layoutDesc), vertexShaderBuffers[1]->GetBufferPointer(), vertexShaderBuffers[1]->GetBufferSize(), &inputLayout);
		Manager::inputLayouts.push_back(inputLayout);
	}
}