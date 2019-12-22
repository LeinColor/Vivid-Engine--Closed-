#pragma once

class Sprite;

class RectTransform {
private:
	XMFLOAT3	position;
	XMFLOAT4	rotation;
	XMFLOAT3	scale;
	XMFLOAT4X4	world;

public:
	Sprite* owner = nullptr;

	RectTransform();

	void SetPosition(const float x, const float y, const float z);
	void SetRotation(float, float, float);
	void SetScale(float, float, float);

	void Translate(const float x, const float y, const float z);
	void Rotate(const XMFLOAT3&);

	void UpdateWorldMatrix();
	XMMATRIX GetWorldMatrix() const;

	XMFLOAT3 GetPosition() const;
	XMFLOAT4 GetRotation() const;
	XMFLOAT3 GetScale() const;

	XMVECTOR GetPositionVector() const;
	XMVECTOR GetRotationVector() const;
	XMVECTOR GetScaleVector() const;
};