#ifndef MAP_EVENTS_H
#define MAP_EVENTS_H

#include <Events/Event.h>
#include <Math/Vectors.hpp>

namespace Events {
	
	struct MapData : Event {
		bool* const ref;
		const vec2i position;

		MapData(bool* const ref, const vec2i& position)
			: ref(ref)
			, position(position) {}
	};

}

#endif
