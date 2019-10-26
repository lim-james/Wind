#ifndef CAMERA_OBJECT_H
#define CAMERA_OBJECT_H

#include "InputEvents.h"
#include "Entity.h"

#include <map>

class CameraObject : public Entity {

public:

	virtual void Build();
	virtual void Initialize();

};

#endif