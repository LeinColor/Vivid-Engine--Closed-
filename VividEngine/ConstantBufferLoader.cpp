#include "stdafx.h"
#include "Loader.h"
#include "Manager.h"

void Loader::ConstantBuffers()
{
	BindConstantBuffer(sizeof(MatrixBufferType));
	BindConstantBuffer(sizeof(CameraBufferType));
	BindConstantBuffer(sizeof(LightBufferType));
	BindConstantBuffer(sizeof(ColorBufferType));
}

void Loader::BindConstantBuffer(UINT byteWidth)
{
	ID3D11Buffer* constantBuffer = nullptr;
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = byteWidth;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	graphics->GetDevice()->CreateBuffer(&bd, nullptr, &constantBuffer);
	Manager::constantBuffers.push_back(constantBuffer);
}