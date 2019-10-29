#include "ParticleSystem.h"

#include "Entity.h"
#include "EntityEvents.h"
#include "ParticleObject.h"

#include "Transform.h"
#include "Render.h"

#include <Events/EventsManager.h>
#include <Math/Random.hpp>
#include <MACROS.h>

ParticleSystem::ParticleSystem() {
	Events::EventsManager::GetInstance()->Subscribe("EMITTER_ACTIVE", &ParticleSystem::EmitterActiveHandler, this);
}

void ParticleSystem::Update(const float& dt) {
	for (auto& pair : particleGroups) {
		auto& emitter = pair.first;
		auto& group = pair.second;

		const auto& position = emitter->GetParent()->GetComponent<Transform>()->GetWorldTranslation();

		for (int i = static_cast<int>(group.size()) - 1; i >= 0; --i) {
			auto object = group[i];
			auto particle = object->GetComponent<Particle>();
			auto transform = object->GetComponent<Transform>();
			auto render = object->GetComponent<Render>();

			const auto dir = Math::Normalized(transform->translation - position);

			transform->translation += particle->velocity * dt;
			particle->velocity += emitter->gravity * dt;

			const float vAccelRad = Math::RandMinMax(-emitter->accelRadRange, emitter->accelRadRange);
			particle->velocity += (emitter->accelRad + vAccelRad) * dt * dir;

			const float ratio = particle->age / particle->lifetime;

			const vec3f dSize = particle->endSize - particle->startSize;
			transform->scale = dSize * ratio + particle->startSize;

			const vec4f dColor = particle->endColor - particle->startColor;
			render->tint = dColor * ratio + particle->startColor;

			particle->age += dt;

			if (particle->age >= particle->lifetime) {
				object->Destroy();
				group.erase(group.begin() + i);
			}
		}

		if (!emitter->IsActive()) continue;
		
		if (emitter->duration > 0) {
			if (emitter->age > emitter->duration) {
				if (emitter->loop && group.empty())
					emitter->age = 0.f;
				else
					continue;
			}

			emitter->age += dt;
		}

		emitter->spawnTimer -= dt;
		if (emitter->spawnTimer <= 0.f) {

			for (unsigned i = 0; i < emitter->burstAmount; ++i) {
				auto event = new Events::CreateAnyEntity<ParticleObject>();
				auto object = event->entity;
				Events::EventsManager::GetInstance()->Trigger("CREATE_ENTITY", event);
				group.push_back(object);

				auto particle = object->GetComponent<Particle>();
				auto transform = object->GetComponent<Transform>();
				auto render = object->GetComponent<Render>();
				
				particle->age = 0.f;
				particle->lifetime = emitter->lifetime + Math::RandMinMax(-emitter->lifetimeRange, emitter->lifetimeRange);

				transform->translation = position + emitter->offset + vec3f(
					Math::RandMinMax(-emitter->positionRange.x, emitter->positionRange.x),
					Math::RandMinMax(-emitter->positionRange.y, emitter->positionRange.y),
					Math::RandMinMax(-emitter->positionRange.z, emitter->positionRange.z)
				);

				const vec3f vAngle = Math::Rad(transform->rotation + emitter->angle + vec3f(
					Math::RandMinMax(-emitter->angleRange.x, emitter->angleRange.x),
					Math::RandMinMax(-emitter->angleRange.y, emitter->angleRange.y),
					Math::RandMinMax(-emitter->angleRange.z, emitter->angleRange.z)
				));

				const float vSpeed = Math::RandMinMax(-emitter->speedRange, emitter->speedRange);

				const vec3f worldUp(sin(vAngle.z), cos(vAngle.z), 0.f);

				// made for 2D
				particle->velocity = (emitter->speed + vSpeed) * worldUp;

				particle->startSize = emitter->startSize + vec3f(
					Math::RandMinMax(-emitter->startSizeRange.x, emitter->startSizeRange.x),
					Math::RandMinMax(-emitter->startSizeRange.y, emitter->startSizeRange.y),
					Math::RandMinMax(-emitter->startSizeRange.z, emitter->startSizeRange.z)
				);

				particle->endSize = emitter->endSize + vec3f(
					Math::RandMinMax(-emitter->endSizeRange.x, emitter->endSizeRange.x),
					Math::RandMinMax(-emitter->endSizeRange.y, emitter->endSizeRange.y),
					Math::RandMinMax(-emitter->endSizeRange.z, emitter->endSizeRange.z)
				);

				transform->scale = particle->startSize;

				// color
				particle->startColor = emitter->startColor + vec4f(
					Math::RandMinMax(-emitter->startColorRange.x, emitter->startColorRange.x),
					Math::RandMinMax(-emitter->startColorRange.y, emitter->startColorRange.y),
					Math::RandMinMax(-emitter->startColorRange.z, emitter->startColorRange.z),
					Math::RandMinMax(-emitter->startColorRange.w, emitter->startColorRange.w)
				);

				particle->endColor = emitter->endColor + vec4f(
					Math::RandMinMax(-emitter->endColorRange.x, emitter->endColorRange.x),
					Math::RandMinMax(-emitter->endColorRange.y, emitter->endColorRange.y),
					Math::RandMinMax(-emitter->endColorRange.z, emitter->endColorRange.z),
					Math::RandMinMax(-emitter->endColorRange.w, emitter->endColorRange.w)
				);

				render->tint = particle->startColor;
			}

			emitter->spawnTimer = emitter->spawnInterval;
		}
	}
}

void ParticleSystem::FixedUpdate(const float& dt) {
	
}

void ParticleSystem::EmitterActiveHandler(Events::Event* event) {
	auto& c = static_cast<Events::AnyType<ParticleEmitter*>*>(event)->data;

	if (particleGroups.find(c) == particleGroups.end())
		particleGroups[c] = {};
}
