#ifndef COLLIDER_SYSTEM_H
#define COLLIDER_SYSTEM_H

#include "System.h"
#include "Collider.h"

#include <Events/Event.h>

#include <vector>

class ColliderSystem : public System {

	std::vector<Collider*> components;

public:

	ColliderSystem();
	~ColliderSystem() override;

	void Update(const float& dt) override;
	void FixedUpdate(const float& dt) override;

private:

	void ActiveHandler(Events::Event* event);
};

#endif