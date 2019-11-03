#ifndef AI_SPRITE_H
#define AI_SPRITE_H

#include "Entity.h"

#include <Math/Vectors.hpp>
#include <Events/Event.h>

class AISprite : public Entity {

protected:

	vec3f target;
	float speed;

public:

	AISprite();

	virtual void Build();
	virtual void Initialize();

	void SetTarget(const vec3f& value);
	void SetSpeed(const float& value);

protected:

	void Move(const float& dt);
	void Update(const float& dt);

	void OnCollisionEnter(Entity * const target);

	void EventHandler(Events::Event* event);

};

#endif