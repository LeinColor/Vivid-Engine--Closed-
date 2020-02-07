#pragma once

class Texture
{
	friend class Importer;
public:
	bool Initialize(ID3D11Device* device, WCHAR* fileName);

	inline ID3D11ShaderResourceView** GetData() { return &resourceView; }


private:
	ID3D11ShaderResourceView* resourceView = nullptr;
};