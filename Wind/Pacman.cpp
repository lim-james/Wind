#include "Pacman.h"

#include "Transform.h"
#include "Collider.h"
#include "Script.h"
#include "MapEvents.h"

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

void Pacman::OnCollisionEnter(Entity * const target) {
	if (target->GetTag() == "POWER") {
		target->Destroy();
	}
}
