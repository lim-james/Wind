#include "AISprite.h"

#include "Transform.h"
#include "Render.h"
#include "Collider.h"
#include "Script.h"

#include <Math/Math.hpp>
#include <Math/Random.hpp>

void AISprite::Build() {
	AddComponent<Transform>();
	AddComponent<Render>();
	AddComponent<Collider>();
	AddComponent<Script>();
}

void AISprite::Initialize() {
	Entity::Initialize();

	speed = 1.f;

	GetComponent<Script>()->update = std::bind(&AISprite::Update, this, std::placeholders::_1);
	GetComponent<Collider>()->onCollisionEnter = std::bind(&AISprite::OnCollisionEnter, this, std::placeholders::_1);
}

void AISprite::SetTarget(const vec3f& value) {
	target = value;
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
	if (tag != target->tag)
		std::cout << "Collided with " << target->tag << '\n'; 
}
