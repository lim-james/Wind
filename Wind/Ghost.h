#ifndef GHOST_H
#define GHOST_H

#include "AISprite.h"

class Ghost : public AISprite {

	// scatter attrib
	vec3f dock, corner;
	// specific chase state
	std::string chaseState;
	// inky specific
	Ghost* partner;

public:

	Ghost();

	void Initialize() override;

	void InvertDirection();

	const vec3f& GetDock() const;
	void SetDock(const vec3f& position);

	const vec3f& GetCorner() const;
	void SetCorner(const vec3f& position);

	const std::string& GetChaseState() const;
	void SetChaseState(const std::string& state);

	Ghost* const GetPartner() const;
	void SetPartner(Ghost* const ghost);

private:

	void StateHandler(Events::Event* event);

	void SetEnterTarget();
	void Move(const float& dt) override;

};

#endif

