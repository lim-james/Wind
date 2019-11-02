#ifndef TEST_STATE_H
#define TEST_STATE_H

#include "State.h"

namespace States {

	class LeftState : public State {
		
	public:

		void Enter(Entity * const target) override;
		void Update(Entity* const target, const float& dt) override;
		void Exit(Entity* const target) override;

	};

	class RightState : public State {
		
	public:

		void Enter(Entity * const target) override;
		void Update(Entity* const target, const float& dt) override;
		void Exit(Entity* const target) override;

	};

	class StopState : public State {
		
	public:

		void Enter(Entity * const target) override;
		void Update(Entity* const target, const float& dt) override;
		void Exit(Entity* const target) override;

	};

}

#endif
