#ifndef FPS_LABEL_H
#define FPS_LABEL_H

#include "Sprite.h"

#include <map>

class FPSLabel : public Sprite {

	std::map<int, int> keyInputs;
	int FPS;

public:

	FPSLabel();

	void Build() override;
	void Initialize() override;

private:

	void Update(const float& dt);
	void FixedUpdate(const float& dt);

	void KeyHandler(Events::Event* event);

};


#endif
