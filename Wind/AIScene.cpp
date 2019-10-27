#include "AIScene.h"

// game objects
#include "Sprite.h"
#include "CameraObject.h"
// components
#include "Transform.h"
#include "Render.h"
#include "Text.h"
#include "Camera.h"
// systems
#include "RenderSystem.h"
// Utils
#include "LoadTGA.h"
#include "LoadFNT.h"

AIScene::AIScene() {
	components->Subscribe<Transform>(10, 1);
	components->Subscribe<Render>(10, 1);
	components->Subscribe<Text>(10, 1);
	components->Subscribe<Camera>(1, 1);
	
	entities->Subscribe<Sprite>(10, 1);
	entities->Subscribe<CameraObject>(1, 1);

	systems->Subscribe<RenderSystem>();
}

void AIScene::Awake() {
	entities->Create<CameraObject>();

	// most performant : 500 - 700 FPS
	auto sprite = entities->Create<Sprite>();
	sprite->GetComponent<Transform>()->scale.Set(5.f, 5.f, 1.f);
	sprite->GetComponent<Render>()->texture = Load::TGA("Files/Textures/container.tga");
	sprite->GetComponent<Text>()->SetFont(Load::FNT("Files/Fonts/Microsoft.fnt", "Files/Fonts/Microsoft.tga"));
	sprite->GetComponent<Text>()->text = "Wind\nEngine";
	sprite->GetComponent<Text>()->scale = 0.5f;
	sprite->GetComponent<Text>()->paragraphAlignment = PARAGRAPH_RIGHT;
	sprite->GetComponent<Text>()->verticalAlignment = ALIGN_BOTTOM;
}
