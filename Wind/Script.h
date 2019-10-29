#ifndef SCRIPT_H
#define SCRIPT_H

#include "Component.h"

#include <functional>

struct Script : Component {

	std::function<void()> awake;
	std::function<void()> reset;
	std::function<void()> start;

	std::function<void(const float&)> fixedUpdate;
	std::function<void(const float&)> update;

	std::function<void()> destroy;

	Script();

	void Initialize() override;
	void SetActive(const bool& state) override;

};

#endif