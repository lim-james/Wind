#include "Collider.h"

#include <Events/EventsManager.h>

Collider::Collider() {
	enterCallback = stayCallback = exitCallback = nullptr;
}

void Collider::Initialize() {
	Component::Initialize();
	enterCallback = stayCallback = exitCallback = nullptr;
}

void Collider::SetActive(const bool& state) {
	Component::SetActive(state);
	Events::EventsManager::GetInstance()->Trigger("COLLIDER_ACTIVE", new Events::AnyType<Collider*>(this));
}

void Collider::OnCollisionEnter(Entity* const object) const {
	if (enterCallback) enterCallback(object);
}

void Collider::OnCollisionStay(Entity* const object) const {
	if (stayCallback) stayCallback(object);
}

void Collider::OnCollisionExit(Entity* const object) const {
	if (exitCallback) exitCallback(object);
}
