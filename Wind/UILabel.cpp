#include "UILabel.h"

#include "Text.h"

void UILabel::Build() {
	Sprite::Build();
	AddComponent<Text>();
}
