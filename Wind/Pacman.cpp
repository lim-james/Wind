#include "Pacman.h"

#include "Transform.h"
#include "Animation.h"
#include "Collider.h"
#include "Script.h"
#include "ParticleEmitter.h"
#include "StateContainer.h"
#include "MapEvents.h"
#include "GameModes.h"

#include <Events/EventsManager.h>

void Pacman::Build() {
	AISprite::Build();

	AddComponent<Script>();
	AddComponent<ParticleEmitter>();
}

void Pacman::Initialize() {
	AISprite::Initialize();

	GetComponent<Collider>()->BindCollisionEnter(&Pacman::OnCollisionEnter, this);

	GetComponent<Script>()->fixedUpdate = std::bind(&Pacman::FixedUpdate, this, std::placeholders::_1);

	auto emitter = GetComponent<ParticleEmitter>();

	emitter->loop = false;
	emitter->offset.z = -1.f;
	emitter->duration = 0.5f;
	emitter->spawnInterval = 0.01f;
	emitter->lifetime = 1.f;
	emitter->angleRange.z = 180.f;
	emitter->speed = 15.f;
	emitter->accelRad = -50.f;
	emitter->accelRadRange = 10.f;
	emitter->startSize.Set(0.5f);
	emitter->endSize.Set(0.f);
	emitter->startColor.Set(1.f, 1.f, 0.f, 1.f);
	emitter->endColor.Set(1.f, 1.f, 0.f, 0.f);
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
	if (target->GetTag() == "POWER" && target->IsUsed()) {
		Events::EventsManager::GetInstance()->Trigger("GAME_MODE", new Events::ModeEvent(FRIGHTENED));
		target->Destroy();
		GetComponent<StateContainer>()->queuedState = "PACMAN_HUNT";
		GetComponent<ParticleEmitter>()->Play();
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

