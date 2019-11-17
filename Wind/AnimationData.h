#ifndef ANIMATION_DATA_H
#define ANIMATION_DATA_H

#include "Keyframe.h"

#include <vector>
#include <functional>

struct AnimationData {

	float et;
	unsigned currentFrame;

	std::vector<Keyframe> frames;
	bool loop;

	std::function<void()> completed;

	AnimationData();

};

#endif
