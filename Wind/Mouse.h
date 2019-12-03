#ifndef MOUSE_H
#define MOUSE_H

#include "Maze.h"

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

class Mouse {

	vec2i directions[4];

	vec2i position;

	Maze* maze;
	std::vector<unsigned> vision;

public:
	
	Mouse();

	void Reset(const vec2i& _position, const unsigned& mapSize);
	void Explore(Maze* _maze);
	void Goto(const vec2i& target);

	const vec2i& GetPosition() const;
	const std::vector<unsigned>& GetVision() const;

private:

	void Scan(const vec2i& curr);

	std::vector<vec2i> GetPath(DNode* end);

};

#endif