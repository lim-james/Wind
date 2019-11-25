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
// events
#include "Line.h"

#include <Events/EventsManager.h>

QueenProblem::QueenProblem() {
	gridSize = 8;
	iterations = 0;
	solutions = 0;

	knightPaths[0] = vec2i(2, 1);
	knightPaths[1] = vec2i(1, 2);
	knightPaths[2] = vec2i(-1, 2);
	knightPaths[3] = vec2i(-2, 1);
	knightPaths[4] = vec2i(-2, -1);
	knightPaths[5] = vec2i(-1, -2);
	knightPaths[6] = vec2i(1, -2);
	knightPaths[7] = vec2i(2, -1);
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
	cam->GetComponent<Camera>()->SetSize(static_cast<float>(gridSize));

	auto grid = entities->Create<Sprite>();
	grid->GetComponent<Transform>()->scale.Set(static_cast<float>(gridSize));
	grid->GetComponent<Render>()->SetTexture(Load::TGA("Files/Textures/chessboard.tga"));

	// most performant : 500 - 700 FPS
	auto fps = entities->Create<FPSLabel>();
	fps->GetComponent<Transform>()->translation.Set(static_cast<float>(gridSize), 0.f, 0.f);
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
		queen->GetComponent<Render>()->SetTexture(Load::TGA("Files/Textures/queen.tga"));
		queenObjects[i] = queen;
	}

	//knight = entities->Create<Sprite>();
	//knight->GetComponent<Transform>()->translation.Set(0.5f, 0.5f, 0.5f);
	//knight->GetComponent<Render>()->SetTexture(Load::TGA("Files/Textures/knight.tga"));

	std::vector<int> list(gridSize, -1);
	std::vector<int> avail(gridSize);
	for (int i = 0; i < gridSize; ++i)
		avail[i] = i;

	Solve8Queen(0, list, avail);
	std::cout << "Iterations : " << iterations << '\n';
	std::cout << "Solutions  : " << solutions << '\n';

	//std::vector<bool> visited(gridSize * gridSize, false);
	//visited[0] = true;
	//KnightTour(0, { vec2i(0) }, visited);
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
		Sleep(1000);
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

				for (int c = 0; c < column - 1; ++c) {
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


int QueenProblem::GetMapIndex(const vec2i& position) {
	if (position.x < 0 || position.x >= gridSize ||
		position.y < 0 || position.y >= gridSize)
		return -1;

	return position.x + position.y * gridSize;
}

bool QueenProblem::IsBoardCompleted(std::vector<bool>& visited) {
	for (unsigned i = 0; i < visited.size(); ++i)
		if (!visited[i]) return false;

	return true;
}

bool QueenProblem::IsKnightValid(const int& index, std::vector<bool>& visited) {
	return index >= 0 && !visited[index];
}

void QueenProblem::KnightTour(int depth, std::vector<vec2i> positions, std::vector<bool> visited) {
	UpdateKnight(positions.back());

	if (IsBoardCompleted(visited)) {
		++solutions;
		std::cout << "Tour : ";
		auto previous = positions.front();
		bool first = true;

		const vec2i offset(gridSize * 0.5f);

		for (const auto& pos : positions) {
			std::cout << GetMapIndex(pos) + 1 << ' ';
			if (first) {
				first = false;
			} else {
				Line line;
				line.Set(previous - offset, pos - offset);
				line.tint.Set(1.f);
				Events::EventsManager::GetInstance()->Trigger("DRAW_LINE", new Events::AnyType<Line>(line));
				previous = pos;
			}
		}
		std::cout << '\n';
		Events::EventsManager::GetInstance()->Trigger("STEP");
		Sleep(1000);
		return;
	}

	Events::EventsManager::GetInstance()->Trigger("STEP");

	++iterations;
	for (const auto& path : knightPaths) {
		const auto newPosition = positions.back() + path;
		const auto positionIndex = GetMapIndex(newPosition);
		if (IsKnightValid(positionIndex, visited)) {
			auto copy = visited;
			copy[positionIndex] = true;

			auto positionsCopy = positions;
			positionsCopy.push_back(newPosition);
			KnightTour(depth + 1, positionsCopy, copy);
		}
	}
}

void QueenProblem::UpdateKnight(const vec2i & position) {
	const float offset = static_cast<float>(gridSize) * 0.5f;
	const float x = static_cast<float>(position.x) - offset;
	const float y = static_cast<float>(position.y) - offset;
	knight->GetComponent<Transform>()->translation.Set(x + 0.5f, y + 0.5f, 0.5f);
}


