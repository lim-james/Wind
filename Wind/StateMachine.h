#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "Component.h"
#include "State.h"

struct StateMachine : Component {

	States::State* queuedState;
	States::State* currentState;

	StateMachine();

	void Initialize() override;
	void SetActive(const bool& state) override;


};

#endif
