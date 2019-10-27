#include "AIScene.h"

// game objects
#include "Sprite.h"
#include "FPSLabel.h"
#include "CameraObject.h"
#include "Player.h"
#include "UILabel.h"
// components
#include "Transform.h"
#include "Render.h"
#include "Text.h"
#include "Camera.h"
#include "Script.h"
// systems
#include "RenderSystem.h"
#include "ScriptSystem.h"
// Utils
#include "LoadTGA.h"
#include "LoadFNT.h"

AIScene::AIScene() {
	components->Subscribe<Transform>(10, 1);
	components->Subscribe<Render>(10, 1);
	components->Subscribe<Text>(10, 1);
	components->Subscribe<Camera>(1, 1);
	components->Subscribe<Script>(10, 1);
	
	entities->Subscribe<Sprite>(10, 1);
	entities->Subscribe<FPSLabel>(1, 1);
	entities->Subscribe<CameraObject>(1, 1);
	entities->Subscribe<Player>(1, 1);
	entities->Subscribe<UILabel>(10, 1);

	systems->Subscribe<RenderSystem>();
	systems->Subscribe<ScriptSystem>();
}

void AIScene::Awake() {
	auto cam = entities->Create<CameraObject>();
	cam->GetComponent<Camera>()->clearColor.Set(1.f);

	// most performant : 500 - 700 FPS
	auto fps = entities->Create<FPSLabel>();
	fps->GetComponent<Transform>()->translation.Set(5.f, 0.f, 0.f);
	fps->GetComponent<Render>()->tint.Set(0.f, 0.f, 0.f, 1.f);
	fps->GetComponent<Text>()->SetFont(Load::FNT("Files/Fonts/Microsoft.fnt", "Files/Fonts/Microsoft.tga"));
	fps->GetComponent<Text>()->text = "60";
	fps->GetComponent<Text>()->scale = 0.25f;
	fps->GetComponent<Text>()->paragraphAlignment = PARAGRAPH_RIGHT;
	fps->GetComponent<Text>()->verticalAlignment = ALIGN_BOTTOM;

	auto player = entities->Create<Player>();
	player->GetComponent<Transform>()->scale.Set(0.5f);
	player->GetComponent<Render>()->tint.Set(1.f, 0.f, 0.f, 1.f);

	auto healthBar = entities->Create<Sprite>();
	healthBar->GetComponent<Transform>()->translation.Set(0.f, 1.f, 0.f);
	healthBar->GetComponent<Transform>()->scale.Set(1.f, 0.2f, 1.f);
	healthBar->GetComponent<Render>()->tint.Set(0.f, 1.f, 0.f, 1.f);
	healthBar->SetParent(player);

	auto statsView = entities->Create<Sprite>();
	statsView->GetComponent<Transform>()->translation.Set(3.f, 0.f, 0.f);
	statsView->GetComponent<Transform>()->scale.Set(2.f, 3.f, 1.f);
	statsView->GetComponent<Render>()->tint.Set(vec3f(0.1f), 0.8f);
	statsView->SetParent(player);

	auto title = entities->Create<UILabel>();
	title->GetComponent<Transform>()->translation.Set(0.f, 1.f, 0.f);
	title->GetComponent<Transform>()->scale.Set(1.5f, 1.f, 1.f);
	title->GetComponent<Text>()->SetFont(Load::FNT("Files/Fonts/Microsoft.fnt", "Files/Fonts/Microsoft.tga"));
	title->GetComponent<Text>()->text = "Player";
	title->GetComponent<Text>()->scale = 0.25f;
	title->GetComponent<Text>()->paragraphAlignment = PARAGRAPH_LEFT;
	title->GetComponent<Text>()->verticalAlignment = ALIGN_MIDDLE;
	title->SetParent(statsView);

}
