#include "StateMachine.h"

#include <Events/EventsManager.h>
#include <MACROS.h>

StateMachine::StateMachine() {
	Events::EventsManager::GetInstance()->Subscribe("STATE_CONTAINER_ACTIVE", &StateMachine::ActiveHandler, this);
}

StateMachine::~StateMachine() {
	for (auto& state : states)
		delete state.second;

	states.clear();
}

void StateMachine::Update(const float& dt) {
	for (auto& c : components) {
		auto entity = c->GetParent();
		if (c->currentState != "")
			states[c->currentState]->Update(entity, dt);
	}
}

void StateMachine::FixedUpdate(const float& dt) {
	for (auto& c : components) {
		auto entity = c->GetParent();

		if (c->queuedState != "") {
			if (c->currentState != "")
				states[c->currentState]->Exit(entity);
			c->currentState = c->queuedState;
			c->queuedState = "";
			states[c->currentState]->Enter(entity);
		}

		if (c->currentState != "")
			states[c->currentState]->FixedUpdate(entity, dt);
	}
}

void StateMachine::ActiveHandler(Events::Event* event) {
	const auto c = static_cast<Events::AnyType<StateContainer*>*>(event)->data;

	if (c->IsActive())
		components.push_back(c);
	else
		components.erase(vfind(components, c));

}
