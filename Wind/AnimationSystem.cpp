#include "AnimationSystem.h"

#include "Entity.h"
#include "Render.h"

#include <Events/EventsManager.h>

AnimationSystem::AnimationSystem() {
	Events::EventsManager::GetInstance()->Subscribe("ANIMATION_ACTIVE", &AnimationSystem::ActiveHandler, this);
}

AnimationSystem::~AnimationSystem() {
	components.clear();
}

void AnimationSystem::Update(const float& dt) {
	for (auto& c : components) {
		if (c->queued != "") {
			ResetAnimation(c->animations[c->currentAnimation]);
			c->currentAnimation = c->queued;
			c->queued = "";
		}
		
		if (c->currentAnimation != "") {
			auto& currentAnimation = c->animations[c->currentAnimation];
			currentAnimation.et += dt;
			ProcessAnimation(c, currentAnimation);
		}
	}
}

void AnimationSystem::FixedUpdate(const float& dt) {}

void AnimationSystem::ActiveHandler(Events::Event* event) {
	const auto c = static_cast<Events::AnyType<Animation*>*>(event)->data;

	if (c->IsActive()) {
		components.push_back(c);
	} else {
		components.erase(vfind(components, c));
	}
}

void AnimationSystem::ResetAnimation(AnimationData& animation) {
	animation.et = 0.f;
	animation.currentFrame = 0;
}

void AnimationSystem::ProcessAnimation(Animation* const component, AnimationData& animation) {
	if (animation.frames.size() <= animation.currentFrame) return;

	auto& currentFrame = animation.frames[animation.currentFrame];
	if (animation.et >= currentFrame.duration) {
		++animation.currentFrame;
		if (animation.currentFrame == animation.frames.size()) {
			if (animation.loop) {
				animation.currentFrame = 0;
			} else {
				animation.completed();
				return;
			}
		}
		animation.et = 0.f;
		SetProperties(component, animation);
	}
}

void AnimationSystem::SetProperties(Animation* const component, AnimationData& animation) {
	auto render = component->GetParent()->GetComponent<Render>();
	render->uvRect = animation.frames[animation.currentFrame].uvRect;
}
