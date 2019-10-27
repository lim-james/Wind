#include "ParticleSystem.h"

#include <Events/EventsManager.h>
#include <MACROS.h>

ParticleSystem::ParticleSystem() {
	Events::EventsManager::GetInstance()->Subscribe("EMITTER_ACTIVE", &ParticleSystem::EmitterActiveHandler, this);
}

void ParticleSystem::Update(const float& dt) {
	for (auto& emit : emitters) {
		
	}
}

void ParticleSystem::FixedUpdate(const float& dt) {
	
}

void ParticleSystem::EmitterActiveHandler(Events::Event* event) {
	auto& c = static_cast<Events::AnyType<ParticleEmitter*>*>(event)->data;

	if (c->IsActive()) {
		emitters.push_back(c);
	} else {
		emitters.erase(vfind(emitters, c));
	}
}
