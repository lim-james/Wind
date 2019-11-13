#include "Ghost.h"

#include "Transform.h"
#include "Animation.h"
#include "StateContainer.h"
#include "Line.h"

#include <Events/EventsManager.h>

Ghost::Ghost() {
	direction.Set(0.f, 1.f, 0.f);
}

void Ghost::Initialize() {
	AISprite::Initialize();

	Events::EventsManager::GetInstance()->Subscribe("GHOST_MODE", &Ghost::StateHandler, this);
}

void Ghost::InvertDirection() {
	direction = -direction;
}

const vec3f& Ghost::GetDock() const {
	return dock;
}

void Ghost::SetDock(const vec3f& position) {
	dock = position;
}

const vec3f& Ghost::GetCorner() const {
	return corner;
}

void Ghost::SetCorner(const vec3f& position) {
	corner = position;
}

const std::string& Ghost::GetChaseState() const {
	return chaseState;
}

void Ghost::SetChaseState(const std::string& state) {
	chaseState = state;
}

Ghost* const Ghost::GetPartner() const {
	return partner;
}

void Ghost::SetPartner(Ghost* const ghost) {
	partner = ghost;
}

void Ghost::SetDirection(const vec3f& _direction) {
	AISprite::SetDirection(_direction);

	if (GetComponent<StateContainer>()->currentState == "FRIGHTENED") return;

	auto animation = GetComponent<Animation>();

	if (direction.x) {
		if (direction.x > 0) {
			animation->queued = "RIGHT";
		} else {
			animation->queued = "LEFT";
		}
	} else {
		if (direction.y > 0) {
			animation->queued = "UP";
		} else {
			animation->queued = "DOWN";
		}
	}
}



void Ghost::StateHandler(Events::Event* event) {
	auto mode = static_cast<Events::AnyType<std::string>*>(event)->data;

	if (mode == "CHASE")
		GetComponent<StateContainer>()->queuedState = chaseState;
	else if (mode == "SCATTER")
		GetComponent<StateContainer>()->queuedState = "GHOST_SCATTER";
	else if (mode == "FRIGHTENED")
		GetComponent<StateContainer>()->queuedState = "GHOST_FRIGHTENED";
}

void Ghost::SetEnterTarget() {
	const auto& position = GetComponent<Transform>()->translation;

	const vec3f dir = Math::Normalized(GetDestination() - position);
	const vec3f fDir = Math::Abs(dir);

	if (Math::LengthSquared(fDir) > 0) {
		if (fDir.y < fDir.x)
			target.x += dir.x / fDir.x;
		else
			target.y += dir.y / fDir.y;
	}
}

void Ghost::Move(const float& dt) {
	auto& position = GetComponent<Transform>()->translation;
	vec3f dir = target - position;

	if (Math::Length(dir) <= speed * dt) {
		position = target;
		if (GetComponent<StateContainer>()->currentState == "GHOST_ENTERING")
			SetEnterTarget();
		else
			SetNewTarget();
	} else {
		Math::Normalize(dir);
		position += dir * speed * static_cast<float>(dt);
	}
}

