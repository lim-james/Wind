#ifndef ANIMATION_SYSTEM_H
#define ANIMATION_SYSTEM_H

#include "System.h"
#include "Animation.h"

#include <Events/Event.h>

#include <vector>

class AnimationSystem : public System {

	std::vector<Animation*> components;

public:

	~AnimationSystem() override;

	void Start() override;

	void Update(const float& dt) override;
	void FixedUpdate(const float& dt) override;

private:

	// Event handlers

	void ActiveHandler(Events::Event* event);

};

#endif
