#pragma once


//class TransformComponent {
//private:
//	XMFLOAT3	position;
//	XMFLOAT4	rotation;		// this is quaternion so it has 4 values (x,y,z,w)
//	XMFLOAT3	scale;
//	XMFLOAT4X4	world;
//public:
//	TransformComponent();
//
//	void SetPosition(const float x, const float y, const float z);
//	void SetRotation(const float x, const float y, const float z, const float w);
//	void SetScale(const float x, const float y, const float z);
//
//	void Translate(const float x, const float y, const float z);
//	void Rotate(const float x, const float y, const float z);		// Rotate by eulor angle
//	void RotateQuaternion(const XMFLOAT4&);							// Rotate by quaternion angle
//
//	void LookAt(const XMVECTOR& origin, const XMVECTOR& target, const XMVECTOR& up);
//
//	void UpdateWorldMatrix();
//
//	XMMATRIX GetWorldMatrix() const;
//
//	XMFLOAT3 GetPosition() const;
//	XMFLOAT4 GetRotation() const;
//	XMFLOAT3 GetScale() const;
//
//	XMVECTOR GetPositionVector() const;
//	XMVECTOR GetRotationVector() const;
//	XMVECTOR GetScaleVector() const;
//
//
//};


//class CameraComponent
//{
//	XMFLOAT3 eye;
//	XMFLOAT3 focus;
//	XMFLOAT3 up;
//	float farZ = 800.0f;
//	float nearZ = 0.001f;
//	float fieldOfView = 3.141592654f / 4.0f;
//
//	XMFLOAT4X4	view;
//	XMFLOAT4X4	projection;
//	XMFLOAT4X4	ortho;
//
//
//public:
//	ALIGNED_ALLOC_16;
//
//	TransformComponent transform;
//
//	CameraComponent() {}
//	CameraComponent(const CameraComponent&) {}
//	~CameraComponent() {}
//
//	void Update(int screenWidth, int screenHeight);
//
//	XMVECTOR GetEye() const;
//	XMVECTOR GetFocus() const;
//	XMVECTOR GetUp() const;
//	XMMATRIX GetViewMatrix() const;
//	XMMATRIX GetProjectionMatrix() const;
//	XMMATRIX GetViewProjectionMatrix() const;
//	XMMATRIX GetOrthoMatrix() const;
//};