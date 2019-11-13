#include "AISprite.h"

#include "Transform.h"
#include "Animation.h"
#include "Render.h"
#include "Collider.h"
#include "Script.h"
#include "StateContainer.h"
// Events
#include "MapEvents.h"

#include <Math/Math.hpp>
#include <Math/Random.hpp>
#include <Events/EventsManager.h>

AISprite::AISprite() {
	interest = nullptr;
	direction.Set(1.f, 0.f, 0.f);
}

void AISprite::Build() {
	AddComponent<Transform>();
	AddComponent<Animation>();
	AddComponent<Render>();
	AddComponent<Collider>();
	AddComponent<Script>();
	AddComponent<StateContainer>();
}

void AISprite::Initialize() {
	Entity::Initialize();

	speed = 1.f;

	GetComponent<Script>()->update = std::bind(&AISprite::Update, this, std::placeholders::_1);
}

void AISprite::SetInterest(Entity * const _interest) {
	interest = _interest;
}

Entity * const AISprite::GetInterest() const {
	return interest;
}

const vec3f& AISprite::GetTarget() const {
	return target;
}

void AISprite::SetTarget(const vec3f& value) {
	target = value;
}

const vec3f& AISprite::GetDestination() const {
	return destination;
}

void AISprite::SetDestination(const vec3f& _destination) {
	destination = _destination;
}

const vec3f& AISprite::GetDirection() const {
	return direction;
}

void AISprite::SetSpeed(const float& value) {
	speed = value;
}

void AISprite::SetDirection(const vec3f& _direction) {
	direction = _direction;
}

void AISprite::SetNewTarget() {
	auto em = Events::EventsManager::GetInstance();

	const auto& position = GetComponent<Transform>()->translation;
	
	vec3f newTarget = position + direction;

	bool avail = false;
	vec2i check(static_cast<int>(newTarget.x), static_cast<int>(newTarget.y));
	Events::MapData* event = new Events::MapData(&avail, check);
	em->Trigger("SPOT_AVAIL", event);

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
	event = new Events::MapData(&avail, check);
	em->Trigger("SPOT_AVAIL", event);

	if (avail) {
		const float distance = Math::LengthSquared(destination - newTarget);
		if (closest < 0 || distance <= closest) {
			closest = distance;
			SetDirection(other);
			target = newTarget;
		}
	}

	newTarget = position - other;
	if (closest < 0) {
		SetDirection(-other);
		target = newTarget;
		return;
	}	

	avail = false;
	check.Set(static_cast<int>(newTarget.x), static_cast<int>(newTarget.y));
	event = new Events::MapData(&avail, check);
	em->Trigger("SPOT_AVAIL", event);

	if (avail) {
		const float distance = Math::LengthSquared(destination - newTarget);
		if (distance <= closest) {
			SetDirection(-other);
			target = newTarget;
		}
	}
}

void AISprite::Move(const float& dt) {
	auto& position = GetComponent<Transform>()->translation;
	vec3f dir = target - position;

	if (Math::Length(dir) <= speed * dt) {
		position = target;
		SetNewTarget();
	} else {
		Math::Normalize(dir);
		position += dir * speed * static_cast<float>(dt);
	}
}

void AISprite::Update(const float& dt) {
	Move(dt);
}
