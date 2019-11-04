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

	struct NearestEntityWithTag : Event {
		Entity ** const entityRef;
		const std::string tag;

		const vec3f position;

		NearestEntityWithTag(Entity ** const entityRef, const std::string& tag, const vec3f& position)
			: entityRef(entityRef)
			, tag(tag) 
			, position(position) {}
	};


}
