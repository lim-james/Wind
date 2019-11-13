#ifndef ANIMATION_H
#define ANIMATION_H

#include "Component.h"

#include "AnimationData.h"

#include <map>
#include <string>

struct Animation : Component {

	std::map<std::string, AnimationData> animations;
	std::string currentAnimation, queued;

	Animation();

	void Initialize() override;
	void SetActive(const bool& state) override;

};

#endif
