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
		for (auto& callback : emptyCallbacks[name])
			callback();
	}

	void EventsManager::Trigger(const std::string& name, Event* event) {
		for (auto& callback : emptyCallbacks[name])
			callback();

		event->name = name;
		for (auto& callback : eventCallbacks[name])
			callback(event);

		delete event;
	}

}
