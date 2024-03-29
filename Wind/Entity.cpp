#include "Entity.h"

#include <Events/EventsManager.h>

Entity::Entity()
	: used(false)
	, staticEntity(false)
	, tag("") 
	, parent(nullptr) {
	Initialize();
}

Entity::~Entity() {
	children.clear();
	components.clear();
}

void Entity::Initialize() {
	for (const auto& c : components)
		c.second->Initialize();
}

void Entity::Destroy() {
	used = false;
	//ClearChildren();
	children.clear();
	parent = nullptr;
	//if (parent)
	//	parent->RemoveChild(this);
	SetActive(false);

	Events::EventsManager::GetInstance()->Trigger("ENTITY_DESTROY", new Events::AnyType<Entity*>(this));
}

const std::string& Entity::GetTag() const {
	return tag;
}

void Entity::SetTag(const std::string& _tag) {
	auto event = new Events::TagChange(tag, this);
	tag = _tag;
	Events::EventsManager::GetInstance()->Trigger("TAG_CHANGE", event);
}

Entity* const Entity::GetParent() const {
	return parent;
}

void Entity::SetParent(Entity * const entity) {
	if (parent)
		parent->RemoveChild(this);

	parent = entity;

	if (entity)
		entity->children.push_back(this);
}

void Entity::AddChild(Entity* const entity) {
	if (entity->parent) {
		if (entity->parent == this)
			return;
		else
			entity->parent->RemoveChild(entity);
	}

	entity->parent = this;
	children.push_back(entity);
}

void Entity::RemoveChild(Entity* const entity) {
	auto position = vfind(children, entity);
	if (position == children.end()) return;
	children.erase(position);
}

void Entity::ClearChildren() {
	for (auto& child : children)
		child->SetParent(nullptr);

	children.clear();
}

std::vector<Entity*>& Entity::GetChildren() {
	return children;
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
