#include "UITableViewCell.h"

#include "Animation.h"
#include "Button.h"

void UITableViewCell::Build() {
	Sprite::Build();
	AddComponent<Animation>();
	AddComponent<Button>();
}
