#include "GhostChaseStates.h"

// objects
#include "AISprite.h"
#include "Ghost.h"
// components
#include "Transform.h"
#include "StateContainer.h"
// others
#include "Line.h"
#include "EntityEvents.h"

#include <Events/EventsManager.h>



// Blinky 

void States::BlinkyChase::Enter(Entity* const target) {
	auto aiSprite = static_cast<AISprite*  const>(target);

	Entity* pacman = nullptr;
	Events::EventsManager::GetInstance()->Trigger("FIRST_ENTITY_WITH_TAG", new Events::FindEntityWithTag(&pacman, "PACMAN")); 
	aiSprite->SetInterest(pacman);
}

void States::BlinkyChase::Update(Entity* const target, const float& dt) {
	const vec3f position = target->GetComponent<Transform>()->GetWorldTranslation();
	auto aiSprite = static_cast<AISprite*  const>(target);
	
	if (aiSprite->GetInterest()) {
		Line line;
		line.tint.Set(1.f, 0.f, 0.f, 0.5f);
		line.Set(position, aiSprite->GetDestination());
		Events::EventsManager::GetInstance()->Trigger("DRAW_LINE", new Events::AnyType<Line>(line));
	}
}

void States::BlinkyChase::FixedUpdate(Entity* const target, const float& dt) {
	auto aiSprite = static_cast<AISprite*  const>(target);
	auto interest = aiSprite->GetInterest();

	if (interest)
		aiSprite->SetDestination(interest->GetComponent<Transform>()->GetWorldTranslation());
}

void States::BlinkyChase::Exit(Entity* const target) {
	auto ghost = static_cast<Ghost* const>(target);
	ghost->InvertDirection();
}



// Pinky 

void States::PinkyChase::Enter(Entity* const target) {
	auto aiSprite = static_cast<AISprite*  const>(target);

	Entity* pacman = nullptr;
	Events::EventsManager::GetInstance()->Trigger("FIRST_ENTITY_WITH_TAG", new Events::FindEntityWithTag(&pacman, "PACMAN")); 
	aiSprite->SetInterest(pacman);
}

void States::PinkyChase::Update(Entity* const target, const float& dt) {
	const vec3f position = target->GetComponent<Transform>()->GetWorldTranslation();
	auto aiSprite = static_cast<AISprite*  const>(target);

	if (aiSprite->GetInterest()) {
		const vec3f& destination = aiSprite->GetDestination();

		Line line;
		line.tint.Set(1.f, 0.72f, 1.f, 0.5f);
		line.Set(position, destination);
		Events::EventsManager::GetInstance()->Trigger("DRAW_LINE", new Events::AnyType<Line>(line));

		auto interest = static_cast<AISprite*  const>(aiSprite->GetInterest());
		const vec3f& direction = interest->GetDirection();
		if (interest) {
			const vec3f pacmanPosition = interest->GetComponent<Transform>()->GetWorldTranslation();
			line.tint.Set(1.f, 1.f, 1.f, 0.25f);
			line.Set(pacmanPosition, destination);
			Events::EventsManager::GetInstance()->Trigger("DRAW_LINE", new Events::AnyType<Line>(line));
		}
	}
}

void States::PinkyChase::FixedUpdate(Entity* const target, const float& dt) {
	auto aiSprite = static_cast<AISprite*  const>(target);
	auto interest = static_cast<AISprite*  const>(aiSprite->GetInterest());

	if (interest) {
		const vec3f& direction = interest->GetDirection();
		if (direction == vec3f(0.f, 1.f, 0.f)) {
			aiSprite->SetDestination(
				interest->GetComponent<Transform>()->GetWorldTranslation() + vec3f(-2.f, 2.f, 0.f)
			);
		} else {
			aiSprite->SetDestination(
				interest->GetComponent<Transform>()->GetWorldTranslation() + direction * 4.f
			);
		}
	}
}

void States::PinkyChase::Exit(Entity* const target) {
	auto ghost = static_cast<Ghost* const>(target);
	ghost->InvertDirection();
}



// Clyde 

void States::ClydeChase::Enter(Entity* const target) {
	auto aiSprite = static_cast<AISprite*  const>(target);

	Entity* pacman = nullptr;
	Events::EventsManager::GetInstance()->Trigger("FIRST_ENTITY_WITH_TAG", new Events::FindEntityWithTag(&pacman, "PACMAN")); 
	aiSprite->SetInterest(pacman);
}

void States::ClydeChase::Update(Entity* const target, const float& dt) {
	const vec3f position = target->GetComponent<Transform>()->GetWorldTranslation();
	auto aiSprite = static_cast<AISprite*  const>(target);
	
	if (aiSprite->GetInterest()) {
		Line line;
		line.tint.Set(1.f, 0.72f, 0.32f, 0.5f);
		line.Set(position, aiSprite->GetDestination());
		Events::EventsManager::GetInstance()->Trigger("DRAW_LINE", new Events::AnyType<Line>(line));
	}
}

