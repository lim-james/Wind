#include "EntityManager.h"

#include <Events/EventsManager.h>	

EntityManager::EntityManager(ComponentsManager* manager)
	: componentsManager(manager) {
	Events::EventsManager::GetInstance()->Subscribe("ENTITY_USED", &EntityManager::OnUsed, this);
	Events::EventsManager::GetInstance()->Subscribe("ENTITY_DESTROY", &EntityManager::OnDestroy, this);
}

EntityManager::~EntityManager() {
	typeMap.clear();
	expandSizes.clear();

	for (auto& set : pools) {
		for (auto& c : set.second) {
			delete c;
		}
		set.second.clear();
	}

	pools.clear();
	unused.clear();
}

void EntityManager::Initialize() {
	for (const auto& set : pools) {
		for (const auto& c : set.second) {
			c->Initialize();
		}
	}
}

void EntityManager::OnUsed(Events::Event* event) {
	const auto& entity = static_cast<Events::AnyType<Entity*>*>(event)->data;
	auto& unusedGroup = unused[typeMap[entity]];

	unusedGroup.erase(vfind(unusedGroup, entity));
}

void EntityManager::OnDestroy(Events::Event* event) {
	const auto& entity = static_cast<Events::AnyType<Entity*>*>(event)->data;
	unused[typeMap[entity]].push_back(entity);
}
