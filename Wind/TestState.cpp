#include "TestState.h"

#include "Entity.h"
#include "Transform.h"
#include "Render.h"
#include "StateMachine.h"

void States::LeftState::Enter(Entity * const target) {
	target->GetComponent<Render>()->tint.Set(0.f, 1.f, 0.f, 1.f);
}

void States::LeftState::Update(Entity * const target, const float& dt) {
	auto transform = target->GetComponent<Transform>();
	transform->translation.x -= 10.f * dt;
	if (transform->translation.x < -10.f)
		target->GetComponent<StateMachine>()->queuedState = new RightState;
}

void States::LeftState::Exit(Entity * const target) {
	delete this;
}

void States::RightState::Enter(Entity * const target) {
	target->GetComponent<Render>()->tint.Set(0.f, 0.f, 1.f, 1.f);
}

void States::RightState::Update(Entity * const target, const float& dt) {
	auto transform = target->GetComponent<Transform>();
	transform->translation.x += 10.f * dt;
	if (transform->translation.x > 10.f)
		target->GetComponent<StateMachine>()->queuedState = new LeftState;
}

void States::RightState::Exit(Entity * const target) {
	delete this;
}

void States::StopState::Enter(Entity * const target) {
	target->GetComponent<Render>()->tint.Set(1.f, 0.f, 0.f, 1.f);
}

void States::StopState::Update(Entity * const target, const float& dt) {
}

void States::StopState::Exit(Entity * const target) {
	delete this;
}
