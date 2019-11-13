#ifndef PACMAN_H
#define PACMAN_H

#include "AISprite.h"

class Pacman : public AISprite {

public:

	void Build() override;
	void Initialize() override;

private:

	void SetDirection(const vec3f& _direction) override;

	void FixedUpdate(const float& dt);

	void OnCollisionEnter(Entity * const target);

};

#endif
