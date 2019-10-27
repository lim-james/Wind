#include "Component.h"

#include <Events/EventsManager.h>

Component::Component()
	: parent(nullptr)
	, active(false) {
}

Component::~Component() {

}

void Component::Initialize() {
	active = false;
}

void Component::SetActive(const bool& state) {
	active = state;
}

const bool& Component::IsActive() const {
	return active;
}

void Component::SetParent(Entity* const entity) {
	if (parent == entity) return;

	parent = entity;
	if (entity) {
		Events::EventsManager::GetInstance()->Trigger("COMPONENT_ATTACHED", new Events::AnyType<Component*>(this));
	} else {
		SetActive(false);
		Events::EventsManager::GetInstance()->Trigger("COMPONENT_DETACHED", new Events::AnyType<Component*>(this));
	}
}

Entity* const Component::GetParent() {
	return parent;
}
