#include "UITableViewCell.h"

#include "Animation.h"

void UITableViewCell::Build() {
	Sprite::Build();
	AddComponent<Animation>();
}
