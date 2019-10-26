#include "Entity.h"

#include <Events/EventsManager.h>

Entity::Entity()
	: used(false)
	, staticEntity(false)
	, tag("") {
	Initialize();
}

Entity::~Entity() {
	components.clear();
}

void Entity::Initialize() {
	for (const auto& c : components)
		c.second->Initialize();
}

void Entity::Destroy() {
	used = false;
	SetActive(false);

	Events::EventsManager::GetInstance()->Trigger("ENTITY_DESTROY", new Events::AnyType<Entity*>(this));
}

void Entity::SetActive(const bool& state) {
	for (const auto& c : components)
		c.second->SetActive(state);
}

void Entity::Use() {
	used = true;
	SetActive(true);

	Events::EventsManager::GetInstance()->Trigger("ENTITY_USED", new Events::AnyType<Entity*>(this));
}

const bool& Entity::IsUsed() const {
	return used;
}
