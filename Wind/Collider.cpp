#include "Collider.h"

#include <Events/EventsManager.h>

Collider::Collider() {
	onCollisionEnter = [](Entity * const) {};
}

void Collider::Initialize() {
	Component::Initialize();

	onCollisionEnter = [](Entity * const) {};
}

void Collider::SetActive(const bool& state) {
	Component::SetActive(state);
	Events::EventsManager::GetInstance()->Trigger("COLLIDER_ACTIVE", new Events::AnyType<Collider*>(this));
}
