#ifndef GHOST_STATES_H
#define GHOST_STATES_H

#include "State.h"

namespace States {

	class Entering : public State{

	public:

		void Enter(Entity* const target) override;
		void Update(Entity* const target, const float& dt) override;
		void FixedUpdate(Entity* const target, const float& dt) override;
		void Exit(Entity* const target) override;

	};

	class Scatter : public State {

	public:

		void Enter(Entity* const target) override;
		void Update(Entity* const target, const float& dt) override;
		void FixedUpdate(Entity* const target, const float& dt) override;
		void Exit(Entity* const target) override;

	};

	class Eaten : public State {

	public:

		void Enter(Entity* const target) override;
		void Update(Entity* const target, const float& dt) override;
		void FixedUpdate(Entity* const target, const float& dt) override;
		void Exit(Entity* const target) override;

	};

	class Frightened : public State {

	public:

		void Enter(Entity* const target) override;
		void Update(Entity* const target, const float& dt) override;
		void FixedUpdate(Entity* const target, const float& dt) override;
		void Exit(Entity* const target) override;

	};

}

#endif
