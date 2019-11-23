#ifndef COMPONENTS_MANAGER_H
#define COMPONENTS_MANAGER_H

#include "Component.h"

#include <MACROS.h>
#include <Logger/Logger.h>
#include <Events/Event.h>

#include <map>
#include <typeindex>
#include <vector>

class ComponentsManager {

	std::map<Component*, unsigned> typeMap;
	std::map<unsigned, unsigned> expandSizes;

	std::map<std::type_index, std::vector<Component*>> pools;
	std::map<unsigned, std::vector<Component*>> unused;

public:

	ComponentsManager();
	~ComponentsManager();

	void Start();
	void Stop();

	void Initialize();

	template<typename ComponentType>
	void Initialize();

	template<typename ComponentType>
	const bool Has() const;

	template<typename ComponentType>
	void Subscribe(int start, const unsigned& expand);

	template<typename ComponentType>
	ComponentType* const Fetch();

private:

	template<typename ComponentType>
	void Expand();

	void OnAttached(Events::Event* event);
	void OnDetached(Events::Event* event);

};

template<typename ComponentType>
void ComponentsManager::Initialize() {
	for (const auto& c : pools[indexof(ComponentType)])
		c->Initialize();
}

template<typename ComponentType>
const bool ComponentsManager::Has() const {
	return pools.find(indexof(ComponentType)) != pools.end();
}

template<typename ComponentType>
void ComponentsManager::Subscribe(int start, const unsigned& expand) {
	if (Has<ComponentType>()) return;

	const auto index = indexof(ComponentType);
	const auto hash = hashof(ComponentType);

	expandSizes[hash] = expand;

	pools[index].reserve(start);
	unused[hash].reserve(start);

	while (--start >= 0) {
		Component* component = new ComponentType;
		typeMap[component] = hash;
		pools[index].push_back(component);
		unused[hash].push_back(component);
	}
}

template<typename ComponentType>
ComponentType* const ComponentsManager::Fetch() {
	const auto hash = hashof(ComponentType);
	if (unused[hash].empty())
		Expand<ComponentType>();

	return static_cast<ComponentType * const>(*unused[hash].begin());
}

template<typename ComponentType>
void ComponentsManager::Expand() {
	const auto index = indexof(ComponentType);
	const auto hash = hashof(ComponentType);

	for (unsigned i = 0; i < expandSizes[hash]; ++i) {
		Component* component = new ComponentType;
		typeMap[component] = hash;
		pools[index].push_back(component);
		unused[hash].push_back(component);
	}
}


#endif
