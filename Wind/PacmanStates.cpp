#include "PacmanStates.h"

#include "AISprite.h"
#include "Transform.h"
#include "StateContainer.h"
#include "Animation.h"

#include "Line.h"
#include "EntityEvents.h"

#include <Events/EventsManager.h>

void States::Hungry::Enter(Entity * const target) {
	SetNearestPallet(target);
}

void States::Hungry::Update(Entity * const target, const float & dt) {
	const vec3f position = target->GetComponent<Transform>()->GetWorldTranslation();
	auto pacman = static_cast<AISprite*  const>(target);
	
	if (pacman->GetInterest()) {
		Line line;
		line.tint.Set(0.f, 1.f, 0.f, 1.f);
		line.Set(position, pacman->GetDestination());
		Events::EventsManager::GetInstance()->Trigger("DRAW_LINE", new Events::AnyType<Line>(line));
	}
}

void States::Hungry::FixedUpdate(Entity * const target, const float & dt) {
	auto pacman = static_cast<AISprite*  const>(target);

	auto interest = pacman->GetInterest();

	if (!interest || !interest->IsUsed()) {
		SetNearestPallet(target);
		interest = pacman->GetInterest();
	}

	if (interest)
		pacman->SetDestination(interest->GetComponent<Transform>()->GetWorldTranslation());

	auto ghost = GetNearestGhost(target);
	const auto& ghostPosition = ghost->GetComponent<Transform>()->GetWorldTranslation();
	const auto& pacmanPosition = pacman->GetComponent<Transform>()->GetWorldTranslation();
	const auto& direction = ghostPosition - pacmanPosition;

	if (Math::LengthSquared(direction) < 25.f) {
		pacman->GetComponent<StateContainer>()->queuedState = "PACMAN_AVOID";
		pacman->InvertDirection();
	}
}

void States::Hungry::Exit(Entity * const target) {
}

void States::Hungry::SetNearestPallet(Entity * const target) {
	const vec3f position = target->GetComponent<Transform>()->GetWorldTranslation();
	auto pacman = static_cast<AISprite* const>(target);
	Entity* nearest = nullptr;
	Events::EventsManager::GetInstance()->Trigger("NEAREST_ENTITY_WITH_TAG", new Events::NearestEntityWithTag(&nearest, "PALLET", position));
	pacman->SetInterest(nearest);
}


Entity * States::Hungry::GetNearestGhost(Entity * const target) {
	const vec3f position = target->GetComponent<Transform>()->GetWorldTranslation();
	auto pacman = static_cast<AISprite* const>(target);

	Entity* nearest = nullptr;
	Events::EventsManager::GetInstance()->Trigger("NEAREST_ENTITY_WITH_TAG", new Events::NearestEntityWithTag(&nearest, "GHOST", position));
	return nearest;
}



// Search

void States::Search::Enter(Entity* const target) {
	SetNearestPower(target);

	auto pacman = static_cast<AISprite* const>(target);
	auto interest = pacman->GetInterest();
	if (interest) {
		pacman->SetDestination(interest->GetComponent<Transform>()->GetWorldTranslation());
	} else {
		pacman->GetComponent<StateContainer>()->queuedState = "PACMAN_HUNGRY";
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

void States::Search::SetNearestPower(Entity * const target) {
	const vec3f position = target->GetComponent<Transform>()->GetWorldTranslation();
	auto pacman = static_cast<AISprite* const>(target);
	Entity* nearest = nullptr;
	Events::EventsManager::GetInstance()->Trigger("NEAREST_ENTITY_WITH_TAG", new Events::NearestEntityWithTag(&nearest, "POWER", position));
	pacman->SetInterest(nearest);
}



// Avoid

void States::Avoid::Enter(Entity * const target) {
	SetGhostInRange(target, 25.f);
}

void States::Avoid::Update(Entity * const target, const float & dt) {
	auto pacman = static_cast<AISprite*  const>(target);
	auto interest = pacman->GetInterest();

	if (interest) {
		const vec3f position = target->GetComponent<Transform>()->GetWorldTranslation();
		const vec3f interestPosition = interest->GetComponent<Transform>()->GetWorldTranslation();

		Line line;
		line.tint.Set(1.f, 1.f, 1.f, 0.25f);
		line.Set(position, pacman->GetDestination());
		Events::EventsManager::GetInstance()->Trigger("DRAW_LINE", new Events::AnyType<Line>(line));

		line.tint.Set(1.f, 0.f, 0.f, 1.f);
		line.Set(position, interestPosition);
		Events::EventsManager::GetInstance()->Trigger("DRAW_LINE", new Events::AnyType<Line>(line));
	}
}

void States::Avoid::FixedUpdate(Entity * const target, const float & dt) {
	SetGhostInRange(target, 50.f);
}

void States::Avoid::Exit(Entity * const target) {

}

void States::Avoid::SetGhostInRange(Entity * const target, const float & range) {
	const vec3f position = target->GetComponent<Transform>()->GetWorldTranslation();
	auto pacman = static_cast<AISprite* const>(target);

	Entity* nearest = nullptr;
	Events::EventsManager::GetInstance()->Trigger("NEAREST_ENTITY_WITH_TAG", new Events::NearestEntityWithTag(&nearest, "GHOST", position));
	
	if (nearest) {
		const auto nearestPosition = nearest->GetComponent<Transform>()->GetWorldTranslation();
		const auto diff = nearestPosition - position;
		const auto distance = Math::LengthSquared(diff);
		if (distance <= range) {
			pacman->SetInterest(nearest);
			pacman->SetDestination(position - diff);
		} else {
			pacman->GetComponent<StateContainer>()->queuedState = "PACMAN_HUNGRY";
		}
	}
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

void States::Dead::Enter(Entity * const target) {
	auto pacman = static_cast<AISprite*  const>(target);
	pacman->SetSpeed(0.f);
	target->GetComponent<Animation>()->queued = "DEAD";
}

void States::Dead::Update(Entity * const target, const float & dt) {
}

void States::Dead::FixedUpdate(Entity * const target, const float & dt) {
}

void States::Dead::Exit(Entity * const target) {
}

