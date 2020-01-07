#pragma once
class Object;
class Component;

constexpr float SCREEN_DEPTH = 800.0f;
constexpr float SCREEN_NEAR = 0.001f;
constexpr float FIELD_OF_VIEW = 3.141592654f / 4.0f;

class Camera : public Component
{
public:
	static const uint32_t type = CAMERA;
	virtual bool IsSameType(const uint32_t value) const override;
private:
	XMFLOAT4X4	view;
	XMFLOAT4X4	projection;
	XMFLOAT4X4	ortho;
	float		fieldOfView;

public:
	ALIGNED_ALLOC_16;

	Camera() {}
	Camera(const Camera&) {}
	~Camera() {}

	XMFLOAT3 Render(int screenWidth, int screenHeight, float screenDepth, float screenNear);

	XMMATRIX GetViewMatrix() const;
	XMMATRIX GetProjectionMatrix() const;
	XMMATRIX GetViewProjectionMatrix() const;
	XMMATRIX GetOrthoMatrix() const;
};