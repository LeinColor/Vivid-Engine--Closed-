#pragma once

class Component;

class Light : public Component
{
public:
	static const uint32_t type = LIGHT;
	virtual bool IsSameType(const uint32_t value) const override;
public:
	LightBufferType attrib;
};