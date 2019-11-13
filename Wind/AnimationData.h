#ifndef ANIMATION_DATA_H
#define ANIMATION_DATA_H

#include "Keyframe.h"

#include <vector>

struct AnimationData {

	float et;
	unsigned currentFrame;

	std::vector<Keyframe> frames;
	bool loop;

	AnimationData();

};

#endif
