#ifndef QUEEN_PROBLEM_H
#define QUEEN_PROBLEM_H

#include "Scene.h"
#include "Maze.h"
#include "Mouse.h"

#include <Math/Vectors.hpp>

#include <vector>

class PathFinding : public Scene {

	int gridSize;
	int iterations;

	Maze maze;
	Mouse mouse;
	std::vector<Entity*> grid;

public:

	PathFinding();

	void Awake() override;
	void Reset() override;
	void Start() override;
	
private:

	void UpdateVision();
	void DrawPath(Events::Event* event);

	void OnMouseOverHandler(Entity* entity);
	void OnMouseOutHandler(Entity* entity);
	void OnMouseDownHandler(Entity* entity);
	void OnMouseUpHandler(Entity* entity);
	void OnClick(Entity* entity);

};

#endif
