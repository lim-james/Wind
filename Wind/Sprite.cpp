#include "Sprite.h"

#include "Transform.h"
#include "Render.h"

void Sprite::Build() {
	AddComponent<Transform>();
	AddComponent<Render>();
}
