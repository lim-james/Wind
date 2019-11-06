#ifndef EVENTS_MANAGER_H
#define EVENTS_MANAGER_H

#include "Event.h"
#include "../Logger/Logger.h"

#include <string>
#include <map>
#include <vector>
#include <functional>

namespace Events {

	class EventsManager {

		static EventsManager* instance;

		std::map<std::string, std::vector<std::function<void()>>> emptyCallbacks;
		std::map<std::string, std::vector<std::function<void(Event*)>>> eventCallbacks;

		std::map<void*, std::map<std::string, std::function<void()>>> emptyContextCallbacks;
		std::map<void*, std::map<std::string, std::function<void(Event*)>>> eventContextCallbacks;

		struct QueuedEvent {
			std::string name;
			Event* event;
		};

		std::vector<QueuedEvent> queuedEvents;

	public:

		~EventsManager();

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

		void Queue(const std::string& name);
		void Queue(const std::string& name, Event* const event);

		void TriggerQueued();

	};

	template<typename Context>
	void EventsManager::Subscribe(const std::string& name, void(Context::*callback)(), Context* const context) {
		const auto function = std::bind(callback, context);
		emptyCallbacks[name].push_back(function);
		emptyContextCallbacks[(void*)context][name] = function;
	}

	template<typename Context>
	void EventsManager::Subscribe(const std::string& name, void(Context::*callback)() const, Context* const context) {
		const auto function = std::bind(callback, context);
		emptyCallbacks[name].push_back(function);
		emptyContextCallbacks[(void*)context][name] = function;
	}

	template<typename Context>
	void EventsManager::Subscribe(const std::string& name, void(Context::*callback)(Event*), Context* const context) {
		const auto function = std::bind(callback, context, std::placeholders::_1);
		eventCallbacks[name].push_back(function);
		eventContextCallbacks[(void*)context][name] = function;
	}

	template<typename Context>
	void EventsManager::Subscribe(const std::string& name, void(Context::*callback)(Event*) const, Context* const context) {
		const auto function = std::bind(callback, context, std::placeholders::_1);
		eventCallbacks[name].push_back(function);
		eventContextCallbacks[(void*)context][name] = function;
	}

	template<typename Context>
	void EventsManager::TriggerContext(const std::string& name, Context* const context) {
		void* ptr = (void*)context;
		if (emptyContextCallbacks.find(ptr) != emptyContextCallbacks.end())
			emptyContextCallbacks[ptr][name]();
	}

	template<typename Context>
	void EventsManager::TriggerContext(const std::string& name, Context* const context, Event* const event) {
		const auto ptr = (void*)context;

		if (emptyContextCallbacks.find(ptr) != emptyContextCallbacks.end())
			emptyContextCallbacks[ptr][name]();

		event->name = name;
		if (eventContextCallbacks.find(ptr) != eventContextCallbacks.end())
			eventContextCallbacks[ptr][name](event);

		delete event;
	}
}

#endif

