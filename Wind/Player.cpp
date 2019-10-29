#include "Player.h"

// Components
#include "Transform.h"
#include "ParticleEmitter.h"
#include "Script.h"
// Events
#include "InputEvents.h"

#include <Events/EventsManager.h>
#include <GLFW/glfw3.h>

Player::Player() {
	Events::EventsManager::GetInstance()->Subscribe("KEY_INPUT", &Player::KeyHandler, this);
}

void Player::Build() {
	Sprite::Build();
	AddComponent<ParticleEmitter>();
	AddComponent<Script>();
}

void Player::Initialize() {
	Sprite::Initialize();
	GetComponent<Script>()->update = std::bind(&Player::Update, this, std::placeholders::_1);
}

void Player::Update(const float& dt) {
	if (keyInputs[GLFW_KEY_W] == GLFW_PRESS)
		GetComponent<Transform>()->translation.y += 10.f * dt;
	if (keyInputs[GLFW_KEY_S] == GLFW_PRESS)
		GetComponent<Transform>()->translation.y -= 10.f * dt;
	if (keyInputs[GLFW_KEY_A] == GLFW_PRESS)
		GetComponent<Transform>()->translation.x -= 10.f * dt;
	if (keyInputs[GLFW_KEY_D] == GLFW_PRESS)
		GetComponent<Transform>()->translation.x += 10.f * dt;
	if (keyInputs[GLFW_KEY_SPACE] == GLFW_PRESS)
		GetComponent<ParticleEmitter>()->age = 0.f;
}

void Player::KeyHandler(Events::Event* event) {
	Events::KeyInput* input = static_cast<Events::KeyInput*>(event);
	keyInputs[input->key] = input->action;
}
