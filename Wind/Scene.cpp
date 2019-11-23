#include "Scene.h"

// components
#include "Transform.h"
#include "StateContainer.h"
// events
#include "EntityEvents.h"

#include <Math/Vectors.hpp>
#include <Events/EventsManager.h>

Scene::Scene() {
	components = new ComponentsManager;
	entities = new EntityManager(components);
	systems = new SystemsManager;
}

Scene::~Scene() {
	delete entities;
	delete components;
	delete systems;

	Destroy();
}

void Scene::Awake() {
	Events::EventsManager::GetInstance()->Subscribe("NEAREST_ENTITY_WITH_TAG", &Scene::NearestEntityHanlder, this);
	Events::EventsManager::GetInstance()->Subscribe("FIRST_ENTITY_WITH_TAG", &Scene::FirstEntityHanlder, this);

	components->Start();
	entities->Start();
	systems->Start();
}

void Scene::Reset() {
	Events::EventsManager::GetInstance()->Subscribe("NEAREST_ENTITY_WITH_TAG", &Scene::NearestEntityHanlder, this);
	Events::EventsManager::GetInstance()->Subscribe("FIRST_ENTITY_WITH_TAG", &Scene::FirstEntityHanlder, this);

	components->Start();
	entities->Start();
	systems->Start();
}

void Scene::Start() {

}

void Scene::FixedUpdate(const float& dt) {
	systems->FixedUpdate(dt);
}

void Scene::Update(const float& dt) {
	systems->Update(dt);
}

void Scene::Stop() {
	components->Stop();
	entities->Stop();
	systems->Stop();

	Events::EventsManager::GetInstance()->UnsubscribeContext(this);
}

void Scene::Destroy() {}

void Scene::PrepareForSegue(Scene * destination) { }

void Scene::NearestEntityHanlder(Events::Event* event) {
	auto entityEvent = static_cast<Events::NearestEntityWithTag*>(event);
	const auto items = entities->GetEntitiesWithTag(entityEvent->tag);
	
	float len = -1.f;

	if (entityEvent->state == "") {
		for (const auto& list : items) {
			for (const auto& entity : list.second) {
				const auto d = entityEvent->position - entity->GetComponent<Transform>()->GetWorldTranslation();
				const float currentLength = Math::LengthSquared(d);
				if (len < 0 || len > currentLength) {
					len = currentLength;
					*entityEvent->entityRef = entity;
				}
			}
		}
	} else {
		for (const auto& list : items) {
			for (const auto& entity : list.second) {
				const auto stateContainer = entity->GetComponent<StateContainer>();
				if (!stateContainer) continue;
				const auto& state = stateContainer->currentState;
				if (state != entityEvent->state)  continue;
				
				const auto d = entityEvent->position - entity->GetComponent<Transform>()->GetWorldTranslation();
				const float currentLength = Math::LengthSquared(d);
				if (len < 0 || len > currentLength) {
					len = currentLength;
					*entityEvent->entityRef = entity;
				}
			}
		}
	}

}

void Scene::FirstEntityHanlder(Events::Event* event) {
	auto entityEvent = static_cast<Events::FindEntityWithTag*>(event);
	const auto entityMap = entities->GetEntitiesWithTag(entityEvent->tag);
	if (entityMap.size()) {
		if (entityEvent->state == "") {
			const auto firstSet = (*entityMap.begin()).second;
			if (firstSet.size()) {
				*entityEvent->entityRef = firstSet[0];
			}
		} else {
			for (const auto& pair : entityMap) {
				for (const auto& entity : pair.second) {
					if (entity->GetComponent<StateContainer>()->currentState == entityEvent->state) {
						*entityEvent->entityRef = entity;
						return;
					}
				}
			}
		}
	}
}

