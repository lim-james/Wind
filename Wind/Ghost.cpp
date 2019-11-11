#include "Ghost.h"

#include "Transform.h"
#include "StateContainer.h"
#include "Line.h"
#include "MapEvents.h"

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

void Ghost::StateHandler(Events::Event* event) {
	auto mode = static_cast<Events::AnyType<std::string>*>(event)->data;

	if (mode == "CHASE")
		GetComponent<StateContainer>()->queuedState = chaseState;
	else if (mode == "SCATTER")
		GetComponent<StateContainer>()->queuedState = "GHOST_SCATTER";
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

void Ghost::SetNewTarget() {
	const auto& position = GetComponent<Transform>()->translation;
	
	vec3f newTarget = position + direction;

	bool avail = false;
	vec2i check(static_cast<int>(newTarget.x), static_cast<int>(newTarget.y));
	Events::SpotAvailability* event = new Events::SpotAvailability(&avail, check);
	Events::EventsManager::GetInstance()->Trigger("SPOT_AVAIL",  event);

	float closest = -1.f;

	if (avail) {
		closest = Math::LengthSquared(destination - newTarget);
		target = newTarget;
	}	
	
	// check on other axis

	vec3f other(direction.y, direction.x, 0.f);

	newTarget = position + other;

	avail = false;
	check.Set(static_cast<int>(newTarget.x), static_cast<int>(newTarget.y));
	event = new Events::SpotAvailability(&avail, check);
	Events::EventsManager::GetInstance()->Trigger("SPOT_AVAIL",  event);

	if (avail) {
		const float distance = Math::LengthSquared(destination - newTarget);
		if (closest < 0 || distance <= closest) {
			closest = distance;
			direction = other;
			target = newTarget;
		}
	}

	newTarget = position - other;
	if (closest < 0) {
		direction = -other;
		target = newTarget;
		return;
	}	

	avail = false;
	check.Set(static_cast<int>(newTarget.x), static_cast<int>(newTarget.y));
	event = new Events::SpotAvailability(&avail, check);
	Events::EventsManager::GetInstance()->Trigger("SPOT_AVAIL",  event);

	if (avail) {
		const float distance = Math::LengthSquared(destination - newTarget);
		if (distance <= closest) {
			direction = -other;
			target = newTarget;
		}
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

