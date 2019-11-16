#include "PacmanScene.h"

// game objects
#include "Sprite.h"
#include "FPSLabel.h"
#include "CameraObject.h"
#include "Player.h"
#include "UILabel.h"
#include "ParticleObject.h"
#include "AISprite.h"
#include "Ghost.h"
#include "Pacman.h"
#include "PowerPallet.h"
// components
#include "Transform.h"
#include "Animation.h"
#include "Render.h"
#include "Text.h"
#include "Camera.h"
#include "Particle.h"
#include "ParticleEmitter.h"
#include "Collider.h"
#include "Script.h"
// systems
#include "AnimationSystem.h"
#include "RenderSystem.h"
#include "ParticleSystem.h"
#include "ColliderSystem.h"
#include "ScriptSystem.h"
#include "StateMachine.h"
// States
#include "GhostStates.h"
#include "GhostChaseStates.h"
#include "PacmanStates.h"
// Utils
#include "LoadTGA.h"
#include "LoadFNT.h"

#include "InputEvents.h"
#include "MapEvents.h"

#include "GameModes.h"

#include <Math/Random.hpp>
#include <Events/EventsManager.h>
#include <GLFW/glfw3.h>

#include <fstream>

PacmanScene::PacmanScene() {
	components->Subscribe<Transform>(10, 1);
	components->Subscribe<Animation>(10, 1);
	components->Subscribe<Render>(10, 1);
	components->Subscribe<Text>(10, 1);
	components->Subscribe<Camera>(1, 1);
	components->Subscribe<ParticleEmitter>(10, 1);
	components->Subscribe<Particle>(100, 1);
	components->Subscribe<Collider>(10, 1);
	components->Subscribe<Script>(10, 1);
	components->Subscribe<StateContainer>(10, 1);

	entities->Subscribe<Sprite>(100, 1);
	entities->Subscribe<FPSLabel>(1, 1);
	entities->Subscribe<CameraObject>(1, 1);
	entities->Subscribe<Player>(1, 1);
	entities->Subscribe<UILabel>(10, 1);
	entities->Subscribe<ParticleObject>(10, 1);
	// AI objects
	entities->Subscribe<AISprite>(10, 1);
	entities->Subscribe<Ghost>(4, 1);
	entities->Subscribe<Pacman>(1, 1);
	entities->Subscribe<PowerPallet>(4, 1);

	systems->Subscribe<AnimationSystem>();
	systems->Subscribe<RenderSystem>();
	systems->Subscribe<ParticleSystem>();
	systems->Subscribe<ColliderSystem>();
	systems->Subscribe<ScriptSystem>();
	systems->Subscribe<StateMachine>();

	systems->Get<StateMachine>()->AttachState<States::Entering>("GHOST_ENTERING");
	systems->Get<StateMachine>()->AttachState<States::Scatter>("GHOST_SCATTER");
	systems->Get<StateMachine>()->AttachState<States::Eaten>("GHOST_EATEN");
	systems->Get<StateMachine>()->AttachState<States::Frightened>("GHOST_FRIGHTENED");

	systems->Get<StateMachine>()->AttachState<States::BlinkyChase>("BLINKY_CHASE_STATE");
	systems->Get<StateMachine>()->AttachState<States::PinkyChase>("PINKY_CHASE_STATE");
	systems->Get<StateMachine>()->AttachState<States::ClydeChase>("CLYDE_CHASE_STATE");
	systems->Get<StateMachine>()->AttachState<States::ClydeInverseChase>("CLYDE_INVERSE_CHASE_STATE");
	systems->Get<StateMachine>()->AttachState<States::InkyChase>("INKY_CHASE_STATE");

	systems->Get<StateMachine>()->AttachState<States::Search>("PACMAN_SEARCH");
	systems->Get<StateMachine>()->AttachState<States::Avoid>("PACMAN_AVOID");
	systems->Get<StateMachine>()->AttachState<States::Hunt>("PACMAN_HUNT");
	systems->Get<StateMachine>()->AttachState<States::Dead>("PACMAN_DEAD");

	ReadMapData("Files/Data/map_data.txt");
	mapOffset.Set(
		static_cast<int>(mapSize.x * 0.5f) - 1,
		static_cast<int>(mapSize.y * 0.5f) + 1
	);

	gameMode = SCATTER;
	bounceTime = 0.f;
}

PacmanScene::~PacmanScene() {
	delete[] wallData; 
	delete[] palletData; 
}

