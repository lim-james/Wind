#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "System.h"
#include "ParticleEmitter.h"

#include <Events/Event.h>
#include <vector>

class ParticleSystem : public System {
	
	std::vector<ParticleEmitter*> emitters;

public:

	ParticleSystem();

	void Update(const float& dt) override;
	void FixedUpdate(const float& dt) override;

private:

	void EmitterActiveHandler(Events::Event* event);


};

#endif