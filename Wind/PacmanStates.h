#ifndef PACMAN_STATES_H
#define PACMAN_STATES_H

#include "State.h"

namespace States {
		
	class Hungry : public State {

	public:

		void Enter(Entity* const target) override;
		void Update(Entity* const target, const float& dt) override;
		void FixedUpdate(Entity* const target, const float& dt) override;
		void Exit(Entity* const target) override;

	private:
		
		void SetNearestPallet(Entity * const target);
		Entity * GetNearestGhost(Entity * const target);

	};

	class Search : public State {

	public:

		void Enter(Entity* const target) override;
		void Update(Entity* const target, const float& dt) override;
		void FixedUpdate(Entity* const target, const float& dt) override;
		void Exit(Entity* const target) override;

	private:
		
		void SetNearestPower(Entity * const target);

	};

	class Avoid : public State {

	public:

		void Enter(Entity* const target) override;
		void Update(Entity* const target, const float& dt) override;
		void FixedUpdate(Entity* const target, const float& dt) override;
		void Exit(Entity* const target) override;

	private:
		
		void SetGhostInRange(Entity * const target, const float& range);

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
		
	class Dead : public State {

	public:

		void Enter(Entity* const target) override;
		void Update(Entity* const target, const float& dt) override;
		void FixedUpdate(Entity* const target, const float& dt) override;
		void Exit(Entity* const target) override;

	};
	
}

#endif
