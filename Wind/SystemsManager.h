#ifndef SYSTEMS_MANAGER_H
#define SYSTEMS_MANAGER_H

#include "System.h"

#include <MACROS.h>

#include <map>
#include <typeindex>

class SystemsManager {

	std::map<std::type_index, System*> systems;

public:

	~SystemsManager();

	template<typename SystemType>
	const bool Has() const {
		return systems.find(indexof(SystemType)) != systems.end();
	}

	template<typename SystemType>
	void Subscribe() {
		if (Has<SystemType>()) return;

		systems[indexof(SystemType)] = new SystemType;
	}

	template<typename SystemType>
	SystemType* const Get() {
		return dynamic_cast<SystemType*>(systems[indexof(SystemType)]);
	}

	void Initialize();
	void Update(const float& t);

};

#endif