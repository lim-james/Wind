#include "FPSLabel.h"

// Components
#include "Text.h"
#include "Script.h"
// Events
#include "InputEvents.h"

#include <Events/EventsManager.h>

FPSLabel::FPSLabel() {
	Events::EventsManager::GetInstance()->Subscribe("KEY_INPUT", &FPSLabel::KeyHandler, this);
}

void FPSLabel::Build() {
	Sprite::Build();
	AddComponent<Text>();
	AddComponent<Script>();
}

void FPSLabel::Initialize() {
	Sprite::Initialize();
	GetComponent<Script>()->update = std::bind(&FPSLabel::Update, this, std::placeholders::_1);
	GetComponent<Script>()->fixedUpdate = std::bind(&FPSLabel::FixedUpdate, this, std::placeholders::_1);
}

void FPSLabel::Update(const float& dt) {
	FPS = static_cast<int>(1.f / dt);
}

void FPSLabel::FixedUpdate(const float& dt) {
	const int fixedFPS = static_cast<int>(1.f / dt);

	auto t = GetComponent<Text>();
	t->text = std::to_string(fixedFPS) + '\n' + std::to_string(FPS);
	if (FPS > 120)
		t->color.Set(0.f, 1.f, 1.f, 1.f);
	else if (FPS > 60)
		t->color.Set(0.f, 1.f, 0.f, 1.f);
	else
		t->color.Set(1.f, 0.f, 0.f, 1.f);
}

void FPSLabel::KeyHandler(Events::Event* event) {
	Events::KeyInput* input = static_cast<Events::KeyInput*>(event);
	keyInputs[input->key] = input->action;
}
