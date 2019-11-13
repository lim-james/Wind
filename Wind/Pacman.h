#ifndef PACMAN_H
#define PACMAN_H

#include "AISprite.h"

class Pacman : public AISprite {

public:

	void Build() override;
	void Initialize() override;

private:

	// parent class methods

	void SetDirection(const vec3f& _direction) override;
	void SetNewTarget() override;

	// component methods

	// collider 
	void OnCollisionEnter(Entity * const target);
	// script
	void FixedUpdate(const float& dt);

};

#endif
