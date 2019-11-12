#ifndef GHOST_CHASE_STATES_H
#define GHOST_CHASE_STATES_H

#include "State.h"

namespace States {

	class BlinkyChase : public State {

	public:

		void Enter(Entity* const target) override;
		void Update(Entity* const target, const float& dt) override;
		void FixedUpdate(Entity* const target, const float& dt) override;
		void Exit(Entity* const target) override;

	};

	class PinkyChase : public State {

	public:

		void Enter(Entity* const target) override;
		void Update(Entity* const target, const float& dt) override;
		void FixedUpdate(Entity* const target, const float& dt) override;
		void Exit(Entity* const target) override;

	};

	class ClydeChase : public State {

	public:

		void Enter(Entity* const target) override;
		void Update(Entity* const target, const float& dt) override;
		void FixedUpdate(Entity* const target, const float& dt) override;
		void Exit(Entity* const target) override;

	};

	class ClydeInverseChase : public State {

	public:

		void Enter(Entity* const target) override;
		void Update(Entity* const target, const float& dt) override;
		void FixedUpdate(Entity* const target, const float& dt) override;
		void Exit(Entity* const target) override;

	};

	class InkyChase : public State {

	public:

		void Enter(Entity* const target) override;
		void Update(Entity* const target, const float& dt) override;
		void FixedUpdate(Entity* const target, const float& dt) override;
		void Exit(Entity* const target) override;

	};




}

#endif
