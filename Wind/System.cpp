#include "System.h"

#include <Events/EventsManager.h>

System::~System() {}

void System::Start() {}

void System::Stop() {
	Events::EventsManager::GetInstance()->UnsubscribeContext(this);
}

void System::Initialize() {}
