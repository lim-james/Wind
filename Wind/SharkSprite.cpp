#include "SharkSprite.h"

#include "Transform.h"
#include "Render.h"
#include "Script.h"

#include <Logger/Logger.h>

void SharkSprite::Build() {
	AddComponent<Transform>();
	AddComponent<Render>();
	AddComponent<Script>();
}

void SharkSprite::Initialize() {
	Entity::Initialize();
	GetComponent<Script>()->update = std::bind(&SharkSprite::Update, this, std::placeholders::_1);
}

void SharkSprite::Update(const float& dt) {
	Console::Log << "Shark : " << dt << '\n';
}
