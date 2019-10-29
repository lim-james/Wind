#ifndef SHARK_SPRITE_H
#define SHARK_SPRITE_H

#include "AISprite.h"

class SharkSprite : public AISprite {

public:

	void Build() override;
	void Initialize() override;

private:

	void Update(const float& dt);

};

#endif
