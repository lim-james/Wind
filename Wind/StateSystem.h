#ifndef STATE_SYSTEM_H
#define STATE_SYSTEM_H

#include "System.h"
#include "StateMachine.h"

#include <Events/Event.h>

#include <vector>

class StateSystem : public System {

	std::vector<StateMachine*> components;

public:

	StateSystem();

	void Update(const float& dt) override;
	void FixedUpdate(const float& dt) override;

private:

	void ActiveHandler(Events::Event* event);

};

#endif
