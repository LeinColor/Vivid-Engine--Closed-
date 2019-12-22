#pragma once

class Texture
{
public:
	Texture() {}
	Texture(const Texture&) {}
	~Texture() {}

	bool Initialize(ID3D11Device* device, WCHAR* fileName);

	ID3D11ShaderResourceView* GetTexture();


private:
	ID3D11ShaderResourceView* textureView = nullptr;
};