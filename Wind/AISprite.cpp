#include "AISprite.h"

#include "Transform.h"
#include "Render.h"
#include "Collider.h"
#include "Script.h"
#include "StateContainer.h"

#include <Math/Math.hpp>
#include <Math/Random.hpp>
#include <Events/EventsManager.h>

AISprite::AISprite() {
	interest = nullptr;
	direction.Set(1.f, 0.f, 0.f);

	Events::EventsManager::GetInstance()->Subscribe("AI_STATE_CHANGE", &AISprite::EventHandler, this);
}

void AISprite::Build() {
	AddComponent<Transform>();
	AddComponent<Render>();
	AddComponent<Collider>();
	AddComponent<Script>();
	AddComponent<StateContainer>();
}

void AISprite::Initialize() {
	Entity::Initialize();

	speed = 1.f;

	GetComponent<Collider>()->BindCollisionEnter(&AISprite::OnCollisionEnter, this);
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

void AISprite::Move(const float& dt) {
	auto& position = GetComponent<Transform>()->translation;
	vec3f dir = target - position;

	if (Math::Length(dir) <= speed * dt) {
		position = target;
		float random = Math::RandValue();

		if (interest) {
			const vec3f dir = Math::Normalized(GetDestination() - position);
			const vec3f fDir = Math::Abs(dir);

			if (Math::LengthSquared(fDir) > 0) {
				if (random < fDir.x)
					target.x += dir.x / fDir.x;
				else
					target.y += dir.y / fDir.y;

				target.x = Math::Clamp(target.x, -9.5f, 9.5f);
				target.y = Math::Clamp(target.y, -9.5f, 9.5f);

				return;
			}
		}

		if (random < 0.25f)
			target.x += 1.f;
		else if (random < 0.5f)
			target.x -= 1.f;
		else if (random < 0.75f)
			target.y += 1.f;
		else
			target.y -= 1.f;

		target.x = Math::Clamp(target.x, -9.5f, 9.5f);
		target.y = Math::Clamp(target.y, -9.5f, 9.5f);
	} else {
		Math::Normalize(dir);
		position += dir * speed * static_cast<float>(dt);
	}
}

void AISprite::Update(const float& dt) {
	Move(dt);
}

void AISprite::OnCollisionEnter(Entity * const target) {
	if (GetTag() == "FISH" && target->GetTag() == "FOOD") {
		if (target->IsUsed()) {
			target->Destroy();
			energy += 5.f;
		}
	}
}

void AISprite::EventHandler(Events::Event* event) {
	//GetComponent<StateMachine>()->queuedState = new States::StopState;
}
