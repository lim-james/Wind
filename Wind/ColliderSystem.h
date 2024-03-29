#ifndef COLLIDER_SYSTEM_H
#define COLLIDER_SYSTEM_H

#include "System.h"
#include "Collider.h"

#include <Events/Event.h>

#include <map>
#include <vector>

class ColliderSystem : public System {

	std::map<Collider*, std::map<Collider*, bool>> history;
	std::vector<Collider*> components;

public:

	~ColliderSystem() override;

	void Start() override;

	void Update(const float& dt) override;
	void FixedUpdate(const float& dt) override;

private:

	void ActiveHandler(Events::Event* event);

};

#endif