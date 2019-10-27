#include "AIScene.h"

// game objects
#include "Sprite.h"
#include "FPSLabel.h"
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

	systems->Subscribe<RenderSystem>();
	systems->Subscribe<ScriptSystem>();
}

void AIScene::Awake() {
	auto cam = entities->Create<CameraObject>();
	cam->GetComponent<Camera>()->clearColor.Set(1.f);

	// most performant : 500 - 700 FPS
	auto fps = entities->Create<FPSLabel>();
	fps->GetComponent<Render>()->tint.Set(0.f, 0.f, 0.f, 1.f);
	fps->GetComponent<Text>()->SetFont(Load::FNT("Files/Fonts/Microsoft.fnt", "Files/Fonts/Microsoft.tga"));
	fps->GetComponent<Text>()->text = "60";
	fps->GetComponent<Text>()->scale = 0.25f;
	fps->GetComponent<Text>()->paragraphAlignment = PARAGRAPH_RIGHT;
	fps->GetComponent<Text>()->verticalAlignment = ALIGN_BOTTOM;
}