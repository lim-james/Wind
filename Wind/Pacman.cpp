#include "Pacman.h"

#include "Transform.h"
#include "Animation.h"
#include "Collider.h"
#include "Script.h"
#include "StateContainer.h"
#include "MapEvents.h"
#include "GameModes.h"

#include <Events/EventsManager.h>

void Pacman::Build() {
	AISprite::Build();

	AddComponent<Script>();
}

void Pacman::Initialize() {
	AISprite::Initialize();

	GetComponent<Collider>()->BindCollisionEnter(&Pacman::OnCollisionEnter, this);

	GetComponent<Script>()->fixedUpdate = std::bind(&Pacman::FixedUpdate, this, std::placeholders::_1);
}

void Pacman::SetDirection(const vec3f& _direction) {
	AISprite::SetDirection(_direction);

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

void Pacman::SetNewTarget() {
	auto em = Events::EventsManager::GetInstance();

	const auto& position = GetComponent<Transform>()->translation;
	
	vec3f newTarget = position + direction;

	bool avail = false;
	vec2i check(static_cast<int>(newTarget.x), static_cast<int>(newTarget.y));
	Events::MapData* event = new Events::MapData(&avail, check);
	em->Trigger("SPOT_AVAIL",  event);

	float closest = -1.f;

	if (avail) {
		closest = Math::LengthSquared(destination - newTarget);
		target = newTarget;
	}	

	// checkbehind
	if (GetComponent<StateContainer>()->currentState == "PACMAN_HUNT") {
		newTarget = position - direction;

		avail = false;
		check.Set(static_cast<int>(newTarget.x), static_cast<int>(newTarget.y));
		event = new Events::MapData(&avail, check);
		em->Trigger("SPOT_AVAIL", event);

		if (avail) {
			const float distance = Math::LengthSquared(destination - newTarget);
			if (closest < 0 || distance <= closest) {
				closest = distance;
				SetDirection(-direction);
				target = newTarget;
			}
		}
	}

	// check on other axis

	vec3f other(direction.y, direction.x, 0.f);

	newTarget = position + other;

	avail = false;
	check.Set(static_cast<int>(newTarget.x), static_cast<int>(newTarget.y));
	event = new Events::MapData(&avail, check);
	em->Trigger("SPOT_AVAIL",  event);

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
	em->Trigger("SPOT_AVAIL",  event);

	if (avail) {
		const float distance = Math::LengthSquared(destination - newTarget);
		if (distance <= closest) {
			SetDirection(-other);
			target = newTarget;
		}
	}

}

void Pacman::OnCollisionEnter(Entity * const target) {
	if (target->GetTag() == "POWER") {
		Events::EventsManager::GetInstance()->Trigger("GAME_MODE", new Events::ModeEvent(FRIGHTENED));
		target->Destroy();
		GetComponent<StateContainer>()->queuedState = "PACMAN_HUNT";
	}
}

void Pacman::FixedUpdate(const float& dt) {
	const auto& position = GetComponent<Transform>()->GetWorldTranslation();
	bool pickup = false;
	const vec2i check(
		static_cast<int>(round(position.x)), 
		static_cast<int>(round(position.y))
	);

	Events::EventsManager::GetInstance()->Trigger("TAKE_PALLET",
		new Events::MapData(&pickup, check)
	);
}

