#include "stdafx.h"
#include "WICTextureLoader.h"
#include "Texture.h"

bool Texture::Initialize(ID3D11Device* device, WCHAR* fileName)
{
	if (FAILED(CreateWICTextureFromFile(device, fileName, nullptr, &textureView)))
	{
		return false;
	}
	return true;
}

ID3D11ShaderResourceView* Texture::GetTexture()
{
	return textureView;
}