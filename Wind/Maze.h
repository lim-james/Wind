#ifndef MAZE_H
#define MAZE_H

#include <Math/Vectors.hpp>

#include <vector>

#define WALL -1
#define FOG   0
#define PATH  1

class Maze {

	unsigned size;
	std::vector<int> grid;

public:

	void Generate(const unsigned& seed, const unsigned& _size, const vec2i& start, const float& wallLoad);

	const unsigned& GetSize() const;

	bool IsBlocked(const vec2i& position) const;
	bool IsBlocked(const int& index) const;

	int GetMapData(const vec2i& position) const;
	int GetMapData(const int& x, const int& y) const;
	int GetMapData(const int& index) const;

	int GetMapIndex(const vec2i& position) const;
	int GetMapIndex(const int& x, const int& y) const;

};

#endif
