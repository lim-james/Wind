#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "System.h"
#include "StateContainer.h"

#include "State.h"

#include <Events/Event.h>

#include <vector>
#include <map>

class StateMachine : public System {

	std::map<std::string, States::State*> states;
	std::vector<StateContainer*> components;

public:

	StateMachine();
	~StateMachine() override;

	void Update(const float& dt) override;
	void FixedUpdate(const float& dt) override;

	template<typename State>
	void AttachState(const std::string& key);

private:

	void ActiveHandler(Events::Event* event);

};


template<typename State>
void StateMachine::AttachState(const std::string& key) {
	states[key] = new State;
}

#endif
