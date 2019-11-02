#ifndef EVENTS_MANAGER_H
#define EVENTS_MANAGER_H

#include "Event.h"

#include <string>
#include <map>
#include <vector>
#include <functional>

namespace Events {

	class EventsManager {

		static EventsManager* instance;

		std::map<std::string, std::map<void*, std::function<void()>>> emptyCallbacks;
		std::map<std::string, std::map<void*, std::function<void(Event*)>>> eventCallbacks;

	public:

		static EventsManager* GetInstance();
		static void Destroy();

		// subscribe empty callbacks
		template<typename Context>
		void Subscribe(const std::string& name, void(Context::*callback)(), Context* const context);
		template<typename Context>
		void Subscribe(const std::string& name, void(Context::*callback)() const, Context* const context);

		// subscribe event callbacks
		template<typename Context>
		void Subscribe(const std::string& name, void(Context::*callback)(Event*), Context* const context);
		template<typename Context>
		void Subscribe(const std::string& name, void(Context::*callback)(Event*) const, Context* const context);

		void Trigger(const std::string& name);
		void Trigger(const std::string& name, Event* const event);

		template<typename Context>
		void TriggerContext(const std::string& name, Context* const context);
		template<typename Context>
		void TriggerContext(const std::string& name, Context* const context, Event* const event);

	};

	template<typename Context>
	void EventsManager::Subscribe(const std::string& name, void(Context::*callback)(), Context* const context) {
		emptyCallbacks[name][(void*)context] = std::bind(callback, context);
	}

	template<typename Context>
	void EventsManager::Subscribe(const std::string& name, void(Context::*callback)() const, Context* const context) {
		emptyCallbacks[name][(void*)context] = std::bind(callback, context);
	}

	template<typename Context>
	void EventsManager::Subscribe(const std::string& name, void(Context::*callback)(Event*), Context* const context) {
		eventCallbacks[name][(void*)context] = std::bind(callback, context, std::placeholders::_1);
	}

	template<typename Context>
	void EventsManager::Subscribe(const std::string& name, void(Context::*callback)(Event*) const, Context* const context) {
		eventCallbacks[name][(void*)context] = std::bind(callback, context, std::placeholders::_1);
	}

	template<typename Context>
	void EventsManager::TriggerContext(const std::string& name, Context* const context) {
		void* ptr = (void*)context;
		auto& contextCallbacks = emptyCallbacks[name];
		if (contextCallbacks.find(ptr) != contextCallbacks.end())
			contextCallbacks[ptr]();
	}

	template<typename Context>
	void EventsManager::TriggerContext(const std::string& name, Context* const context, Event* const event) {
		void* ptr = (void*)context;

		auto& emptyContextCallbacks = emptyCallbacks[name];
		if (emptyContextCallbacks.find(ptr) != emptyContextCallbacks.end())
			emptyContextCallbacks[ptr]();

		event->name = name;

		auto& contextCallbacks = eventCallbacks[name];
		if (contextCallbacks.find(ptr) != contextCallbacks.end())
			contextCallbacks[ptr](event);

		delete event;
	}
}


#endif

