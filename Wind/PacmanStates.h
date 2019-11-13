#ifndef PACMAN_STATES_H
#define PACMAN_STATES_H

#include "State.h"

namespace States {
	
	class Search : public State {

	public:

		void Enter(Entity* const target) override;
		void Update(Entity* const target, const float& dt) override;
		void FixedUpdate(Entity* const target, const float& dt) override;
		void Exit(Entity* const target) override;

	};

	class Avoid : public State {

	public:

		void Enter(Entity* const target) override;
		void Update(Entity* const target, const float& dt) override;
		void FixedUpdate(Entity* const target, const float& dt) override;
		void Exit(Entity* const target) override;

	};
	
	class Hunt : public State {

	public:

		void Enter(Entity* const target) override;
		void Update(Entity* const target, const float& dt) override;
		void FixedUpdate(Entity* const target, const float& dt) override;
		void Exit(Entity* const target) override;

	private:
		
		void SetNearestGhost(Entity * const target);

	};
	
}

#endif
