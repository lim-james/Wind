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

		std::map<void*, std::map<std::string, std::function<void()>*>> emptyContextCallbacks;
		std::map<void*, std::map<std::string, std::function<void(Event*)>*>> eventContextCallbacks;

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

		void UnsubscribeAll();
		void Unsubscribe(const std::string& name);
		template<typename Context>
		void UnsubscribeContext(Context* const context);
		template<typename Context>
		void Unsubscribe(const std::string& name, Context* const context);

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
		auto& list = emptyCallbacks[name];
		list.push_back(std::bind(callback, context));
		emptyContextCallbacks[(void*)context][name] = &list.back();
	}

	template<typename Context>
	void EventsManager::Subscribe(const std::string& name, void(Context::*callback)() const, Context* const context) {
		auto& list = emptyCallbacks[name];
		list.push_back(std::bind(callback, context));
		emptyContextCallbacks[(void*)context][name] = &list.back();
	}

	template<typename Context>
	void EventsManager::Subscribe(const std::string& name, void(Context::*callback)(Event*), Context* const context) {
		auto& list = eventCallbacks[name];
		list.push_back(std::bind(callback, context, std::placeholders::_1));
		eventContextCallbacks[(void*)context][name] = &list.back();
	}

	template<typename Context>
	void EventsManager::Subscribe(const std::string& name, void(Context::*callback)(Event*) const, Context* const context) {
		auto& list = eventCallbacks[name];
		list.push_back(std::bind(callback, context, std::placeholders::_1));
		eventContextCallbacks[(void*)context][name] = &list.back();
	}

	template<typename Context>
	void EventsManager::UnsubscribeContext(Context* const context) {
		void* ptr = (void*)context;

		auto& empty = emptyContextCallbacks[ptr];

		for (auto& pair : empty) {
			auto& list = emptyCallbacks[pair.first];

			for (unsigned i = 0; i < list.size(); ++i) {
				if (&list[i] == pair.second) {
					list.erase(list.begin() + i);
					break;
				}
			}
		}

		empty.clear();

		auto& events = eventContextCallbacks[ptr];

		for (auto& pair : events) {
			auto& list = eventCallbacks[pair.first];

			for (unsigned i = 0; i < list.size(); ++i) {
				if (&list[i] == pair.second) {
					list.erase(list.begin() + i);
					break;
				}
			}
		}

		events.clear();
	}

	template<typename Context>
	void EventsManager::Unsubscribe(const std::string& name, Context* const context) {
		void* ptr = (void*)context;

		auto& empty = emptyContextCallbacks[ptr][name];
		auto& emptyList = emptyCallbacks[name];
		for (unsigned i = 0; i < emptyList.size(); ++i) {
			if (&emptyList[i] == empty) {
				emptyList.erase(emptyList.begin() + i);
				break;
			}
		}
		emptyContextCallbacks[ptr].erase(name);

		auto& events = eventContextCallbacks[ptr][name];
		auto& eventsList = eventCallbacks[name];
		for (unsigned i = 0; i < eventsList.size(); ++i) {
			if (&eventsList[i] == events) {
				eventsList.erase(eventsList.begin() + i);
				break;
			}
		}
		eventContextCallbacks[ptr].erase(name);
	}

	template<typename Context>
	void EventsManager::TriggerContext(const std::string& name, Context* const context) {
		void* ptr = (void*)context;
		if (emptyContextCallbacks.find(ptr) != emptyContextCallbacks.end())
			*emptyContextCallbacks[ptr][name]();
	}

	template<typename Context>
	void EventsManager::TriggerContext(const std::string& name, Context* const context, Event* const event) {
		void* ptr = (void*)context;

		if (emptyContextCallbacks.find(ptr) != emptyContextCallbacks.end())
			(*emptyContextCallbacks[ptr][name])();

		event->name = name;
		if (eventContextCallbacks.find(ptr) != eventContextCallbacks.end())
			(*eventContextCallbacks[ptr][name])(event);

		delete event;
	}
}

#endif

