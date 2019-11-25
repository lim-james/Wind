#ifndef QUEEN_PROBLEM_H
#define QUEEN_PROBLEM_H

#include "Scene.h"

class QueenProblem : public Scene {

	int gridSize;
	int iterations;
	int solutions;

	int knightPaths[8];
	Entity* queenObjects[8];

public:

	QueenProblem();

	void Awake() override;
	void Start() override;
	
private:

	void KeyHandler(Events::Event* event);

	void Solve8Queen(const int& column, std::vector<int> list, std::vector<int> avail);
	void UpdateQueens(std::vector<int> list);

	bool IsBoardCompleted(std::vector<bool>& visited);
	bool IsKnightValid(int& position, std::vector<bool>& visited);
	void KnightTour(int depth, std::vector<int> positions, std::vector<bool> visited);

 
};

/*
	
	0 1 2 3 4 5 6 7

	0 -> avail = 1 ... 7 -> it 2 ... 7

	3 -> avail = 0...2 & 4...7 -> it 0...1 & 5...7


	0 1 2 3 4
	1   X (2, 1)
	2
	X (0,3)
	4

*/

#endif
