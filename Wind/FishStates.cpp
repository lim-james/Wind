#include "FishStates.h"

#include "AISprite.h"
#include "Transform.h"
#include "Render.h"
#include "StateMachine.h"
#include "Line.h"
#include "EntityEvents.h"

#include <Events/EventsManager.h>


// Fish too full


void States::FishTooFull::Enter(Entity* const target) {
	auto aiSprite = static_cast<AISprite*  const>(target);
	aiSprite->SetSpeed(0.f);
}

void States::FishTooFull::Update(Entity* const target, const float& dt) {}
	
void States::FishTooFull::FixedUpdate(Entity* const target, const float& dt) {
	auto aiSprite = static_cast<AISprite*  const>(target);
	aiSprite->energy -= dt;
	target->GetComponent<Render>()->tint.g = aiSprite->energy / 10.f;

	if (aiSprite->energy < 10)
		target->GetComponent<StateMachine>()->queuedState = new FishFull;
}

void States::FishTooFull::Exit(Entity* const target) {}


// Fish Full


void States::FishFull::Enter(Entity* const target) {
	auto aiSprite = static_cast<AISprite*  const>(target);
	aiSprite->SetSpeed(4.f);
	SetShark(target);
}

void States::FishFull::Update(Entity* const target, const float& dt) {
	const vec3f position = target->GetComponent<Transform>()->GetWorldTranslation();
	auto aiSprite = static_cast<AISprite*  const>(target);
	
	auto interest = aiSprite->GetInterest();

	if (interest) {
		const vec3f avoid = interest->GetComponent<Transform>()->GetWorldTranslation();
		
		Line line;
		line.tint.Set(1.f, 0.f, 0.f, 1.f);
		line.Set(position, avoid);
		Events::EventsManager::GetInstance()->Trigger("DRAW_LINE", new Events::AnyType<Line>(line));
	}
}
	
void States::FishFull::FixedUpdate(Entity* const target, const float& dt) {
	auto aiSprite = static_cast<AISprite*  const>(target);
	aiSprite->energy -= dt;
	target->GetComponent<Render>()->tint.g = aiSprite->energy / 10.f;

	if (aiSprite->energy < 5.f) {
		target->GetComponent<StateMachine>()->queuedState = new FishHungry;
		return;
	} else if (aiSprite->energy >= 10.f) {
		target->GetComponent<StateMachine>()->queuedState = new FishTooFull;
		return;
	}

	auto interest = aiSprite->GetInterest();

	if (!interest || !interest->IsUsed()) {
		SetShark(target);
		interest = aiSprite->GetInterest();
	}

	const vec3f avoid = interest->GetComponent<Transform>()->GetWorldTranslation();
	const vec3f position = target->GetComponent<Transform>()->GetWorldTranslation();
	const vec3f dir = position - avoid;
	if (Math::LengthSquared(dir) < 25.f) {
		aiSprite->SetDestination(position + dir);
	} else {
		aiSprite->SetInterest(nullptr);
	}
}

void States::FishFull::Exit(Entity* const target) {
	auto aiSprite = static_cast<AISprite*  const>(target);
	aiSprite->SetInterest(nullptr);
}

void States::FishFull::SetShark(Entity * const target) {
	auto aiSprite = static_cast<AISprite*  const>(target);

	Entity* shark = nullptr;
	Events::EventsManager::GetInstance()->Trigger("FIRST_ENTITY_WITH_TAG", new Events::FindEntityWithTag(&shark, "SHARK")); 
	aiSprite->SetInterest(shark);
}


// Fish Hungry


void States::FishHungry::Enter(Entity* const target) {
	auto aiSprite = static_cast<AISprite*  const>(target);
	aiSprite->SetSpeed(2.f);
	SetNearestFood(target);
}

void States::FishHungry::Update(Entity* const target, const float& dt) {
	const vec3f position = target->GetComponent<Transform>()->GetWorldTranslation();
	auto aiSprite = static_cast<AISprite*  const>(target);
	
	if (aiSprite->GetInterest()) {
		Line line;
		line.tint.Set(0.f, 1.f, 0.f, 1.f);
		line.Set(position, aiSprite->GetDestination());
		Events::EventsManager::GetInstance()->Trigger("DRAW_LINE", new Events::AnyType<Line>(line));
	}
}
	
void States::FishHungry::FixedUpdate(Entity* const target, const float& dt) {
	auto aiSprite = static_cast<AISprite*  const>(target);

	aiSprite->energy -= dt;
	target->GetComponent<Render>()->tint.g = aiSprite->energy / 10.f;

	if (aiSprite->energy < 0.f) {
		target->GetComponent<StateMachine>()->queuedState = new FishDead;
		return;
	} else if (aiSprite->energy >= 5.f) {
		target->GetComponent<StateMachine>()->queuedState = new FishFull;
		return;
	}

	auto interest = aiSprite->GetInterest();

	if (!interest || !interest->IsUsed()) {
		SetNearestFood(target);
		interest = aiSprite->GetInterest();
	}

	if (interest)
		aiSprite->SetDestination(interest->GetComponent<Transform>()->GetWorldTranslation());
}

void States::FishHungry::Exit(Entity* const target) {
	auto aiSprite = static_cast<AISprite*  const>(target);
	aiSprite->SetInterest(nullptr);
}

void States::FishHungry::SetNearestFood(Entity * const target) {
	const vec3f position = target->GetComponent<Transform>()->GetWorldTranslation();
	auto aiSprite = static_cast<AISprite*  const>(target);

	Entity* nearest = nullptr;
	Events::EventsManager::GetInstance()->Trigger("NEAREST_ENTITY_WITH_TAG", new Events::NearestEntityWithTag(&nearest, "FOOD", position)); 
	aiSprite->SetInterest(nearest);
}


// Fish too full


void States::FishDead::Enter(Entity* const target) {
	auto aiSprite = static_cast<AISprite*  const>(target);
	aiSprite->SetSpeed(0.f);
	target->GetComponent<Render>()->tint.g = 0.f;
}

void States::FishDead::Update(Entity* const target, const float& dt) {}
	
void States::FishDead::FixedUpdate(Entity* const target, const float& dt) {
	auto aiSprite = static_cast<AISprite*  const>(target);
	aiSprite->energy -= dt;
	if (aiSprite->energy < -5.f) {
		aiSprite->Destroy();
	}
}

void States::FishDead::Exit(Entity* const target) {}

