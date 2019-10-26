#include "SystemsManager.h"

SystemsManager::~SystemsManager() {
	for (const auto& s : systems)
		delete s.second;

	systems.clear();
}

void SystemsManager::Initialize() {
	for (const auto& s : systems)
		s.second->Initialize();
}

void SystemsManager::Update(const float& t) {
	for (const auto& s : systems)
		s.second->Update(t);
}