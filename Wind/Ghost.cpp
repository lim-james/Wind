#include "Ghost.h"

#include "Transform.h"
#include "Animation.h"
#include "Collider.h"
#include "StateContainer.h"

#include "Line.h"
#include "GameModes.h"

#include <Events/EventsManager.h>

Ghost::Ghost() {
	direction.Set(0.f, 1.f, 0.f);
}

void Ghost::Initialize() {
	AISprite::Initialize();

	GetComponent<Collider>()->BindCollisionEnter(&Ghost::OnCollisionEnter, this);

	Events::EventsManager::GetInstance()->Subscribe("GAME_MODE", &Ghost::StateHandler, this);
}

void Ghost::InvertDirection() {
	SetDirection(-direction);
}

const vec3f& Ghost::GetDock() const {
	return dock;
}

void Ghost::SetDock(const vec3f& position) {
	dock = position;
}

const vec3f& Ghost::GetCorner() const {
	return corner;
}

void Ghost::SetCorner(const vec3f& position) {
	corner = position;
}

const std::string& Ghost::GetChaseState() const {
	return chaseState;
}

void Ghost::SetChaseState(const std::string& state) {
	chaseState = state;
}

Ghost* const Ghost::GetPartner() const {
	return partner;
}

void Ghost::SetPartner(Ghost* const ghost) {
	partner = ghost;
}

void Ghost::SetDirection(const vec3f& _direction) {
	AISprite::SetDirection(_direction);

	const auto& state = GetComponent<StateContainer>()->currentState;
	if (state == "GHOST_FRIGHTENED") return;

	auto animation = GetComponent<Animation>();

	if (state == "GHOST_EATEN") {
		if (direction.x) {
			if (direction.x > 0) {
				animation->queued = "EATEN_RIGHT";
			} else {
				animation->queued = "EATEN_LEFT";
			}
		} else {
			if (direction.y > 0) {
				animation->queued = "EATEN_UP";
			} else {
				animation->queued = "EATEN_DOWN";
			}
		}
	} else {
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

}

void Ghost::StateHandler(Events::Event* event) {
	auto mode = static_cast<Events::ModeEvent*>(event)->data;

	if (mode == CHASE)
		GetComponent<StateContainer>()->queuedState = chaseState;
	else if (mode == SCATTER)
		GetComponent<StateContainer>()->queuedState = "GHOST_SCATTER";
	else if (mode == FRIGHTENED)
		GetComponent<StateContainer>()->queuedState = "GHOST_FRIGHTENED";
	else if (mode == END_FRIGHTENED)
		GetComponent<Animation>()->queued = "END_FRIGHTENED";
}

void Ghost::OnCollisionEnter(Entity * const target) {
	if (target->GetTag() == "PACMAN") {
		auto state = GetComponent<StateContainer>();
		if (state->currentState == "GHOST_FRIGHTENED") {
			state->queuedState = "GHOST_EATEN";
		}
	}
}

void Ghost::SetEnterTarget() {
	const auto& position = GetComponent<Transform>()->translation;

	const vec3f dir = Math::Normalized(GetDestination() - position);
	const vec3f fDir = Math::Abs(dir);

	if (Math::LengthSquared(fDir) > 0) {
		if (fDir.y < fDir.x)
			target.x += dir.x / fDir.x;
		else
			target.y += dir.y / fDir.y;
	}
}

void Ghost::Move(const float& dt) {
	auto& position = GetComponent<Transform>()->translation;
	vec3f dir = target - position;

	if (Math::Length(dir) <= speed * dt) {
		position = target;
		if (GetComponent<StateContainer>()->currentState == "GHOST_ENTERING")
			SetEnterTarget();
		else
			SetNewTarget();
	} else {
		Math::Normalize(dir);
		position += dir * speed * static_cast<float>(dt);
	}
}

