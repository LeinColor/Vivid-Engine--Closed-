#pragma once

class Texture;
class Sprite;

class SpriteRenderer
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

public:
	SpriteRenderer() {}
	SpriteRenderer(const SpriteRenderer&) {}
	~SpriteRenderer() {}

	void LoadSprite(ID3D11Device* device, WCHAR* fileName);
	ID3D11ShaderResourceView* GetSprite();

private:
	Sprite* owner = nullptr;
	bool Initialize(ID3D11Device* device);
	void Shutdown();
	bool Update(ID3D11DeviceContext* deviceContext, int positionX, int positionY, int screenWidth, int screenHeight);
	void Render(ID3D11DeviceContext*);


private:
	ID3D11Buffer* vertexBuffer = nullptr;
	ID3D11Buffer* indexBuffer = nullptr;
	int vertexCount = 0;
	int indexCount = 0;

	ID3D11ShaderResourceView* texture = nullptr;

	int spriteWidth = 0;
	int spriteHeight = 0;

	int currPosX = 0;
	int currPosY = 0;
	int prevPosX = 0;
	int prevPosY = 0;
};