void States::ClydeChase::FixedUpdate(Entity* const target, const float& dt) {
	auto aiSprite = static_cast<AISprite*  const>(target);
	auto interest = aiSprite->GetInterest();

	if (interest) {
		const vec3f target = interest->GetComponent<Transform>()->GetWorldTranslation();
		const vec3f diff = aiSprite->GetComponent<Transform>()->GetWorldTranslation() - target;
		if (Math::LengthSquared(diff) >= 64.f)
			aiSprite->SetDestination(target);
		else
			aiSprite->GetComponent<StateContainer>()->queuedState = "CLYDE_INVERSE_CHASE_STATE";
	}
}

void States::ClydeChase::Exit(Entity* const target) {
	auto ghost = static_cast<Ghost* const>(target);
	ghost->InvertDirection();
}



// Clyde (inverse)

void States::ClydeInverseChase::Enter(Entity* const target) {
	auto ghost = static_cast<Ghost*  const>(target);

	Entity* pacman = nullptr;
	Events::EventsManager::GetInstance()->Trigger("FIRST_ENTITY_WITH_TAG", new Events::FindEntityWithTag(&pacman, "PACMAN")); 
	ghost->SetInterest(pacman);

	ghost->SetDestination(ghost->GetDock());
}

void States::ClydeInverseChase::Update(Entity* const target, const float& dt) {
	const vec3f position = target->GetComponent<Transform>()->GetWorldTranslation();
	auto aiSprite = static_cast<AISprite*  const>(target);
	
	if (aiSprite->GetInterest()) {
		Line line;
		line.tint.Set(1.f, 0.72f, 0.32f, 0.5f);
		line.Set(position, aiSprite->GetDestination());
		Events::EventsManager::GetInstance()->Trigger("DRAW_LINE", new Events::AnyType<Line>(line));
	}
}

void States::ClydeInverseChase::FixedUpdate(Entity* const target, const float& dt) {
	auto ghost = static_cast<Ghost*  const>(target);
	auto interest = ghost->GetInterest();

	if (interest) {
		const vec3f target = interest->GetComponent<Transform>()->GetWorldTranslation();
		const vec3f diff = ghost->GetComponent<Transform>()->GetWorldTranslation() - target;
		if (Math::LengthSquared(diff) > 64.f)
			ghost->GetComponent<StateContainer>()->queuedState = "CLYDE_CHASE_STATE";
	}
}

void States::ClydeInverseChase::Exit(Entity* const target) {
	auto ghost = static_cast<Ghost* const>(target);
	ghost->InvertDirection();
}



// Inky 

void States::InkyChase::Enter(Entity* const target) {
	auto aiSprite = static_cast<AISprite*  const>(target);

	Entity* pacman = nullptr;
	Events::EventsManager::GetInstance()->Trigger("FIRST_ENTITY_WITH_TAG", new Events::FindEntityWithTag(&pacman, "PACMAN")); 
	aiSprite->SetInterest(pacman);
}

void States::InkyChase::Update(Entity* const target, const float& dt) {
	const vec3f position = target->GetComponent<Transform>()->GetWorldTranslation();
	auto ghost = static_cast<Ghost*  const>(target);
	
	if (ghost->GetInterest()) {
		const vec3f& destination = ghost->GetDestination();

		Line line;
		line.tint.Set(1.f, 0.f, 1.f, 0.5f);
		line.Set(position, ghost->GetDestination());
		Events::EventsManager::GetInstance()->Trigger("DRAW_LINE", new Events::AnyType<Line>(line));

		auto interest = static_cast<AISprite*  const>(ghost->GetInterest());

		if (interest) {
			const vec3f& direction = interest->GetDirection();
			const vec3f& partnerPosition = ghost->GetPartner()->GetComponent<Transform>()->GetWorldTranslation();
			const vec3f interestPosition = interest->GetComponent<Transform>()->GetWorldTranslation();
			
			const vec3f front = interestPosition + direction * 2.f;
			const vec3f diff = front - partnerPosition;

			line.tint.Set(1.f, 1.f, 1.f, 0.25f);
			line.Set(interestPosition, front);
			Events::EventsManager::GetInstance()->Trigger("DRAW_LINE", new Events::AnyType<Line>(line));

			line.Set(front, destination);
			Events::EventsManager::GetInstance()->Trigger("DRAW_LINE", new Events::AnyType<Line>(line));

		}
	}
}

void States::InkyChase::FixedUpdate(Entity* const target, const float& dt) {
	auto ghost = static_cast<Ghost*  const>(target);
	auto interest = static_cast<AISprite*  const>(ghost->GetInterest());

	if (interest) {
		const vec3f& direction = interest->GetDirection();
		const vec3f& partnerPosition = ghost->GetPartner()->GetComponent<Transform>()->GetWorldTranslation();
		const vec3f& interestPosition = interest->GetComponent<Transform>()->GetWorldTranslation();

		const vec3f front = interestPosition + direction * 2.f;
		const vec3f diff = front - partnerPosition;

		ghost->SetDestination(diff + front);
	}
}

void States::InkyChase::Exit(Entity* const target) {
	auto ghost = static_cast<Ghost* const>(target);
	ghost->InvertDirection();
}
