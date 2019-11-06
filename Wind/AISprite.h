#ifndef AI_SPRITE_H
#define AI_SPRITE_H

#include "Entity.h"

#include <Math/Vectors.hpp>
#include <Events/Event.h>

class AISprite : public Entity {

protected:

	Entity* interest;
	vec3f target, destination;
	float speed;

public:

	float energy;

	AISprite();

	virtual void Build();
	virtual void Initialize();
	
	void SetInterest(Entity * const _interest);
	Entity * const GetInterest() const;

	void SetTarget(const vec3f& value);

	const vec3f& GetDestination() const;
	void SetDestination(const vec3f& _destination);

	void SetSpeed(const float& value);

protected:

	void Move(const float& dt);
	void Update(const float& dt);

	void OnCollisionEnter(Entity * const target);

	void EventHandler(Events::Event* event);

};

#endif
