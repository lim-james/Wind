#ifndef BUTTON_H
#define BUTTON_H

#include "Component.h"

#include <functional>

#define MOUSE_CLICK	0
#define MOUSE_DOWN	1
#define MOUSE_OUT	2
#define MOUSE_OVER	3
#define MOUSE_UP	4
#define BUTTON_ACTION_COUNT 5

struct Button : Component {

	bool isEnabled;

	Button();

	void Initialize() override;
	void SetActive(const bool& state) override;

	template<typename Context>
	void BindHandler(void(Context::*callback)(Entity*), Context* context, const unsigned& action);
	template<typename Context>
	void BindHandler(void(Context::*callback)(Entity*) const, Context* context, const unsigned& action);

private:

	std::function<void(Entity*)> handlers[BUTTON_ACTION_COUNT];

	friend class ButtonSystem;

};

template<typename Context>
void Button::BindHandler(void(Context::*callback)(Entity*), Context* context, const unsigned& action) {
	handlers[action] = std::bind(callback, context, std::placeholders::_1);
}

template<typename Context>
void Button::BindHandler(void(Context::*callback)(Entity*) const, Context* context, const unsigned& action) {
	handlers[action] = std::bind(callback, context, std::placeholders::_1);
}

#endif
