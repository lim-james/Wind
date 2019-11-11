#ifndef ENTITY_EVENTS_H
#define ENTITY_EVENTS_H

#include "Entity.h"

#include <Events/Event.h>
#include <Math/Vectors.hpp>
#include <MACROS.h>

namespace Events {

	struct CreateEntity : Event {
		const unsigned hash;
		Entity * const entity;

		CreateEntity(const unsigned& hash, Entity * const entity)
			: hash(hash)
			, entity(entity) {}
	};

	template<typename EntityType>
	struct CreateAnyEntity : CreateEntity {
		CreateAnyEntity() : CreateEntity(
			hashof(EntityType),
			new EntityType
		) {}
	};

	struct FindEntityWithTag : Event {
		Entity ** const entityRef;
		const std::string tag;
		// optional 
		const std::string state;

		FindEntityWithTag(Entity ** const entityRef, const std::string& tag, const std::string& state = "")
			: entityRef(entityRef)
			, tag(tag) 
			, state(state) {}
	};

	struct NearestEntityWithTag : Event {
		Entity ** const entityRef;
		const std::string tag;
		const vec3f position;
		// optional
		const std::string state;


		NearestEntityWithTag(Entity ** const entityRef, const std::string& tag, const vec3f& position, const std::string& state = "")
			: entityRef(entityRef)
			, tag(tag) 
			, position(position)
			, state(state) {}
	};

}

#endif
