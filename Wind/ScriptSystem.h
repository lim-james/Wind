#ifndef SCRIPT_SYSTEM_H
#define SCRIPT_SYSTEM_H

#include "System.h"
#include "Script.h"

#include <Events/Event.h>

#include <vector>

class ScriptSystem : public System {

	std::vector<Script*> components;

public:

	ScriptSystem();
	~ScriptSystem() override;

	void Initialize() override;
	void Update(const float& t) override;
	void FixedUpdate(const float& t) override;

private:

	void ActiveHandler(Events::Event* event);
};

#endif