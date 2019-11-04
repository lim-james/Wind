#ifndef AI_SCENE_H
#define AI_SCENE_H

#include "Scene.h"

#include <Events/Event.h>

class AIScene : public Scene {
	
	int mapHalfSize;

public:

	AIScene();

	void Awake() override;

private:

	void KeyHandler(Events::Event* event);
	void EntityHanlder(Events::Event* event);

};

#endif