void PacmanScene::Awake() {
	Scene::Awake();

	Events::EventsManager::GetInstance()->Subscribe("SPOT_AVAIL", &PacmanScene::MapWallHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("TAKE_PALLET", &PacmanScene::MapPalletHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("KEY_INPUT", &PacmanScene::KeyHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("GAME_MODE", &PacmanScene::GameModeHandler, this);

	auto cam = entities->Create<CameraObject>();
	cam->GetComponent<Camera>()->clearColor.Set(0.f);
	cam->GetComponent<Camera>()->SetSize(mapSize.h * 0.5f);

	auto grid = entities->Create<Sprite>();
	grid->GetComponent<Transform>()->translation.Set(.5f, -1.f, -1.f);
	grid->GetComponent<Transform>()->scale.Set(mapSize.x, mapSize.y - 5.f, 1.f);
	grid->GetComponent<Render>()->SetTexture(Load::TGA("Files/Textures/pacman_level.tga"));

	// most performant : 500 - 700 FPS
	auto fps = entities->Create<FPSLabel>();
	fps->GetComponent<Transform>()->translation.Set(mapSize.w, 0.f, 0.f);
	fps->GetComponent<Transform>()->scale.Set(4.f);
	fps->GetComponent<Render>()->tint.Set(0.01f);
	fps->GetComponent<Text>()->SetFont(Load::FNT("Files/Fonts/Microsoft.fnt", "Files/Fonts/Microsoft.tga"));
	fps->GetComponent<Text>()->text = "60";
	//fps->GetComponent<Text>()->scale = 0.25f;
	fps->GetComponent<Text>()->paragraphAlignment = PARAGRAPH_RIGHT;
	fps->GetComponent<Text>()->verticalAlignment = ALIGN_BOTTOM;

	auto blinky = SpawnGhost("BLINKY", 15, vec2f(12, 18), vec2f(13, 13), vec2f(1.f, 0.f));
	auto pinky = SpawnGhost("PINKY", 13, vec2f(-11, 18), vec2f(-12, 13), vec2f(-1.f, 0.f));
	auto clyde = SpawnGhost("CLYDE", 12, vec2f(-13, -17), vec2f(-12, -15), vec2f(0.f, 0.f));
	auto inky = SpawnGhost("INKY", 14, vec2f(14, -17), vec2f(13, -15), vec2f(2.f, 0.f));
	inky->SetPartner(blinky);

	SpawnPacman();

	for (float x = 0; x < mapSize.w; ++x) {
		for (float y = 0; y < mapSize.h; ++y) {
			const int i = static_cast<int>(y * mapSize.w + x);
			if (palletData[i].isTaken)
				palletData[i].object = SpawnPallet(x - mapOffset.x, mapSize.h - y - mapOffset.y);
		}
	}

	for (const auto& position : powerPosition) {
		SpawnPower(position.x - mapOffset.x, mapSize.h - position.y - mapOffset.y);
	}
}

void PacmanScene::FixedUpdate(const float& dt) {
	Scene::FixedUpdate(dt);

	bounceTime += dt;
	if (gameMode == CHASE) {
		if (bounceTime > 14.f) {
			Events::EventsManager::GetInstance()->Trigger("GAME_MODE", new Events::ModeEvent(SCATTER));
		}
	} else if (gameMode == SCATTER) {
		if (bounceTime > 7.f) {
			Events::EventsManager::GetInstance()->Trigger("GAME_MODE", new Events::ModeEvent(CHASE));
		}
	} else if (gameMode == FRIGHTENED) {
		if (bounceTime > 7.f) {
			Events::EventsManager::GetInstance()->Trigger("GAME_MODE", new Events::ModeEvent(END_FRIGHTENED));
		}
	} else if (gameMode == END_FRIGHTENED) {
		if (bounceTime > 3.f) {
			Events::EventsManager::GetInstance()->Trigger("GAME_MODE", new Events::ModeEvent(SCATTER));
		}
	}
}

void PacmanScene::KeyHandler(Events::Event* event) {

}

void PacmanScene::MapWallHandler(Events::Event* event) {
	auto mapEvent = static_cast<Events::MapData*>(event);
	auto position = mapEvent->position + mapOffset;
	position.y = static_cast<int>(mapSize.h) - position.y;

	*mapEvent->ref = !wallData[position.y * static_cast<int>(mapSize.w) + position.x];
}

void PacmanScene::MapPalletHandler(Events::Event* event) {
	auto mapEvent = static_cast<Events::MapData*>(event);
	auto position = mapEvent->position + mapOffset;
	position.y = static_cast<int>(mapSize.h) - position.y;

	const int i = position.y * static_cast<int>(mapSize.w) + position.x;
	*mapEvent->ref = palletData[i].isTaken;
	if (palletData[i].isTaken) {
		palletData[i].isTaken = false;
		palletData[i].object->Destroy();
	}
}

void PacmanScene::GameModeHandler(Events::Event* event) {
	bounceTime = 0;
	gameMode = static_cast<Events::ModeEvent*>(event)->data;
}

void PacmanScene::ReadMapData(const char* filepath) {
	std::ifstream ifs(filepath);

	ifs >> mapSize.w >> mapSize.h;

	int size = static_cast<int>(mapSize.w * mapSize.h);
	wallData = new bool[size];
	palletData = new PalletData[size];

	int powerCount = 0;

	char buffer;
	for (int i = 0; i < size; ++i) {
		ifs >> buffer;
		wallData[i] = buffer == '#';

		palletData[i].isTaken = buffer == '.';
		palletData[i].object = nullptr;

		if (buffer == 'O') {
			powerPosition[powerCount].Set(
				static_cast<float>(i % static_cast<int>(mapSize.w)), 
				static_cast<float>(i / static_cast<int>(mapSize.w))
			);
			++powerCount;
		}
	}

	ifs.close();
}

Ghost* const PacmanScene::SpawnGhost(const std::string& name, const int& tileColumn, const vec2f& dock, const vec2f& corner, const vec2f& start) {
	auto ghost = entities->Create<Ghost>();
	ghost->SetTag("GHOST");
	ghost->GetComponent<Transform>()->translation.Set(start, 0.f);

	Keyframe kf;
	kf.SetTilemapSize(16, 16);
	kf.duration = 0.1f;

	AnimationData anim;
	kf.SetCellRect(3, 13, 1, 1);
	anim.frames.push_back(kf);
	kf.SetCellRect(3, 12, 1, 1);
	anim.frames.push_back(kf);

	ghost->GetComponent<Animation>()->animations["FRIGHTENED"] = anim;

	anim.frames.clear();
	kf.SetCellRect(3, 13, 1, 1);
	anim.frames.push_back(kf);
	kf.SetCellRect(3, 10, 1, 1);
	anim.frames.push_back(kf);
	kf.SetCellRect(3, 12, 1, 1);
	anim.frames.push_back(kf);
	kf.SetCellRect(3, 11, 1, 1);
	anim.frames.push_back(kf);

	ghost->GetComponent<Animation>()->animations["END_FRIGHTENED"] = anim;


	anim.frames.clear();
	kf.SetCellRect(tileColumn, 15, 1, 1);
	anim.frames.push_back(kf);
	kf.SetCellRect(tileColumn, 14, 1, 1);
	anim.frames.push_back(kf);

	ghost->GetComponent<Animation>()->animations["RIGHT"] = anim;

	anim.frames.clear();
	kf.SetCellRect(tileColumn, 13, 1, 1);
	anim.frames.push_back(kf);
	kf.SetCellRect(tileColumn, 12, 1, 1);
	anim.frames.push_back(kf);

	ghost->GetComponent<Animation>()->animations["DOWN"] = anim;

	anim.frames.clear();
	kf.SetCellRect(tileColumn, 11, 1, 1);
	anim.frames.push_back(kf);
	kf.SetCellRect(tileColumn, 10, 1, 1);
	anim.frames.push_back(kf);

	ghost->GetComponent<Animation>()->animations["LEFT"] = anim;

	anim.frames.clear();
	kf.SetCellRect(tileColumn, 9, 1, 1);
	anim.frames.push_back(kf);
	kf.SetCellRect(tileColumn, 8, 1, 1);
	anim.frames.push_back(kf);

	ghost->GetComponent<Animation>()->animations["UP"] = anim;

	anim.frames.clear();
	kf.SetCellRect(11, 15, 1, 1);
	anim.frames.push_back(kf);
	ghost->GetComponent<Animation>()->animations["EATEN_RIGHT"] = anim;

	anim.frames.clear();
	kf.SetCellRect(11, 14, 1, 1);
	anim.frames.push_back(kf);
	ghost->GetComponent<Animation>()->animations["EATEN_DOWN"] = anim;

	anim.frames.clear();
	kf.SetCellRect(11, 13, 1, 1);
	anim.frames.push_back(kf);
	ghost->GetComponent<Animation>()->animations["EATEN_LEFT"] = anim;

	anim.frames.clear();
	kf.SetCellRect(11, 12, 1, 1);
	anim.frames.push_back(kf);
	ghost->GetComponent<Animation>()->animations["EATEN_UP"] = anim;

	ghost->GetComponent<Render>()->SetTexture(Load::TGA("Files/Textures/pacman_tilemap.tga"));
	ghost->GetComponent<Render>()->SetTilemapSize(16, 16);
	ghost->GetComponent<Render>()->SetCellRect(tileColumn, 15, 1, 1);
	ghost->GetComponent<StateContainer>()->queuedState = "GHOST_ENTERING";
	ghost->SetDock(dock);
	ghost->SetCorner(corner);
	ghost->SetChaseState(name + "_CHASE_STATE");
	ghost->SetSpeed(8.f);

	return ghost;
}

Entity* const PacmanScene::SpawnPacman() {
	auto pacman = entities->Create<Pacman>();
	pacman->SetTag("PACMAN");
	pacman->GetComponent<Transform>()->translation.Set(0.f, -9.f, 0.f);

	Keyframe kf;
	kf.SetTilemapSize(16, 16);
	kf.duration = 0.15f;

	AnimationData anim;
	kf.SetCellRect(10, 15, 1, 1);
	anim.frames.push_back(kf);
	kf.SetCellRect(10, 14, 1, 1);
	anim.frames.push_back(kf);
	kf.SetCellRect(10, 13, 1, 1);
	anim.frames.push_back(kf);
	kf.SetCellRect(10, 14, 1, 1);
	anim.frames.push_back(kf);

	pacman->GetComponent<Animation>()->animations["RIGHT"] = anim;
	pacman->GetComponent<Animation>()->currentAnimation = "RIGHT";

	anim.frames.clear();
	kf.SetCellRect(9, 15, 1, 1);
	anim.frames.push_back(kf);
	kf.SetCellRect(9, 13, 1, 1);
	anim.frames.push_back(kf);
	kf.SetCellRect(9, 14, 1, 1);
	anim.frames.push_back(kf);
	kf.SetCellRect(9, 13, 1, 1);
	anim.frames.push_back(kf);

	pacman->GetComponent<Animation>()->animations["LEFT"] = anim;

	anim.frames.clear();
	kf.SetCellRect(9, 12, 1, 1);
	anim.frames.push_back(kf);
	kf.SetCellRect(9, 10, 1, 1);
	anim.frames.push_back(kf);
	kf.SetCellRect(9, 11, 1, 1);
	anim.frames.push_back(kf);
	kf.SetCellRect(9, 10, 1, 1);
	anim.frames.push_back(kf);

	pacman->GetComponent<Animation>()->animations["UP"] = anim;

	anim.frames.clear();
	kf.SetCellRect(10, 12, 1, 1);
	anim.frames.push_back(kf);
	kf.SetCellRect(10, 11, 1, 1);
	anim.frames.push_back(kf);
	kf.SetCellRect(10, 10, 1, 1);
	anim.frames.push_back(kf);
	kf.SetCellRect(10, 11, 1, 1);
	anim.frames.push_back(kf);

	pacman->GetComponent<Animation>()->animations["DOWN"] = anim;

	anim.loop = false;
	anim.frames.clear();
	for (int i = 15; i > 7; --i) {
		kf.SetCellRect(8, i, 1, 1);
		anim.frames.push_back(kf);
	}
	for (int i = 15; i > 10; --i) {
		kf.SetCellRect(7, i, 1, 1);
		anim.frames.push_back(kf);
	}
	pacman->GetComponent<Animation>()->animations["DEAD"] = anim;



	pacman->GetComponent<Render>()->SetTexture(Load::TGA("Files/Textures/pacman_tilemap.tga"));
	pacman->GetComponent<Render>()->SetTilemapSize(16, 16);
	pacman->GetComponent<Render>()->SetCellRect(10, 14, 1, 1);
	pacman->GetComponent<StateContainer>()->queuedState = "PACMAN_SEARCH";
	pacman->SetTarget(vec3f(0.f, -9.f, 0.f));
	pacman->SetDestination(vec3f(16.f, -5.f, 0.f));
	pacman->SetSpeed(8.f);

	return pacman;
}

Entity* const PacmanScene::SpawnPallet(const float& x, const float& y) {
	auto pallet = entities->Create<Sprite>();
	pallet->SetTag("PALLET");
	pallet->GetComponent<Transform>()->translation.Set(x, y, 0.f);
	pallet->GetComponent<Transform>()->scale.Set(0.5f);
	pallet->GetComponent<Render>()->SetTexture(Load::TGA("Files/Textures/pacman_tilemap.tga"));
	pallet->GetComponent<Render>()->SetTilemapSize(128, 128);
	pallet->GetComponent<Render>()->SetCellRect(109, 53, 2, 2);

	return pallet;
}


Entity* const PacmanScene::SpawnPower(const float& x, const float& y) {
	auto pallet = entities->Create<PowerPallet>();
	pallet->SetTag("POWER");
	pallet->GetComponent<Transform>()->translation.Set(x, y, 0.f);
	pallet->GetComponent<Render>()->SetTexture(Load::TGA("Files/Textures/pacman_tilemap.tga"));
	pallet->GetComponent<Render>()->SetTilemapSize(32, 32);
	pallet->GetComponent<Render>()->SetCellRect(27, 15, 1, 1);

	return pallet;
}
