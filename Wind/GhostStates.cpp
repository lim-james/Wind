#include "GhostStates.h"

#include "Ghost.h"
#include "Transform.h"
#include "StateContainer.h"
#include "Line.h"
#include "MapEvents.h"

#include <Events/EventsManager.h>

// Entering

void States::Entering::Enter(Entity* const target) {
	auto ghost = static_cast<Ghost*>(target);
	ghost->SetDestination(vec3f(0.f, 3.f, 0.f));
}

void States::Entering::Update(Entity* const target, const float& dt) {
	const vec3f position = target->GetComponent<Transform>()->GetWorldTranslation();
	auto ghost = static_cast<AISprite* const>(target);

	Line line;
	line.tint.Set(1.f, 1.f, 1.f, 0.1f);
	line.Set(position, ghost->GetDestination());
	Events::EventsManager::GetInstance()->Trigger("DRAW_LINE", new Events::AnyType<Line>(line));

	line.tint.Set(0.f, 1.f, 0.f, 0.1f);
	line.Set(position, ghost->GetTarget());
	Events::EventsManager::GetInstance()->Trigger("DRAW_LINE", new Events::AnyType<Line>(line));
}

void States::Entering::FixedUpdate(Entity* const target, const float& dt) {
	if (target->GetComponent<Transform>()->translation == vec3f(0.f, 3.f, 0.f)) {
		target->GetComponent<StateContainer>()->queuedState = "GHOST_SCATTER";
	}
}

void States::Entering::Exit(Entity* const target) {

}


// Scatter

void States::Scatter::Enter(Entity* const target) {
	auto ghost = static_cast<Ghost*>(target);
	ghost->SetDestination(ghost->GetDock());
}

void States::Scatter::Update(Entity* const target, const float& dt) {
	const vec3f position = target->GetComponent<Transform>()->GetWorldTranslation();
	auto ghost = static_cast<Ghost* const>(target);

	Line line;
	line.tint.Set(1.f, 1.f, 1.f, 0.25f);
	line.Set(position, ghost->GetDock());
	Events::EventsManager::GetInstance()->Trigger("DRAW_LINE", new Events::AnyType<Line>(line));

	line.tint.Set(0.f, 1.f, 0.f, 1.f);
	line.Set(position, ghost->GetTarget());
	Events::EventsManager::GetInstance()->Trigger("DRAW_LINE", new Events::AnyType<Line>(line));
}

void States::Scatter::FixedUpdate(Entity* const target, const float& dt) {
	//auto ghost = static_cast<Ghost* const>(target);
	//const vec3f diff = ghost->GetComponent<Transform>()->translation - ghost->GetCorner();
	//if (Math::LengthSquared(diff) <= 1.f) {
	//	ghost->GetComponent<StateContainer>()->queuedState = ghost->GetChaseState();
	//}
}

void States::Scatter::Exit(Entity* const target) {
	auto ghost = static_cast<Ghost* const>(target);
	ghost->InvertDirection();
}



// Eaten

void States::Eaten::Enter(Entity* const target) {

}

void States::Eaten::Update(Entity* const target, const float& dt) {

}

void States::Eaten::FixedUpdate(Entity* const target, const float& dt) {

}

void States::Eaten::Exit(Entity* const target) {

}



//Frightene

void States::Frightened::Enter(Entity* const target) {

}

void States::Frightened::Update(Entity* const target, const float& dt) {

}

void States::Frightened::FixedUpdate(Entity* const target, const float& dt) {

}

void States::Frightened::Exit(Entity* const target) {

}