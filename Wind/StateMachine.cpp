#include "StateMachine.h"

#include <Events/EventsManager.h>
#include <MACROS.h>

StateMachine::~StateMachine() {
	for (auto& state : states)
		delete state.second;

	states.clear();
}

void StateMachine::Start() {
	Events::EventsManager::GetInstance()->Subscribe("STATE_CONTAINER_ACTIVE", &StateMachine::ActiveHandler, this);
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
			const auto previous = c->currentState;
			c->currentState = c->queuedState;
			if (previous != "")
				states[previous]->Exit(entity);
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
