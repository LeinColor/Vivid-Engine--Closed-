#pragma once

#include <vector>
#include <memory>
#include <functional>

#include "Component.h"
class GameObject {
private:
	std::vector<std::unique_ptr<Component> > components;

public:
	template<class ComponentType, typename... Args>
	void AddComponent(Args&&... params);

	template<class ComponentType>
	ComponentType& GetComponent();
};

template<class ComponentType, typename... Args>
void GameObject::AddComponent(Args&&... params) {
	components.emplace_back(std::make_unique<ComponentType>(std::forward<Args>(params)...));
}

template<class ComponentType>
ComponentType& GameObject::GetComponent() {
	for (auto&& component : components) {
		if (component->IsSameType(ComponentType::type))
			return *static_cast<ComponentType*>(component.get());
	}
	return *std::unique_ptr<ComponentType>(nullptr);
}