#ifndef GAMEMODES_H
#define GAMEMODES_H

#include <Events/Event.h>

#define CHASE			0
#define SCATTER			1
#define FRIGHTENED		2
#define END_FRIGHTENED	3

namespace Events {
	typedef AnyType<unsigned> ModeEvent;
}

#endif
