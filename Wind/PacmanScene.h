#ifndef PACMAN_SCENE_H
#define PACMAN_SCENE_H

#include "Scene.h"

#include "Ghost.h"

#include <Math/Vectors.hpp>
#include <Events/Event.h>

class PacmanScene : public Scene {
	
	bool chasing;
	float bounceTime;

	vec2f mapSize;
	vec2i mapOffset;
	bool *wallData;

public:

	PacmanScene();
	~PacmanScene() override;

	void Awake() override;

	void FixedUpdate(const float& dt) override;
	
private:

	// event handlers

	void KeyHandler(Events::Event* event);
	void MapHandler(Events::Event* event);

	// Helper methods

	void ReadWallData(const char* filepath);

	Ghost* const SpawnGhost(const std::string& name, const vec2i& tilePosition, const vec2f& dock, const vec2f& corner, const vec2f& start);
	Entity* const SpawnPacman();

};

#endif
