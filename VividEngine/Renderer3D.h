#pragma once

class Object;
class Component;
class Mesh;
class Material;

class Renderer3D : public Component {
public:
	static const uint32_t type = RENDERER_3D;
	virtual bool IsSameType(const uint32_t value) const override;

	void SetMesh(Mesh* other);

public:
	XMFLOAT4 color;
	Mesh* mesh;
	Material* material;
private:
};