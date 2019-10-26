#ifndef ENTITY_H
#define ENTITY_H

#include "ComponentsManager.h"

#include <MACROS.h>

#include <map>
#include <typeindex>
#include <string>

class Entity {

	bool used;
	bool staticEntity;

	ComponentsManager* componentsManager;
	std::map<std::type_index, Component*> components;

public:

	std::string tag;

	Entity();
	virtual ~Entity();

	virtual void Build() = 0;

	virtual void Initialize();
	void Destroy();

	void SetActive(const bool& state);

	void Use();
	const bool& IsUsed() const;

	template<typename ComponentType>
	const bool HasComponent() const;

	template<typename ComponentType>
	void AddComponent();

	template<typename ComponentType>
	ComponentType* const GetComponent();

	friend class EntityManager;

};

template<typename ComponentType>
const bool Entity::HasComponent() const {
	return components.find(indexof(ComponentType)) != components.end();
}

template<typename ComponentType>
void Entity::AddComponent() {
	if (HasComponent<ComponentType>()) return;

	Component* component = componentsManager->Fetch<ComponentType>();
	component->SetParent(this);
	components[indexof(ComponentType)] = component;
}

template<typename ComponentType>
ComponentType* const Entity::GetComponent() {
	return dynamic_cast<ComponentType * const>(components[indexof(ComponentType)]);
}

#endif
