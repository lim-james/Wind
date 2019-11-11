#ifndef AI_SCENE_H
#define AI_SCENE_H

#include "Scene.h"

#include <Events/Event.h>

class AIScene : public Scene {
	
	int mapHalfSize;
	float bounceTime;

public:

	AIScene();

	void Awake() override;

	void FixedUpdate(const float& dt) override;
	
private:

	// event handlers

	void KeyHandler(Events::Event* event);

	// helper methods

	void SpawnFood();
	void SpawnFish();
	void SpawnShark();

};

#endif