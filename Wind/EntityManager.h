#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "Entity.h"

#include <Events/Event.h>

#include <vector>

class EntityManager {

	ComponentsManager* componentsManager;

	std::map<Entity*, unsigned> typeMap;
	std::map<unsigned, unsigned> expandSizes;

	std::map<std::type_index, std::vector<Entity*>> pools;
	std::map<unsigned, std::vector<Entity*>> unused;

public:

	EntityManager(ComponentsManager* manager);
	~EntityManager();

	void Initialize();

	template<typename EntityType>
	void Initialize();

	template<typename EntityType>
	const bool Has() const;

	template<typename EntityType>
	void Subscribe(int start, const unsigned& expand);

	template<typename EntityType>
	EntityType* const Fetch();

	template<typename EntityType>
	EntityType* const Create();

private:

	template<typename ComponentType>
	void Expand();

	void OnUsed(Events::Event* event);
	void OnDestroy(Events::Event* event);

};


template<typename EntityType>
void EntityManager::Initialize() {
	for (const auto& c : pools[indexof(EntityType)])
		c->Initialize();
}

template<typename EntityType>
const bool EntityManager::Has() const {
	return pools.find(indexof(EntityType)) != pools.end();
}

template<typename EntityType>
void EntityManager::Subscribe(int start, const unsigned& expand) {
	if (Has<EntityType>()) return;

	const auto index = indexof(EntityType);
	const auto hash = hashof(EntityType);

	expandSizes[hash] = expand;

	pools[index].reserve(start);
	unused[hash].reserve(start);

	while (--start >= 0) {
		Entity* entity = new EntityType;
		entity->componentsManager = componentsManager;
		entity->Build();

		typeMap[entity] = hash;
		pools[index].push_back(entity);
		unused[hash].push_back(entity);
	}
}

template<typename EntityType>
EntityType* const EntityManager::Fetch() {
	const auto hash = hashof(EntityType);
	if (unused[hash].empty())
		Expand<EntityType>();

	return dynamic_cast<EntityType * const>(*unused[hash].begin());
}

template<typename EntityType>
EntityType* const EntityManager::Create() {
	auto result = Fetch<EntityType>();
	result->Initialize();
	result->Use();

	return result;
}

template<typename EntityType>
void EntityManager::Expand() {
	const auto index = indexof(EntityType);
	const auto hash = hashof(EntityType);

	for (unsigned i = 0; i < expandSizes[hash]; ++i) {
		Entity* entity = new EntityType;
		entity->componentsManager = componentsManager;
		entity->Build();

		typeMap[entity] = hash;
		pools[index].push_back(entity);
		unused[hash].push_back(entity);
	}
}


#endif