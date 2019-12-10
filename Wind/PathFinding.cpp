#include "PathFinding.h"

// game objects
#include "Sprite.h"
#include "FPSLabel.h"
#include "UILabel.h"
#include "UIButton.h"
#include "CameraObject.h"
// components
#include "Animation.h"
#include "Transform.h"
#include "Render.h"
#include "Text.h"
#include "Camera.h"
#include "Button.h"
#include "Script.h"
// systems
#include "AnimationSystem.h"
#include "RenderSystem.h"
#include "ButtonSystem.h"
#include "ScriptSystem.h"
// utils
#include "LoadTGA.h"
#include "LoadFNT.h"
// events
#include "Line.h"

#include <Events/EventsManager.h>
#include <GLFW/glfw3.h>

PathFinding::PathFinding() {
	gridSize = 12;
	iterations = 0;

	grid.resize(gridSize * gridSize, nullptr);
}

void PathFinding::Awake() {
	Scene::Awake();

	const unsigned total = gridSize * gridSize;

	components->Subscribe<Transform>(total, 1);
	components->Subscribe<Animation>(total, 1);
	components->Subscribe<Render>(total, 1);
	components->Subscribe<Text>(10, 1);
	components->Subscribe<Camera>(1, 1);
	components->Subscribe<Button>(total, 1);
	components->Subscribe<Script>(total, 1);

	entities->Subscribe<UIButton>(total, 1);
	entities->Subscribe<FPSLabel>(1, 1);
	entities->Subscribe<CameraObject>(1, 1);
	entities->Subscribe<UILabel>(10, 1);
	entities->Subscribe<Mouse>(4, 1);

	systems->Subscribe<AnimationSystem>();
	systems->Subscribe<RenderSystem>();
	systems->Subscribe<ButtonSystem>();
	systems->Subscribe<ScriptSystem>();
}

void PathFinding::Reset() {
	Scene::Reset();
	Events::EventsManager::GetInstance()->Subscribe("UPDATE_VISION", &PathFinding::UpdateVision, this);
	Events::EventsManager::GetInstance()->Subscribe("DRAW_PATH", &PathFinding::DrawPath, this);
	Events::EventsManager::GetInstance()->Subscribe("KEY_INPUT", &PathFinding::KeyHandler, this);
}

void PathFinding::Start() {
	Scene::Start();

	auto cam = entities->Create<CameraObject>();
	cam->GetComponent<Camera>()->clearColor.Set(0.f);
	cam->GetComponent<Camera>()->SetSize(static_cast<float>(gridSize));

	//auto grid = entities->Create<Sprite>();
	//grid->GetComponent<Transform>()->scale.Set(static_cast<float>(gridSize));
	//grid->GetComponent<Render>()->SetTexture(Load::TGA("Files/Textures/chessboard.tga"));

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

	// float grid size
	const float fgs = static_cast<float>(gridSize) * 0.5f;
	maze.Generate(0, gridSize, vec2i(0), 0.3f);

	mouse = entities->Create<Mouse>();
	mouse->SetMaze(&maze);
	mouse->SetMapPosition(0.f);
	mouse->GetComponent<Transform>()->scale.Set(1.f);
	mouse->GetComponent<Render>()->tint.Set(0.01f);

	for (float x = -fgs; x < fgs; ++x) {
		for (float y = -fgs; y < fgs; ++y) {
			auto tile = entities->Create<UIButton>();
			tile->GetComponent<Transform>()->translation.Set(static_cast<float>(x), static_cast<float>(y), 0.f);
			tile->GetComponent<Transform>()->scale.Set(0.9f);

			const int index = maze.GetMapIndex(static_cast<int>(x + fgs), static_cast<int>(y + fgs));

			switch (maze.GetMapData(index)) {
			case WALL:
				tile->GetComponent<Render>()->tint.Set(1.f, 0.f, 0.f, 0.5f);
				break;
			case FOG:
				tile->GetComponent<Render>()->tint.Set(0.f);
				break;
			case PATH:
				tile->GetComponent<Render>()->tint.Set(1.f, 1.f, 1.f, 0.5f);
				break;
			default:
				break;
			}

			tile->GetComponent<Button>()->BindHandler(&PathFinding::OnMouseOverHandler, this, MOUSE_OVER);
			tile->GetComponent<Button>()->BindHandler(&PathFinding::OnMouseOutHandler, this, MOUSE_OUT);
			tile->GetComponent<Button>()->BindHandler(&PathFinding::OnMouseDownHandler, this, MOUSE_DOWN);
			tile->GetComponent<Button>()->BindHandler(&PathFinding::OnMouseUpHandler, this, MOUSE_UP);
			tile->GetComponent<Button>()->BindHandler(&PathFinding::OnClick, this, MOUSE_CLICK);


			grid[index] = tile;
		}
	}

	mouse->Explore();
}

