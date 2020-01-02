#pragma once

#include <vector>
#include <memory>
#include <functional>

#include "Component.h"
class Object {
private:
	std::vector<std::unique_ptr<Component> > components;

public:
	Object();

	UINT state = DEFAULT;

	template<class ComponentType, typename... Args>
	void AddComponent(Args&&... params);

	template<class ComponentType>
	ComponentType& GetComponent();
};

template<class ComponentType, typename... Args>
void Object::AddComponent(Args&&... params) {
	components.emplace_back(std::make_unique<ComponentType>(std::forward<Args>(params)...));
	GetComponent<ComponentType>().owner = this;
}

template<class ComponentType>
ComponentType& Object::GetComponent() {
	for (auto&& component : components) {
		if (component->IsSameType(ComponentType::type))
			return *static_cast<ComponentType*>(component.get());
	}
	return *std::unique_ptr<ComponentType>(nullptr);
}