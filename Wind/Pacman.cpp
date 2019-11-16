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

