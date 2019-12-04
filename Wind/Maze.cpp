#include "Maze.h"

void Maze::Generate(const unsigned& seed, const unsigned & _size, const vec2i & start, const float & wallLoad) {
	grid.clear();
	size = _size;
	if (size == 0)
		return;

	const unsigned total = size * size;
	grid.resize(total, PATH);
	srand(seed);

	const unsigned startIndex = GetMapIndex(start);

	for (unsigned i = 0; i < static_cast<unsigned>(total * wallLoad);) {
		unsigned chosen = rand() % total;
		if (chosen == startIndex)
			continue;

		if (grid[chosen] == PATH) {
			grid[chosen] = WALL;
			++i;
		}
	}
}

const unsigned & Maze::GetSize() const {
	return size;
}

bool Maze::IsBlocked(const vec2i & position) const {
	const int index = GetMapIndex(position);
	return index < 0 || grid[index] == WALL;
}

bool Maze::IsBlocked(const int& index) const {
	return index < 0 || grid[index] == WALL;
}

int Maze::GetMapData(const vec2i & position) const {
	const int index = GetMapIndex(position);
	if (index < 0) return WALL;
	return grid[index];
}

int Maze::GetMapData(const int & x, const int & y) const {
	const int index = GetMapIndex(x, y);
	if (index < 0) return WALL;
	return grid[index];
}

int Maze::GetMapData(const int & index) const {
	return grid[index];
}

int Maze::GetMapIndex(const vec2i & position) const {
	const int i = static_cast<int>(size);

	if (position.x < 0 || position.x >= i ||
		position.y < 0 || position.y >= i)
		return -1;

	return position.x + position.y * i;
}

int Maze::GetMapIndex(const int & x, const int & y) const {
	const int i = static_cast<int>(size);

	if (x < 0 || x >= i ||
		y < 0 || y >= i)
		return -1;

	return x + y * i;
}