void PathFinding::UpdateVision() {
	auto vision = mouse->GetVision();
	auto index = maze.GetMapIndex(mouse->GetMapPosition());

	for (unsigned i = 0; i < vision.size(); ++i) {
		if (i == index) {
			grid[i]->GetComponent<Render>()->tint.Set(0.f, 1.f, 0.f, 0.5f);
			continue;
		}

		switch (vision[i]) {
		case WALL:
			grid[i]->GetComponent<Render>()->tint.Set(1.f, 0.f, 0.f, 0.5f);
			break;
		case FOG:
			grid[i]->GetComponent<Render>()->tint.Set(0.f);
			break;
		case PATH:
			grid[i]->GetComponent<Render>()->tint.Set(1.f, 1.f, 1.f, 0.5f);
			break;
		default:
			break;
		}
	}
}

void PathFinding::DrawPath(Events::Event * event) {
	auto path = static_cast<Events::AnyType<std::vector<vec2i>>*>(event)->data;
	UpdateVision();

	for (auto& step : path) {
		const auto i = maze.GetMapIndex(step);
		if (i < 0) break;
		grid[i]->GetComponent<Render>()->tint.Set(0.f, 1.f, 1.f, 1.f);
	}
	Events::EventsManager::GetInstance()->Trigger("STEP");
	Sleep(20);
}

void PathFinding::KeyHandler(Events::Event * event) {
	const auto keyInput = static_cast<Events::KeyInput* >(event);
	if (keyInput->key == GLFW_KEY_SPACE && keyInput->action == GLFW_PRESS) {
		
	}
}

void PathFinding::OnMouseOverHandler(Entity * entity) {
	entity->GetComponent<Animation>()->Animate(
		AnimationBase(false, 0.2f),
		entity->GetComponent<Render>()->tint.a,
		1.f
	);
}

void PathFinding::OnMouseOutHandler(Entity * entity) {
	entity->GetComponent<Animation>()->Animate(
		AnimationBase(false, 0.2f),
		entity->GetComponent<Render>()->tint.a,
		0.5f
	);
}

void PathFinding::OnMouseDownHandler(Entity * entity) {
	entity->GetComponent<Animation>()->Animate(
		AnimationBase(false, 0.2f),
		entity->GetComponent<Transform>()->scale,
		vec3f(0.5f)
	);
}

void PathFinding::OnMouseUpHandler(Entity * entity) {
	entity->GetComponent<Animation>()->Animate(
		AnimationBase(false, 0.2f),
		entity->GetComponent<Transform>()->scale,
		vec3f(0.9f)
	);
}

void PathFinding::OnClick(Entity * entity) {
	const vec3f screenSpace = entity->GetComponent<Transform>()->translation;
	const vec2i mapPosition(
		static_cast<int>(screenSpace.x) + gridSize / 2,
		static_cast<int>(screenSpace.y) + gridSize / 2
	);
	const int index = maze.GetMapIndex(mapPosition);

	if (index < 0) return;

	mouse->Goto(mapPosition);
}

