#include "Animation.h"

#include <Events/EventsManager.h>

Animation::Animation() 
	: currentAnimation("") {}

void Animation::Initialize() {
	animations = {};
	currentAnimation = "";
}

void Animation::SetActive(const bool& state) {
	Component::SetActive(state);
	Events::EventsManager::GetInstance()->Queue("ANIMATION_ACTIVE", new Events::AnyType<Animation*>(this));
}

