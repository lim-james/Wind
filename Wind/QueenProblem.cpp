#include "QueenProblem.h"

// game objects
#include "Sprite.h"
#include "FPSLabel.h"
#include "UILabel.h"
#include "CameraObject.h"
// components
#include "Transform.h"
#include "Render.h"
#include "Text.h"
#include "Camera.h"
#include "Script.h"
// systems
#include "RenderSystem.h"
#include "ScriptSystem.h"
// utils
#include "LoadTGA.h"
#include "LoadFNT.h"

#include <Events/EventsManager.h>

QueenProblem::QueenProblem() {
	gridSize = 8;
	iterations = 0;
	solutions = 0;

	vec2i nightMovement[] = {
		vec2i( 2, 1),
		vec2i( 1, 2),
		vec2i(-1, 2),
		vec2i(-2, 1),
		vec2i(-2, -1),
		vec2i(-1, -2),
		vec2i( 1, -2),
		vec2i( 2, -1)
	};

	for (unsigned i = 0; i < 8; ++i) {
		knightPaths[i] = nightMovement[i].y * gridSize + nightMovement[i].x;
	}
}

void QueenProblem::Awake() {
	Scene::Awake();

	Events::EventsManager::GetInstance()->Subscribe("KEY_INPUT", &QueenProblem::KeyHandler, this);

	components->Subscribe<Transform>(10, 1);
	components->Subscribe<Render>(10, 1);
	components->Subscribe<Text>(10, 1);
	components->Subscribe<Camera>(1, 1);
	components->Subscribe<Script>(1, 1);

	entities->Subscribe<Sprite>(100, 1);
	entities->Subscribe<FPSLabel>(1, 1);
	entities->Subscribe<CameraObject>(1, 1);
	entities->Subscribe<UILabel>(10, 1);

	systems->Subscribe<RenderSystem>();
	systems->Subscribe<ScriptSystem>();
}

void QueenProblem::Start() {
	Scene::Start();

	auto cam = entities->Create<CameraObject>();
	cam->GetComponent<Camera>()->clearColor.Set(0.f);
	cam->GetComponent<Camera>()->SetSize(gridSize);

	auto grid = entities->Create<Sprite>();
	grid->GetComponent<Transform>()->scale.Set(gridSize);
	grid->GetComponent<Render>()->SetTexture(Load::TGA("Files/Textures/chessboard.tga"));

	// most performant : 500 - 700 FPS
	auto fps = entities->Create<FPSLabel>();
	fps->GetComponent<Transform>()->translation.Set(gridSize, 0.f, 0.f);
	fps->GetComponent<Transform>()->scale.Set(1.f);
	fps->GetComponent<Render>()->tint.Set(0.01f);
	fps->GetComponent<Text>()->SetFont(Load::FNT("Files/Fonts/Microsoft.fnt", "Files/Fonts/Microsoft.tga"));
	fps->GetComponent<Text>()->text = "60";
	fps->GetComponent<Text>()->scale = 0.5f;
	fps->GetComponent<Text>()->paragraphAlignment = PARAGRAPH_RIGHT;
	fps->GetComponent<Text>()->verticalAlignment = ALIGN_BOTTOM;

	for (int i = 0; i < gridSize; ++i) {
		auto queen = entities->Create<Sprite>();
		queen->GetComponent<Transform>()->translation.Set((i - gridSize * 0.5f) + 0.5f, (i - gridSize * 0.5f) + 0.5f, 0.5f);
		//queen->GetComponent<Transform>()->scale.Set(0.5f);
		queen->GetComponent<Render>()->SetTexture(Load::TGA("Files/Textures/queen.tga"));
		queenObjects[i] = queen;
	}

	std::vector<int> list(gridSize, -1);
	std::vector<int> avail(gridSize);
	for (int i = 0; i < gridSize; ++i)
		avail[i] = i;

	Solve8Queen(0, list, avail);
	std::cout << "Iterations : " << iterations << '\n';
	std::cout << "Solutions  : " << solutions << '\n';

	//std::vector<bool> visited(gridSize * gridSize);
	//visited[1] = true;
	//KnightTour(0, { 1 }, visited);

	//std::cout << "iterations : " << iterations << '\n';
	//std::cout << "Solutions  : " << solutions << '\n';
}

void QueenProblem::KeyHandler(Events::Event * event) {
}

void QueenProblem::Solve8Queen(const int& column, std::vector<int> list, std::vector<int> avail) {
	UpdateQueens(list);
	Events::EventsManager::GetInstance()->Trigger("STEP");
	Sleep(10);

	if (column == gridSize) {
		++solutions;
		std::cout << "Solution : ";
		for (const auto& item : list)
			std::cout << item << ' ';
		std::cout << '\n';
		return;
	}

	++iterations;
	if (column == 0) {
		for (unsigned i = 0; i < avail.size(); ++i) {
			auto copy = list;
			copy[0] = avail[i];

			auto newAvail = avail;
			newAvail.erase(newAvail.begin() + i);
			
			Solve8Queen(column + 1, copy, newAvail);
		}
	} else if (column == 1) {
		int previous = list[0];

		for (unsigned i = 0; i < avail.size(); ++i) {
			if (abs(avail[i] - previous) > 1) {
				auto copy = list;
				copy[column] = avail[i];
				auto newAvail = avail;
				newAvail.erase(newAvail.begin() + i);

				Solve8Queen(column + 1, copy, newAvail);
			}
		}
	} else {
		int previous = list[column - 1];

		for (unsigned i = 0; i < avail.size(); ++i) {
			const int nY = avail[i];

			if (abs(nY - previous) > 1) {
				auto copy = list;
				copy[column] = nY;
				auto newAvail = avail;
				newAvail.erase(newAvail.begin() + i);

				bool attacks = false;

				for (unsigned c = 0; c < column - 1; ++c) {
					const int x = static_cast<int>(c);
					const int y = list[c];
						
					if (abs(nY - y) == abs(column - x)) {
						attacks = true;
						break;
					}
				}

				if (!attacks) {
					Solve8Queen(column + 1, copy, newAvail);
				} 
			}
		}

	}

}

void QueenProblem::UpdateQueens(std::vector<int> list) {
	for (int i = 0; i < gridSize; ++i) {
		const auto y = list[i];
		queenObjects[i]->GetComponent<Transform>()->translation.Set((i - gridSize * 0.5f) + 0.5f, (y - gridSize * 0.5f) + 0.5f, 0.5f);
		queenObjects[i]->GetComponent<Render>()->tint.a = y >= 0 ? 1.f : 0.f;
	}
}


bool QueenProblem::IsBoardCompleted(std::vector<bool>& visited) {
	for (unsigned i = 0; i < visited.size(); ++i)
		if (!visited[i]) return false;

	return true;
}

bool QueenProblem::IsKnightValid(int& position, std::vector<bool>& visited) {
	return position >= 0 && position < gridSize * gridSize && !visited[position];
}

void QueenProblem::KnightTour(int depth, std::vector<int> positions, std::vector<bool> visited) {
	if (IsBoardCompleted(visited)) {
		++solutions;
		//std::cout << "Tour : ";
		//for (const auto& pos : positions) {
		//	std::cout << pos << ' ';
		//}
		//std::cout << '\n';
		return;
	}

	++iterations;
	for (const auto& path : knightPaths) {
		int newPosition = positions.back() + path;
		if (IsKnightValid(newPosition, visited)) {
			auto copy = visited;
			copy[newPosition] = true;

			auto positionsCopy = positions;
			positionsCopy.push_back(newPosition);
			KnightTour(depth + 1, positionsCopy, copy);
		}
	}
}


