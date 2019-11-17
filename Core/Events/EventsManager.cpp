#include "EventsManager.h"

namespace Events {

	EventsManager* EventsManager::instance = nullptr;


	EventsManager::~EventsManager() {
		for (const auto& queued : queuedEvents)
			if (queued.event)
				delete queued.event;
	}

	EventsManager* EventsManager::GetInstance() {
		if (!instance)
			instance = new EventsManager;

		return instance;
	}

	void EventsManager::Destroy() {
		delete instance;
	}

	void EventsManager::UnsubscribeAll() {
		emptyCallbacks.clear();
		eventCallbacks.clear();
		emptyContextCallbacks.clear();
		eventContextCallbacks.clear();
	}

	void EventsManager::Unsubscribe(const std::string& name) {
		emptyCallbacks[name].clear();
		eventCallbacks[name].clear();
		
		for (auto& callbacks : emptyContextCallbacks)
			callbacks.second.erase(name);

		for (auto& callbacks : eventContextCallbacks)
			callbacks.second.erase(name);
	}

	void EventsManager::Trigger(const std::string& name) {
		for (auto& callback : emptyCallbacks[name])
			callback();
	}

	void EventsManager::Trigger(const std::string& name, Event* const event) {
		for (auto& callback : emptyCallbacks[name])
			callback();

		event->name = name;
		for (auto& callback : eventCallbacks[name])
			callback(event);

		delete event;
	}

	void EventsManager::Queue(const std::string& name) {
		queuedEvents.push_back({ name, nullptr });
	}

	void EventsManager::Queue(const std::string& name, Event* const event) {
		queuedEvents.push_back({ name, event });
	}

	void EventsManager::TriggerQueued() {
		for (const auto& queued : queuedEvents) {
			if (queued.event)
				Trigger(queued.name, queued.event);
			else
				Trigger(queued.name);
		}

		queuedEvents.clear();
	}

}
