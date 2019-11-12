#ifndef PACMAN_SCENE_H
#define PACMAN_SCENE_H

#include "Scene.h"

#include "Ghost.h"

#include <Math/Vectors.hpp>
#include <Events/Event.h>

class PacmanScene : public Scene {
	
	bool chasing;
	float bounceTime;

	struct PalletData {
		bool isTaken;
		Entity* object;
	};

	vec2f mapSize;
	vec2i mapOffset;
	bool *wallData;
	PalletData *palletData;
	vec2f powerPosition[4];

public:

	PacmanScene();
	~PacmanScene() override;

	void Awake() override;

	void FixedUpdate(const float& dt) override;
	
private:

	// event handlers

	void KeyHandler(Events::Event* event);
	void MapWallHandler(Events::Event* event);
	void MapPalletHandler(Events::Event* event);

	// Helper methods

	void ReadMapData(const char* filepath);

	Ghost* const SpawnGhost(const std::string& name, const vec2i& tilePosition, const vec2f& dock, const vec2f& corner, const vec2f& start);
	Entity* const SpawnPacman();
	Entity* const SpawnPallet(const float& x, const float& y);
	Entity* const SpawnPower(const float& x, const float& y);

};

#endif
