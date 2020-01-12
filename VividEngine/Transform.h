#pragma once

class Object;
class Component;

class Transform : public Component {
public:
	static const uint32_t type = TRANSFORM;
	virtual bool IsSameType(const uint32_t value) const override;
private:
	XMFLOAT3	position;
	XMFLOAT4	rotation;		// this is quaternion so it has 4 values (x,y,z,w)
	XMFLOAT3	scale;
	XMFLOAT4X4	world;
public:
	Transform();

	void SetPosition(const float x, const float y, const float z);
	void SetRotation(float, float, float, float);
	void SetScale(float, float, float);

	void Translate(const float x, const float y, const float z);
	void Rotate(const float x, const float y, const float z);		// Rotate by eulor angle
	void RotateQuaternion(const XMFLOAT4&);							// Rotate by quaternion angle

	void LookAt(const XMVECTOR& origin, const XMVECTOR& target, const XMVECTOR& up);

	void UpdateWorldMatrix();

	XMMATRIX GetWorldMatrix() const;

	XMFLOAT3 GetPosition() const;
	XMFLOAT4 GetRotation() const;
	XMFLOAT3 GetScale() const;

	XMVECTOR GetPositionVector() const;
	XMVECTOR GetRotationVector() const;
	XMVECTOR GetScaleVector() const;


};