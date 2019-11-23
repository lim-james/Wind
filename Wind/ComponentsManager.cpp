#include "ComponentsManager.h"

#include <Events/EventsManager.h>

ComponentsManager::ComponentsManager() {}

ComponentsManager::~ComponentsManager() {
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

void ComponentsManager::Start() {
	Events::EventsManager::GetInstance()->Subscribe("COMPONENT_ATTACHED", &ComponentsManager::OnAttached, this);
	Events::EventsManager::GetInstance()->Subscribe("COMPONENT_DETACHED", &ComponentsManager::OnDetached, this);
}

void ComponentsManager::Stop() {
	Events::EventsManager::GetInstance()->UnsubscribeContext(this);
}

void ComponentsManager::Initialize() {
	for (const auto& set : pools) {
		for (const auto& c : set.second) {
			c->Initialize();
		}
	}
}

void ComponentsManager::OnAttached(Events::Event* event) {
	const auto& component = static_cast<Events::AnyType<Component*>*>(event)->data;
	auto& unusedGroup = unused[typeMap[component]];

	unusedGroup.erase(vfind(unusedGroup, component));
}

void ComponentsManager::OnDetached(Events::Event* event) {
	const auto& component = static_cast<Events::AnyType<Component*>*>(event)->data;
	auto& unusedGroup = unused[typeMap[component]];

	unusedGroup.push_back(component);
}

