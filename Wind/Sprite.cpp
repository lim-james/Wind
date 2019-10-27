#include "Sprite.h"

#include "Transform.h"
#include "Render.h"
#include "Text.h"

void Sprite::Build() {
	AddComponent<Transform>();
	AddComponent<Render>();
	AddComponent<Text>();
}
