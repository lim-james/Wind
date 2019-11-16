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

void Ghost::SetNewTarget()
{
	if (GetComponent<StateContainer>()->currentState == "GHOST_ENTERING")
		SetEnterTarget();
	else
		AISprite::SetNewTarget();
}

void Ghost::StateHandler(Events::Event* event) {
	auto mode = static_cast<Events::ModeEvent*>(event)->data;

	auto state = GetComponent<StateContainer>();

	if (mode == CHASE)
		state->queuedState = chaseState;
	else if (mode == SCATTER)
		state->queuedState = "GHOST_SCATTER";
	else if (mode == FRIGHTENED)
		state->queuedState = "GHOST_FRIGHTENED";
	else if (mode == END_FRIGHTENED && state->currentState == "GHOST_FRIGHTENED")
		GetComponent<Animation>()->queued = "END_FRIGHTENED";
}

void Ghost::OnCollisionEnter(Entity * const target) {
	if (target->GetTag() == "PACMAN") {
		auto selfState = GetComponent<StateContainer>();
		auto targetState = target->GetComponent<StateContainer>();
		if (selfState->currentState == "GHOST_FRIGHTENED") {
			selfState->queuedState = "GHOST_EATEN";
		} else if (selfState->currentState != "GHOST_EATEN" && targetState->currentState != "PACMAN_DEAD") {
			targetState->queuedState = "PACMAN_DEAD";
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
