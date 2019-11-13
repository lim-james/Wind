#include "PacmanStates.h"

#include "AISprite.h"
#include "Transform.h"
#include "StateContainer.h"

#include "Line.h"
#include "EntityEvents.h"

#include <Events/EventsManager.h>

void States::Search::Enter(Entity* const target) {
	const vec3f position = target->GetComponent<Transform>()->GetWorldTranslation();
	auto pacman = static_cast<AISprite* const>(target);

	Entity* nearest = nullptr;
	Events::EventsManager::GetInstance()->Trigger("NEAREST_ENTITY_WITH_TAG", new Events::NearestEntityWithTag(&nearest, "POWER", position));
	if (nearest) {
		pacman->SetInterest(nearest);
		pacman->SetDestination(nearest->GetComponent<Transform>()->GetWorldTranslation());
	} else {
		pacman->GetComponent<StateContainer>()->queuedState = "PACMAN_AVOID";
	}
}

void States::Search::Update(Entity* const target, const float& dt) {
	auto pacman = static_cast<AISprite*  const>(target);
	auto interest = pacman->GetInterest();

	if (interest) {
		const vec3f position = target->GetComponent<Transform>()->GetWorldTranslation();

		Line line;
		line.tint.Set(0.f, 1.f, 0.f, 1.f);
		line.Set(position, pacman->GetDestination());
		Events::EventsManager::GetInstance()->Trigger("DRAW_LINE", new Events::AnyType<Line>(line));
	}
}

void States::Search::FixedUpdate(Entity* const target, const float& dt) {

}

void States::Search::Exit(Entity* const target) {

}



// Hunt

void States::Hunt::Enter(Entity* const target) {
	SetNearestGhost(target);
}

void States::Hunt::Update(Entity* const target, const float& dt) {
	auto pacman = static_cast<AISprite*  const>(target);
	auto interest = pacman->GetInterest();

	if (interest) {
		const vec3f position = target->GetComponent<Transform>()->GetWorldTranslation();
		const vec3f& destination = pacman->GetDestination();
		const vec3f interestPosition = interest->GetComponent<Transform>()->GetWorldTranslation();

		Line line;
		line.tint.Set(1.f, 1.f, 1.f, 0.25f);
		line.Set(interestPosition, destination);
		Events::EventsManager::GetInstance()->Trigger("DRAW_LINE", new Events::AnyType<Line>(line));

		line.tint.Set(0.f, 1.f, 0.f, 1.f);
		line.Set(position, destination);
		Events::EventsManager::GetInstance()->Trigger("DRAW_LINE", new Events::AnyType<Line>(line));
	}
}

void States::Hunt::FixedUpdate(Entity* const target, const float& dt) {
	auto pacman = static_cast<AISprite*  const>(target);

	auto interest = pacman->GetInterest();

	if (interest) {
		const auto& interestState = interest->GetComponent<StateContainer>()->currentState;

		if (interestState != "GHOST_FRIGHTENED") {
			SetNearestGhost(target);
			interest = pacman->GetInterest();
		}

		if (interest) {
			const vec3f position = interest->GetComponent<Transform>()->GetWorldTranslation();
			auto ghost = static_cast<AISprite*>(interest);
			pacman->SetDestination(position + ghost->GetDirection() * 2.f);
		} else {
			pacman->GetComponent<StateContainer>()->queuedState = "PACMAN_SEARCH";
		}
	}

}

void States::Hunt::Exit(Entity* const target) {

}

void States::Hunt::SetNearestGhost(Entity * const target) {
	const vec3f position = target->GetComponent<Transform>()->GetWorldTranslation();
	auto pacman = static_cast<AISprite* const>(target);

	Entity* nearest = nullptr;
	Events::EventsManager::GetInstance()->Trigger("NEAREST_ENTITY_WITH_TAG", new Events::NearestEntityWithTag(&nearest, "GHOST", position, "GHOST_FRIGHTENED"));
	pacman->SetInterest(nearest);
}
