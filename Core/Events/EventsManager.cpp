#include "EventsManager.h"

namespace Events {

	EventsManager* EventsManager::instance = nullptr;

	EventsManager* EventsManager::GetInstance() {
		if (!instance)
			instance = new EventsManager;

		return instance;
	}

	void EventsManager::Destroy() {
		delete instance;
	}

	void EventsManager::Trigger(const std::string& name) {
		for (auto& callbackPair : emptyCallbacks[name])
			callbackPair.second();
	}

	void EventsManager::Trigger(const std::string& name, Event* const event) {
		for (auto& callbackPair : emptyCallbacks[name])
			callbackPair.second();

		event->name = name;
		for (auto& callbackPair : eventCallbacks[name])
			callbackPair.second(event);

		delete event;
	}

}
