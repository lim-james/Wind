#include "UIButton.h"

#include "Text.h"

void UIButton::Build() {
	Sprite::Build();
	AddComponent<Text>();
}
