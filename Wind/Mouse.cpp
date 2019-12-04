#include "Mouse.h"

#include "Transform.h"

#include <Events/EventsManager.h>

DNode::DNode() 
	: previous(nullptr)
	, g(0.f)
	, h(0.f)
	, position(-1.f) {}

float DNode::f() const {
	return g + h;
}

bool DNode::operator<(const DNode & rhs) const {
	return position.x < rhs.position.x || position.y < rhs.position.y;
}

Mouse::Mouse() {
	directions[0] = vec2i(1, 0);
	directions[1] = vec2i(0, 1);
	directions[2] = vec2i(-1, 0);
	directions[3] = vec2i(0, -1);
}

void Mouse::Init(const vec2i & _position, Maze* const maze) {
	position = _position;
	vision.resize(maze->GetSize() * maze->GetSize(), FOG);
}

void Mouse::Explore() {
	Scan(GetMapPosition());
}

void Mouse::Goto(const vec2i & target) {
	std::vector<DNode*> opened;
	std::vector<DNode*> closed;

	DNode* current = new DNode;
	current->g = 0;
	current->h = Math::LengthSquared<float>(target - position);
	current->position = position;

	opened.push_back(current);

	while (!opened.empty()) {
		DNode* best = opened[0];
		opened.erase(opened.begin());
		closed.push_back(best);
		Events::EventsManager::GetInstance()->Trigger("DRAW_PATH", new Events::AnyType<std::vector<vec2i>>(GetPath(closed.back())));

		if (best->position == target)
			break;

		for (auto& dir : directions) {
			const vec2i pos = dir + best->position;

			if (maze->IsBlocked(pos)) continue;

			bool found = false;
			for (auto& node : closed) {
				if (node->position == pos) {
					found = true;
					break;
				}
			}

			if (found) continue;
	
			unsigned i = 0;
			for (; i < opened.size(); ++i) {
				if (opened[i]->position == pos) {
					break;
				}
			}

			DNode* neighbour = new DNode;
			neighbour->g = best->g + 1;
			neighbour->h = Math::LengthSquared<float>(pos - target);
			neighbour->position = pos;
			neighbour->previous = best;
	
			if (i != opened.size()) {
				if (opened[i]->g > neighbour->g) {
					delete opened[i];
					opened[i] = neighbour;
				} else {
					delete neighbour;
				}
			} else {
				i = 0;
				for (; i < opened.size(); ++i) {
					if (opened[i]->f() > neighbour->f()) {
						opened.insert(opened.begin() + i, neighbour);
						break;
					}
				}

				if (i == opened.size()) {
					opened.push_back(neighbour);
				}
			}
		}
	}

}

vec2i Mouse::GetMapPosition() {
	auto translation = GetComponent<Transform>()->GetWorldTranslation();
	const int halfSize = static_cast<int>(maze->GetSize()) / 2; 
	
	return vec2i(
		static_cast<int>(translation.x) + halfSize,
		static_cast<int>(translation.y) + halfSize
	);
}

void Mouse::SetMapPosition(const vec2i & position) {
	const float halfSize = static_cast<float>(maze->GetSize()) * 0.5f; 
	GetComponent<Transform>()->translation.Set(
		static_cast<float>(position.x) + halfSize,
		static_cast<float>(position.y) + halfSize,
		0.f
	);
}

const std::vector<unsigned>& Mouse::GetVision() const {
	return vision;
}

void Mouse::Scan(const vec2i & curr) {
	const int index = maze->GetMapIndex(curr);
	if (index < 0 || vision[index] != FOG) return;

	//visited[index] = true;
	const bool isBlocked = maze->IsBlocked(index);
	if (isBlocked) {
		vision[index] = WALL;
		return;
	}
	vision[index] = PATH;
	 
	for (auto& dir : directions) {
		Scan(curr + dir);
	}
}

std::vector<vec2i> Mouse::GetPath(DNode * end) {
	std::vector<vec2i> path;
	while (end->previous) {
		path.insert(path.begin(), end->position);
		end = end->previous;
	}

	return path;
}


