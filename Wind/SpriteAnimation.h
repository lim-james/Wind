#ifndef ANIMATION_H
#define ANIMATION_H

#include "Component.h"

#include "SpriteAnimationData.h"

#include <map>
#include <string>

struct SpriteAnimation : Component {

	std::map<std::string, SpriteAnimationData> animations;
	std::string currentAnimation, queued;

	SpriteAnimation();

	void Initialize() override;
	void SetActive(const bool& state) override;

};

#endif
