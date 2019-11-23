#ifndef SCRIPT_SYSTEM_H
#define SCRIPT_SYSTEM_H

#include "System.h"
#include "Script.h"

#include <Events/Event.h>

#include <vector>

class ScriptSystem : public System {

	std::vector<Script*> components;

public:

	void Start() override;

	void Initialize() override;
	void Update(const float& dt) override;
	void FixedUpdate(const float& dt) override;

private:

	void ActiveHandler(Events::Event* event);
};

#endif