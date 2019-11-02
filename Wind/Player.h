#ifndef PLAYER_H
#define PLAYER_H

#include "Sprite.h"

#include <map>

class Player : public Sprite {

	std::map<int, int> keyInputs;

public:

	Player();

	void Build() override;
	void Initialize() override;

private:

	void Update(const float& dt);
	void KeyHandler(Events::Event* event);

	void Enter(Entity* const object);
	void Stay(Entity* const object);
	void Exit(Entity* const object);

};

#endif
