#include "stdafx.h"
#include "WICTextureLoader.h"
#include "Texture.h"

bool Texture::Initialize(ID3D11Device* device, WCHAR* filename)
{
	if (FAILED(CreateWICTextureFromFile(device, filename, nullptr, &m_texture)))
	{
		return false;
	}
	return true;
}


void Texture::Shutdown()
{
	if (m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}
}

ID3D11ShaderResourceView* Texture::GetTexture()
{
	return m_texture;
}