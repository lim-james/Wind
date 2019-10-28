#include "ParticleObject.h"

#include "Transform.h"
#include "Render.h"
#include "Particle.h"

void ParticleObject::Build() {
	AddComponent<Transform>();
	AddComponent<Render>();
	AddComponent<Particle>();
}
