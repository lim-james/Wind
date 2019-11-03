#include "Player.h"

// Components
#include "Transform.h"
#include "ParticleEmitter.h"
#include "Collider.h"
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
	AddComponent<Collider>();
	AddComponent<Script>();
}

void Player::Initialize() {
	Sprite::Initialize();
	GetComponent<Script>()->update = std::bind(&Player::Update, this, std::placeholders::_1);

	GetComponent<Collider>()->BindCollisionEnter(&Player::Enter, this);
	GetComponent<Collider>()->BindCollisionStay(&Player::Stay, this);
	GetComponent<Collider>()->BindCollisionExit(&Player::Exit, this);

}

void Player::Update(const float& dt) {
	if (keyInputs[GLFW_KEY_W] != GLFW_RELEASE)
		GetComponent<Transform>()->translation.y += 10.f * dt;
	if (keyInputs[GLFW_KEY_S] != GLFW_RELEASE)
		GetComponent<Transform>()->translation.y -= 10.f * dt;
	if (keyInputs[GLFW_KEY_A] != GLFW_RELEASE)
		GetComponent<Transform>()->translation.x -= 10.f * dt;
	if (keyInputs[GLFW_KEY_D] != GLFW_RELEASE)
		GetComponent<Transform>()->translation.x += 10.f * dt;
	if (keyInputs[GLFW_KEY_SPACE] != GLFW_RELEASE)
		GetComponent<ParticleEmitter>()->age = 0.f;
}

void Player::KeyHandler(Events::Event* event) {
	Events::KeyInput* input = static_cast<Events::KeyInput*>(event);
	keyInputs[input->key] = input->action;
}

void Player::Enter(Entity* const object) {
	Console::Log << "Enter : " << object->tag << '\n';
	if (object->tag == "FOOD")
		Events::EventsManager::GetInstance()->TriggerContext("AI_STATE_CHANGE", object, new Events::Event); 
}

void Player::Stay(Entity* const object) {
	Console::Log << "Stay : " << object->tag << '\n';
}

void Player::Exit(Entity* const object) {
	Console::Log << "Exit : " << object->tag << '\n';
}