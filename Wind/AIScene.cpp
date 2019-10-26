#include "AIScene.h"

// game objects
#include "Sprite.h"
#include "CameraObject.h"
// components
#include "Transform.h"
#include "Render.h"
#include "Camera.h"
// systems
#include "RenderSystem.h"
// Utils
#include "LoadTGA.h"

AIScene::AIScene() {
	components->Subscribe<Transform>(10, 1);
	components->Subscribe<Render>(10, 1);
	components->Subscribe<Camera>(1, 1);
	
	entities->Subscribe<Sprite>(10, 1);
	entities->Subscribe<CameraObject>(1, 1);

	systems->Subscribe<RenderSystem>();
}

void AIScene::Awake() {
	entities->Create<CameraObject>();

	// most performant : 500 - 700 FPS
	for (float x = -5.f; x <= 5.f; ++x) {
		for (float y = -5.f; y <= 5.f; ++y) {
			auto sprite = entities->Create<Sprite>();
			sprite->GetComponent<Transform>()->translation.x = x;
			sprite->GetComponent<Transform>()->translation.y = y;
			sprite->GetComponent<Render>()->texture = Load::TGA("Files/Textures/container.tga");
		}
	}
}
