#include "StateSystem.h"

#include <Events/EventsManager.h>
#include <MACROS.h>

StateSystem::StateSystem() {
	Events::EventsManager::GetInstance()->Subscribe("STATE_MACHINE_ACTIVE", &StateSystem::ActiveHandler, this);
}

void StateSystem::Update(const float& dt) {}

void StateSystem::FixedUpdate(const float& dt) {
	for (auto& c : components) {
		auto entity = c->GetParent();

		if (c->queuedState) {
			if (c->currentState)
				c->currentState->Exit(entity);
			c->currentState = c->queuedState;
			c->queuedState = nullptr;
			c->currentState->Enter(entity);
		}

		if (c->currentState)
			c->currentState->Update(entity, dt);
	}
}

void StateSystem::ActiveHandler(Events::Event* event) {
	const auto c = static_cast<Events::AnyType<StateMachine*>*>(event)->data;

	if (c->IsActive())
		components.push_back(c);
	else
		components.erase(vfind(components, c));

}
