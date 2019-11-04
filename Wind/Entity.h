#ifndef ENTITY_H
#define ENTITY_H

#include "ComponentsManager.h"

#include <Events/Event.h>
#include <MACROS.h>

#include <map>
#include <typeindex>
#include <string>

class Entity;

namespace Events {
	struct TagChange : Event {
		const std::string previous;
		Entity * const entity;

		TagChange(const std::string& previous, Entity * const entity)
			: previous(previous)
			, entity(entity) {}
	};
}

class Entity {

	std::string tag;

	Entity* parent;
	std::vector<Entity*> children;

	bool used;
	bool staticEntity;

	ComponentsManager* componentsManager;
	std::map<std::type_index, Component*> components;

public:

	Entity();
	virtual ~Entity();

	virtual void Build() = 0;

	virtual void Initialize();
	void Destroy();

	const std::string& GetTag() const;
	void SetTag(const std::string& _tag);

	Entity* const GetParent() const;
	virtual void SetParent(Entity * const entity);

	void AddChild(Entity* const entity);
	void RemoveChild(Entity* const entity);

	void ClearChildren();
	std::vector<Entity*>& GetChildren();

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
	return static_cast<ComponentType * const>(components[indexof(ComponentType)]);
}

#endif
