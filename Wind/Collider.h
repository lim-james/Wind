#ifndef COLLIDER_H
#define COLLIDER_H

#include "Component.h"

#include <functional>

struct Collider : Component {

	std::function<void(Entity* const)> onCollisionEnter;

	Collider();
	
	void Initialize() override;
	void SetActive(const bool& state) override;

};

#endif
