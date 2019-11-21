#include "UIButton.h"

#include "Animation.h"
#include "Text.h"
#include "Button.h"

void UIButton::Build() {
	Sprite::Build();
	AddComponent<Animation>();
	AddComponent<Text>();
	AddComponent<Button>();
}
