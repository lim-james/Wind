#ifndef STATE_H
#define STATE_H

#include <string>

class Entity;

namespace States {

	class State {

	public:

		virtual void Enter(Entity* const target) = 0;
		virtual void Update(Entity* const target, const float& dt) = 0;
		virtual void FixedUpdate(Entity* const target, const float& dt) = 0;
		virtual void Exit(Entity* const target) = 0;

	};

}


#endif
