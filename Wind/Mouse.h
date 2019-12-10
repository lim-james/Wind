#ifndef MOUSE_H
#define MOUSE_H

#include "Maze.h"
#include "Sprite.h"

#include <Math/Vectors.hpp>

#include <vector>

struct DNode {

	DNode* previous;
	float g, h;
	vec2i position;

	DNode();

	float f() const;

	bool operator<(const DNode& rhs) const;

};

class Mouse : public Entity {

	vec2i directions[4];

	Maze* maze;
	std::vector<unsigned> vision;

public:
	
	Mouse();

	void Build() override;

	void Explore();
	void Goto(const vec2i& target);

	void SetMaze(Maze* const _maze);
	vec2i GetMapPosition();
	void SetMapPosition(const vec2i& position);

	const std::vector<unsigned>& GetVision() const;

private:

	void Scan(const vec2i& curr);

	std::vector<vec2i> GetPath(DNode* end);

};

#endif