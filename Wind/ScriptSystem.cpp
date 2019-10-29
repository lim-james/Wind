#include "ScriptSystem.h"

#include <Events/EventsManager.h>
#include <MACROS.h>

ScriptSystem::ScriptSystem() {
	Events::EventsManager::GetInstance()->Subscribe("SCRIPT_ACTIVE", &ScriptSystem::ActiveHandler, this);
}

ScriptSystem::~ScriptSystem() {
	components.clear();
}

void ScriptSystem::Initialize() {
	for (auto& c : components) {
		c->reset();
		c->start();
	}
}

void ScriptSystem::Update(const float& dt) {
	for (auto& c : components)
		c->update(dt);
}

void ScriptSystem::FixedUpdate(const float& dt) {
	for (auto& c : components)
		c->fixedUpdate(dt);
}

void ScriptSystem::ActiveHandler(Events::Event* event) {
	const auto c = static_cast<Events::AnyType<Script*>*>(event)->data;

	if (c->IsActive()) {
		c->awake();
		components.push_back(c);
	} else {
		c->destroy();
		components.erase(vfind(components, c));
	}
}