#include "StateMachine.h"

#include <Events/EventsManager.h>

StateMachine::StateMachine() 
	: queuedState(nullptr)
	, currentState(nullptr) {}

void StateMachine::Initialize() {
	Component::Initialize();

	queuedState = nullptr;
	currentState = nullptr;
}

void StateMachine::SetActive(const bool& state) {
	Component::SetActive(state);
	Events::EventsManager::GetInstance()->Trigger("STATE_MACHINE_ACTIVE", new Events::AnyType<StateMachine*>(this));
}
