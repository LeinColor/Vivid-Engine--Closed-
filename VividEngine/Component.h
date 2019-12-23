#pragma once

class Component {
public:
	static const uint32_t type = COMPONENT;
	virtual bool IsSameType(const uint32_t value) const {
		return type == value;
	}
public:
	virtual ~Component() = default;
};