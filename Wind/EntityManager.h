#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "Entity.h"

#include <Events/Event.h>

#include <vector>

class EntityManager {

	ComponentsManager* componentsManager;

	std::map<Entity*, unsigned> typeMap;
	std::map<unsigned, unsigned> expandSizes;

	// pools
	//std::map<unsigned, std::map<std::string, std::vector<Entity*>>> used;
	std::map<std::string, std::map<unsigned, std::vector<Entity*>>> used;
	std::map<unsigned, std::vector<Entity*>> unused;

public:

	EntityManager(ComponentsManager* manager);
	~EntityManager();

	void Start();
	void Stop();

	void Initialize();

	template<typename EntityType>
	void Initialize();

	void Destroy();

	template<typename EntityType>
	const bool Has() const;

	template<typename EntityType>
	void Subscribe(int start, const unsigned& expand);

	template<typename EntityType>
	EntityType* const Fetch();

	template<typename EntityType>
	EntityType* const Create();

	const std::map<unsigned, std::vector<Entity*>>& GetEntitiesWithTag(const std::string& tag);


private:

	template<typename ComponentType>
	void Expand();

	void AddEntity(const unsigned& hash, Entity* entity);

	void OnCreate(Events::Event* event);

	void OnUsed(Events::Event* event);
	void OnDestroy(Events::Event* event);

	void TagChangeHandler(Events::Event* event);

};


template<typename EntityType>
void EntityManager::Initialize() {
	for (const auto& sets : used)
		for (const auto& c : sets.second[hashof(EntityType)])
			c->Initialize();

	for (const auto& c : unused[hashof(EntityType)])
		c->Initialize();
}

template<typename EntityType>
const bool EntityManager::Has() const {
	return unused.find(hashof(EntityType)) != unused.end();
}

template<typename EntityType>
void EntityManager::Subscribe(int start, const unsigned& expand) {
	if (Has<EntityType>()) return;

	const auto hash = hashof(EntityType);

	expandSizes[hash] = expand;

	unused[hash].reserve(start);

	while (--start >= 0)
		AddEntity(hash, new EntityType);
}

template<typename EntityType>
EntityType* const EntityManager::Fetch() {
	const auto hash = hashof(EntityType);
	if (unused[hash].empty())
		Expand<EntityType>();

	return static_cast<EntityType * const>(*unused[hash].begin());
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
	const auto hash = hashof(EntityType);

	for (unsigned i = 0; i < expandSizes[hash]; ++i)
		AddEntity(hash, new EntityType);
}


#endif
