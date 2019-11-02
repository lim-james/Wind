#include "AIScene.h"

// game objects
#include "Sprite.h"
#include "FPSLabel.h"
#include "CameraObject.h"
#include "Player.h"
#include "UILabel.h"
#include "ParticleObject.h"
#include "AISprite.h"
// components
#include "Transform.h"
#include "Render.h"
#include "Text.h"
#include "Camera.h"
#include "Particle.h"
#include "ParticleEmitter.h"
#include "Collider.h"
#include "Script.h"
// systems
#include "RenderSystem.h"
#include "ParticleSystem.h"
#include "ColliderSystem.h"
#include "ScriptSystem.h"
#include "StateSystem.h"
// Utils
#include "LoadTGA.h"
#include "LoadFNT.h"

#include "InputEvents.h"

#include <Math/Random.hpp>
#include <Events/EventsManager.h>
#include <GLFW/glfw3.h>

AIScene::AIScene() {
	components->Subscribe<Transform>(10, 1);
	components->Subscribe<Render>(10, 1);
	components->Subscribe<Text>(10, 1);
	components->Subscribe<Camera>(1, 1);
	components->Subscribe<ParticleEmitter>(10, 1);
	components->Subscribe<Particle>(100, 1);
	components->Subscribe<Collider>(10, 1);
	components->Subscribe<Script>(10, 1);
	components->Subscribe<StateMachine>(10, 1);

	entities->Subscribe<Sprite>(10, 1);
	entities->Subscribe<FPSLabel>(1, 1);
	entities->Subscribe<CameraObject>(1, 1);
	entities->Subscribe<Player>(1, 1);
	entities->Subscribe<UILabel>(10, 1);
	entities->Subscribe<ParticleObject>(10, 1);
	// AI objects
	entities->Subscribe<AISprite>(10, 1);

	systems->Subscribe<RenderSystem>();
	systems->Subscribe<ParticleSystem>();
	systems->Subscribe<ColliderSystem>();
	systems->Subscribe<ScriptSystem>();
	systems->Subscribe<StateSystem>();

	mapHalfSize = 10;

	//Events::EventsManager::GetInstance()->Subscribe("KEY_INPUT", &AIScene::KeyHandler, this);
}

void AIScene::Awake() {
	auto cam = entities->Create<CameraObject>();
	cam->GetComponent<Camera>()->clearColor.Set(0.f);
	cam->GetComponent<Camera>()->SetSize(static_cast<float>(mapHalfSize));

	//auto grid = entities->Create<Sprite>();
	//grid->GetComponent<Transform>()->scale.Set(2.f * static_cast<float>(mapHalfSize));
	//grid->GetComponent<Render>()->SetTexture(Load::TGA("Files/Textures/grid20.tga"));

	// most performant : 500 - 700 FPS
	auto fps = entities->Create<FPSLabel>();
	fps->GetComponent<Transform>()->translation.Set(15.f, 0.f, 0.f);
	fps->GetComponent<Transform>()->scale.Set(4.f);
	fps->GetComponent<Render>()->tint.Set(0.01f);
	fps->GetComponent<Text>()->SetFont(Load::FNT("Files/Fonts/Microsoft.fnt", "Files/Fonts/Microsoft.tga"));
	fps->GetComponent<Text>()->text = "60";
	//fps->GetComponent<Text>()->scale = 0.25f;
	fps->GetComponent<Text>()->paragraphAlignment = PARAGRAPH_RIGHT;
	fps->GetComponent<Text>()->verticalAlignment = ALIGN_BOTTOM;

	auto player = entities->Create<Player>();
	player->GetComponent<Transform>()->translation.Set(0.5f, 0.5f, 0.f);
	player->GetComponent<Render>()->tint.Set(1.f, 0.f, 0.f, 1.f);
	player->GetComponent<ParticleEmitter>()->offset.z = -0.1f;
	player->GetComponent<ParticleEmitter>()->duration = 0.5f;
	player->GetComponent<ParticleEmitter>()->burstAmount = 5;
	player->GetComponent<ParticleEmitter>()->spawnInterval = 0.01f;
	player->GetComponent<ParticleEmitter>()->lifetime = 3.f;
	player->GetComponent<ParticleEmitter>()->angleRange.z = 180.f;
	player->GetComponent<ParticleEmitter>()->speed = 30.f;
	player->GetComponent<ParticleEmitter>()->accelRad = -50.f;
	player->GetComponent<ParticleEmitter>()->accelRadRange = 10.f;
	//player->GetComponent<ParticleEmitter>()->positionRange.Set(2.f);
	player->GetComponent<ParticleEmitter>()->startSize.Set(1.f);
	player->GetComponent<ParticleEmitter>()->endSize.Set(0.f);
	player->GetComponent<ParticleEmitter>()->endColor.Set(0.f);
	//player->GetComponent<ParticleEmitter>()->startColor.Set(1.f, 1.f, 0.f, 1.f);
	//player->GetComponent<ParticleEmitter>()->startColorRange.Set(.1f, .1f, 0.f, 0.f);
	//player->GetComponent<ParticleEmitter>()->endColor.Set(1.f, 0.f, 0.f, 0.5f);
	//player->GetComponent<ParticleEmitter>()->endColorRange.Set(0.1f, 0., 0.f, 0.5f);
	//player->GetComponent<ParticleEmitter>()->gravity.Set(0.f, -9.8f, 0.f);

	for (unsigned i = 0; i < 10; ++i) {
		vec2f position(
			static_cast<float>(Math::RandMinMax(-mapHalfSize, mapHalfSize)),
			static_cast<float>(Math::RandMinMax(-mapHalfSize, mapHalfSize))
		);
		position += vec2f(0.5f);

		auto food = entities->Create<AISprite>();
		food->tag = "FOOD";
		food->GetComponent<Transform>()->translation.Set(position, 0.f);
		food->GetComponent<Transform>()->scale.Set(0.8f);
		food->SetTarget(vec3f(position, 0.f));
		food->GetComponent<Render>()->tint.Set(1.f, 0.5f, 0.f, 1.f);
	}
}

void AIScene::KeyHandler(Events::Event* event) {
	auto input = static_cast<Events::KeyInput*>(event);

	if (input->key == GLFW_KEY_SPACE && input->action == GLFW_RELEASE) {
		vec2f position(
			static_cast<float>(Math::RandMinMax(-mapHalfSize, mapHalfSize)),
			static_cast<float>(Math::RandMinMax(-mapHalfSize, mapHalfSize))
		);
		position += vec2f(0.5f);

		auto fish = entities->Create<AISprite>();
		fish->tag = "FISH";
		fish->GetComponent<Transform>()->translation.Set(position, 0.f);
		fish->GetComponent<Transform>()->scale.Set(0.8f);
		fish->SetTarget(vec3f(position, 0.f));
		fish->GetComponent<Render>()->tint.Set(0.f, 0.f, 1.f, 1.f);
	}
}
