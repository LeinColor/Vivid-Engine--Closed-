#pragma once
class Object;
class Component;

class Camera : public Component
{
public:
	static const uint32_t type = CAMERA;
	virtual bool IsSameType(const uint32_t value) const override;
private:
	XMFLOAT3 eye;
	XMFLOAT3 focus;
	XMFLOAT3 up;
	float farZ = 800.0f;
	float nearZ = 0.001f;
	float fieldOfView = 3.141592654f / 4.0f;

	XMFLOAT4X4	view;
	XMFLOAT4X4	projection;
	XMFLOAT4X4	ortho;

public:
	ALIGNED_ALLOC_16;

	Camera() {}
	Camera(const Camera&) {}
	~Camera() {}

	void Update(int screenWidth, int screenHeight);

	XMVECTOR GetEye() const;
	XMVECTOR GetFocus() const;
	XMVECTOR GetUp() const;
	XMMATRIX GetViewMatrix() const;
	XMMATRIX GetProjectionMatrix() const;
	XMMATRIX GetViewProjectionMatrix() const;
	XMMATRIX GetOrthoMatrix() const;
};