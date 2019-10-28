#include "Entity.h"

#include <Events/Event.h>
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

}
