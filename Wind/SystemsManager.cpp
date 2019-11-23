#include "SystemsManager.h"

#include <Logger/Logger.h>

SystemsManager::~SystemsManager() {
	for (const auto& s : systems)
		delete s.second;

	systems.clear();
}

void SystemsManager::Start() {
	for (auto& s : systems)
		s.second->Start();
}

void SystemsManager::Stop() {
	for (auto& s : systems)
		s.second->Stop();
}

void SystemsManager::Initialize() {
	for (const auto& s : systems)
		s.second->Initialize();
}

void SystemsManager::Update(const float& dt) {
	for (const auto& s : systems)
		s.second->Update(dt);
}

void SystemsManager::FixedUpdate(const float& dt) {
	for (const auto& s : systems)
		s.second->FixedUpdate(dt);
}