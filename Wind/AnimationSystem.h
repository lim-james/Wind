#ifndef ANIMATION_SYSTEM_H
#define ANIMATION_SYSTEM_H

#include "System.h"
#include "Animation.h"

#include <Events/Event.h>

#include <vector>

class AnimationSystem : public System {

	std::vector<Animation*> components;

public:

	AnimationSystem();
	~AnimationSystem() override;

	void Update(const float& dt) override;
	void FixedUpdate(const float& dt) override;

private:

	// Event handlers

	void ActiveHandler(Events::Event* event);

	// helper methods

	void ResetAnimation(AnimationData& animation);
	void ProcessAnimation(Animation* const component, AnimationData& animation);
	void SetProperties(Animation* const component, AnimationData& animation);
	
};

#endif