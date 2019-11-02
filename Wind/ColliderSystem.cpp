#include "ColliderSystem.h"

#include "Entity.h"
#include "Transform.h"

#include <Events/EventsManager.h>
#include <MACROS.h>

ColliderSystem::ColliderSystem() {
	Events::EventsManager::GetInstance()->Subscribe("COLLIDER_ACTIVE", &ColliderSystem::ActiveHandler, this);
}

ColliderSystem::~ColliderSystem() {
	components.clear();
}

void ColliderSystem::Update(const float& dt) {}

void ColliderSystem::FixedUpdate(const float& dt) {
	for (auto it1 = components.begin(); it1 < components.end(); ++it1) {
		auto c1 = static_cast<Collider*>(*it1);
		auto p1 = c1->GetParent();
		auto t1 = p1->GetComponent<Transform>();
		
		const vec2f position1 = t1->GetWorldTranslation();
		vec2f halfSize1 = t1->scale * 0.5f;
		halfSize1.y = -halfSize1.y;
		const vec2f min1 = position1 - halfSize1;
		const vec2f max1 = position1 + halfSize1;

		for (auto it2 = it1 + 1; it2 < components.end(); ++it2) {
			auto c2 = static_cast<Collider*>(*it2);
			auto p2 = c2->GetParent();
			auto t2 = p2->GetComponent<Transform>();

			const vec2f position2 = t2->GetWorldTranslation();
			vec2f halfSize2 = t2->scale * 0.5f;
			halfSize2.y = -halfSize2.y;
			const vec2f min2 = position2 - halfSize2;
			const vec2f max2 = position2 + halfSize2;

			if (!( min1.x > max2.x  // right
				|| max1.x < min2.x  // left
				|| min1.y < max2.y  // top
				|| max1.y > min2.y)) { // down
				if (history[c1][c2] && history[c2][c1]) {
					c1->OnCollisionStay(p2);
					c2->OnCollisionStay(p1);
				} else {
					c1->OnCollisionEnter(p2);
					c2->OnCollisionEnter(p1);
					history[c1][c2] = history[c2][c1] = true;
				}
			} else {
				if (history[c1][c2] || history[c2][c1]) {
					c1->OnCollisionExit(p2);
					c2->OnCollisionExit(p1);
					history[c1][c2] = history[c2][c1] = false;
				} 
			}
		}
	}
}

void ColliderSystem::ActiveHandler(Events::Event* event) {
	const auto c = static_cast<Events::AnyType<Collider*>*>(event)->data;

	if (c->IsActive()) {
		components.push_back(c);
	} else {
		components.erase(vfind(components, c));
		for (auto& pair : history[c]) {
			pair.second = false;
		}
	}
}